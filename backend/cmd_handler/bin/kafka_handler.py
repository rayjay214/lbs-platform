import json
import sys

import threading
import queue
import time
from confluent_kafka import Producer, Consumer
from globals import g_logger, g_cfg
from dev_pb2 import DownDevMsg, MsgType, UpDevMsg

#consume cmd.content from web_bo
class CmdReqConsumer(threading.Thread):
    def __init__(self, queue):
        threading.Thread.__init__(self)
        self.consumer = Consumer({
            'bootstrap.servers': g_cfg['kafka']['broker'],
            'group.id': g_cfg['kafka']['group'],
            'auto.offset.reset': 'earliest'})
        self.queue = queue

    def process_msg(self, msg):
        down_devmsg = DownDevMsg()
        down_devmsg.ParseFromString(msg)
        g_logger.info(down_devmsg)

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

