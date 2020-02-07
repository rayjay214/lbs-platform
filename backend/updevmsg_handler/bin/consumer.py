import threading
from confluent_kafka import Consumer
from globals import g_logger, g_cfg
from dev_pb2 import MsgType, UpDevMsg
from handler import Handler
import traceback

#todo use process
class UpDevMsgConsumer(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.consumer = Consumer({
            'bootstrap.servers': g_cfg['kafka']['broker'],
            'group.id': g_cfg['kafka']['group'],
            'auto.offset.reset': 'earliest'})
        self.handler = Handler()

    def get_handler(self, msg_type):
        return {
            MsgType.kCommandRsp : self.handler.proc_cmdresp,
            MsgType.kAlarmMsg : self.handler.proc_alarm,
            MsgType.kIccidMsg : self.handler.proc_iccid
        }.get(msg_type, None)

    def process_msg(self, msg):
        updev_msg = UpDevMsg()
        updev_msg.ParseFromString(msg)
        g_logger.info(updev_msg)
        handle = self.get_handler(updev_msg.msgtype)
        if handle is None:
            g_logger.error('handle not found, updev_msg {} invalid'.format(updev_msg))
            return
        handle(updev_msg)

    def run(self):
        self.consumer.subscribe(g_cfg['kafka']['topic'].split(','))
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
                g_logger.error(traceback.format_exc())
                g_logger.error(e)

        self.consumer.close()

