import threading
from confluent_kafka import Consumer
from confluent_kafka import Producer
from google.protobuf import text_format
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
        self.producer = Producer({'bootstrap.servers': g_cfg['kafka']['broker']})
        self.handler = Handler()

    def get_handler(self, msg_type):
        return {
            MsgType.kGpsMsg : self.handler.proc_gps
        }.get(msg_type, None)

    def send_alarm(self, updev_msg, alarm_type):
        msg = UpDevMsg()
        msg.msgtype = MsgType.kAlarmMsg
        msg.alarm.id = updev_msg.gps.id
        msg.alarm.imei =  updev_msg.gps.imei
        msg.alarm.alarmtime = updev_msg.gps.datetime
        msg.alarm.alarmtype = alarm_type
        msg.alarm.lng = updev_msg.gps.lng
        msg.alarm.lat = updev_msg.gps.lat
        msg.alarm.speed = updev_msg.gps.speed
        msg.alarm.route = updev_msg.gps.route
            
        sMsg = msg.SerializeToString()
        debug_msg = text_format.MessageToString(msg, as_utf8=True)
        g_logger.debug('alarm msg:'+debug_msg)
        self.producer.produce('gw.alarm', sMsg)
        self.producer.flush()
        return 0

    def process_msg(self, msg):
        updev_msg = UpDevMsg()
        updev_msg.ParseFromString(msg)
        g_logger.info(updev_msg)
        handle = self.get_handler(updev_msg.msgtype)
        if handle is None:
            g_logger.error('handle not found, updev_msg {} invalid'.format(updev_msg))
            return
        alarm_type = handle(updev_msg)
        if alarm_type != 0:
            self.send_alarm(updev_msg, alarm_type)
        return

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
        self.producer.close()

