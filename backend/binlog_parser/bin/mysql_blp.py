import json
import sys

from pymysqlreplication import BinLogStreamReader
from pymysqlreplication.row_event import (DeleteRowsEvent, UpdateRowsEvent, WriteRowsEvent)

from bson import json_util
import datetime
import threading
import queue
import time
from confluent_kafka import Producer
from globals import g_logger, g_cfg

#fetch data from mysql binlog and enqueue
class BLPProducer(threading.Thread):
    def __init__(self, name, queue, mysql_setting):
        threading.Thread.__init__(self, name=name)
        self.queue = queue
        self.setting = mysql_setting
        self.stream = BinLogStreamReader(
            connection_settings=self.setting,
            server_id=3,
            only_events=[DeleteRowsEvent, WriteRowsEvent, UpdateRowsEvent],
            blocking=True,
            resume_stream=True)

    def run(self):
        for binlogevent in self.stream:
            for row in binlogevent.rows:
                event = {"schema": binlogevent.schema, "table": binlogevent.table}

                if isinstance(binlogevent, DeleteRowsEvent):
                    event["action"] = "delete"
                    event = dict(**row["values"], **event)
                elif isinstance(binlogevent, UpdateRowsEvent):
                    event["action"] = "update"
                    diffs = {'old_'+diff: row["before_values"][diff] for diff in row["before_values"] if row["before_values"][diff] != row["after_values"][diff]}
                    event = dict(**row["after_values"], **diffs, **event)
                elif isinstance(binlogevent, WriteRowsEvent):
                    event["action"] = "insert"
                    event = dict(**row["values"], **event)
                #sys.stdout.flush()
                self.queue.put(event)

            self.stream.close()

#dequeue and write to kafka
class BLPConsumer(threading.Thread):
    def __init__(self, name, queue, kafka_setting):
        threading.Thread.__init__(self, name=name)
        self.queue = queue
        self.setting = kafka_setting
        self.producer = Producer({'bootstrap.servers': self.setting['broker']})

    def myconverter(self, o):
        if isinstance(o, datetime.datetime):
            return o.__str__()

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


    def process_event(self, event):
        topic = self.setting.get(event['table'])
        if topic is None:
            g_logger.info('no need to process table {}'.format(event['table']))
            return
        json_str = json.dumps(event, ensure_ascii=False, default=self.myconverter)
        #logger.info('topic is {}'.format(topic))
        g_logger.info(json_str)
        self.producer.produce(topic, json_str, callback=self.delivery_report)

if __name__ == "__main__":
    mysql_setting = {
        'host': g_cfg['MYSQL']['host'],
        'port': int(g_cfg['MYSQL']['port']),
        'user': g_cfg['MYSQL']['user'],
        'passwd': g_cfg['MYSQL']['passwd']
    }

    kafka_setting = {
        't_device' : g_cfg['KAFKA']['t_device'],
        't_enterprise' : g_cfg['KAFKA']['t_enterprise'],
        't_fenceinfo' : g_cfg['KAFKA']['t_fenceinfo'],
        'broker' : g_cfg['KAFKA']['broker']
    }
    queue = queue.Queue()
    blp_producer = BLPProducer('Binlog Parser', queue, mysql_setting)
    blp_producer.daemon = True
    blp_consumer = BLPConsumer('kakfa Writer', queue, kafka_setting)
    blp_consumer.daemon = True
    blp_producer.start()
    blp_consumer.start()
    blp_producer.join()
    blp_consumer.join()

