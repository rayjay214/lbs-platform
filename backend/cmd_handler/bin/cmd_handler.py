import json
import sys

import threading
import queue
import time
from confluent_kafka import Producer, Consumer
from globals import g_logger, g_cfg
from dev_pb2 import DownDevMsg, MsgType, UpDevMsg
from redis_op import RedisOp
import struct

def num_to_bcd(num):
    packed = struct.pack('6B', *[(num // 10 ** i % 10 << 4) + (num // 10 ** (i - 1) % 10) for i in range(11, -1, -2)])
    # print(''.join(f"\\{p:02x}" for p in packed))
    return packed

def pack_char(character):
    return struct.pack('c', character.to_bytes(1, byteorder='big'))

def pack_signed_char(character):
    return struct.pack('b', character)

def xor_bytes(byteStr):  # 按字节异或
    if len(byteStr) == 1:
        return byteStr
    result = (struct.unpack('b', pack_char(byteStr[0])))[0]  # 字符转成 signed char（对应python中的 int ） 后进行异或
    for c in byteStr[1:]:
        signed_c = (struct.unpack('b', pack_char(c)))[0]
        result = result ^ signed_c
    return pack_signed_char(result)

#consume cmd.content from web_bo
class CmdReqConsumer(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.consumer = Consumer({
            'bootstrap.servers': g_cfg['kafka']['broker'],
            'group.id': g_cfg['kafka']['group'],
            'auto.offset.reset': 'earliest'})
        self.queue = queue
        self.counter = 1

    def get_parser(self, protocol):
        return {
            't808' : self.t808_parser
        }.get(protocol, None)

    def trans0x7e(self, b_str):
        b_dst = bytearray(1024)
        b_dst[0] = b_str[0]
        idx = 1
        for ele in b_str[1:-1]:
            if ele == 0x7e:
                b_dst[idx] = 0x7d
                idx += 1
                b_dst[idx] = 0x02
                idx += 1
            elif ele == 0x7d:
                b_dst[idx] = 0x7d
                idx += 1
                b_dst[idx] = 0x01
                idx += 1
            else:
                b_dst[idx] = ele
                idx += 1
        b_dst[idx] = b_str[-1]
        idx += 1
        b_final = bytes(b_dst[:idx])
        return b_final

    def t808_parser(self, down_devmsg):
        buf = bytearray(1024)
        buf_idx = 0
        struct.pack_into('>B', buf, buf_idx, 0x7e)
        buf_idx += 1
        struct.pack_into('>H', buf, buf_idx, 0x8300)
        buf_idx += 2
        struct.pack_into('>H', buf, buf_idx, len(down_devmsg.cmdreq.content.encode()))
        buf_idx += 2
        ba_phone = num_to_bcd(int(down_devmsg.cmdreq.imei))
        struct.pack_into('>6s', buf, buf_idx, ba_phone)
        buf_idx += 6
        struct.pack_into('>H', buf, buf_idx, self.counter)
        self.counter += 1
        buf_idx += 2
        struct.pack_into('>B', buf, buf_idx, 0x01)
        buf_idx += 1
        struct.pack_into('>{}s'.format(len(down_devmsg.cmdreq.content.encode())),
                    buf, buf_idx, down_devmsg.cmdreq.content.encode())
        buf_idx += len(down_devmsg.cmdreq.content.encode())
        xor = xor_bytes(buf[1:buf_idx])
        struct.pack_into('>1s', buf, buf_idx, xor)
        buf_idx += 1
        struct.pack_into('>B', buf, buf_idx, 0x7e)
        b_str = buf[:buf_idx + 1]
        b_final = self.trans0x7e(b_str)
        down_devmsg.comreq.content = b_final
        print(down_devmsg)

    def get_protocol_by_type(self, product_type):
        for key in g_cfg['protocol']:
            types = g_cfg['protocol'][key].split(',')
            if product_type in types:
                return key
        return None

    def process_msg(self, msg):
        down_devmsg = DownDevMsg()
        down_devmsg.ParseFromString(msg)
        g_logger.info(down_devmsg)
        redis_op = RedisOp(g_cfg['redis'])
        dev_info = redis_op.getDeviceInfoByImei(down_devmsg.cmdreq.imei)
        protocol = self.get_protocol_by_type(dev_info['product_type'])
        if protocol is None:
            g_logger.error('protocol not found, type:{}'.format(dev_info['product_type']))
            return
        parser = self.get_parser(protocol)
        if parser is None:
            g_logger.error('parser not found, protocol {}'.format(protocol))
            return
        parser(down_devmsg)

    def run(self):
        topics = []
        topics.append(g_cfg['kafka']['cmdreq_topic'])
        self.consumer.subscribe(topics)
        while True:
            try:
                msg = self.consumer.poll(1)
                if msg is None:
                    continue
                if msg.error():
                    g_logger.info("Consumer error: {}".format(msg.error()))
                    continue

                self.process_msg(msg.value())
            except Exception as e:
                g_logger.error(e)

        self.consumer.close()

#consume cmd.resp from gw
class CmdRespConsumer(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.consumer = Consumer({
            'bootstrap.servers': g_cfg['kafka']['broker'],
            'group.id': g_cfg['kafka']['group'],
            'auto.offset.reset': 'earliest'})

    def process_msg(self, str):
        updev_msg = UpDevMsg()
        updev_msg.pasreFromString(str)
        print(updev_msg)

    def run(self):
        self.consumer.subscribe(list(g_cfg['kafka']['cmdresp_topic']))
        while True:
            try:
                msg = self.consumer.poll(1)
                if msg is None:
                    continue
                if msg.error():
                    g_logger.info("Consumer error: {}".format(msg.error()))
                    continue

                g_logger.info('Received message: {}'.format(msg.value().decode('utf-8')))
                self.process_msg(msg.value())
            except Exception as e:
                g_logger.error(e)

        self.consumer.close()

#after constructing binary
class CmdProducer(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.queue = queue
        self.producer = Producer({'bootstrap.servers': g_cfg['kafka']['broker']})

    def run(self):
        while True:
            self.producer.poll(0)
            event = self.queue.get()
            if event is None:
                time.sleep(0.1)
            self.process_event(event)
            self.queue.task_done()

    def delivery_report(self, err, msg):
        if err is not None:
            g_logger.info('Message delivery failed: {}'.format(err))
        else:
            g_logger.info('Message delivered to {} [{}]'.format(msg.topic(), msg.partition()))

    def process_msg(self, msg):
        topic = self.setting.get(g_cfg['kafka']['cmd_binaryreq_topic'])
        g_logger.info(msg)
        self.producer.produce(topic, msg, callback=self.delivery_report)

