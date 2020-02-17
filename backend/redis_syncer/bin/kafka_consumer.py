from confluent_kafka import Consumer, KafkaError
import threading
from globals import g_logger, g_cfg
import json

class KafkaConsumer(threading.Thread):
    def __init__(self, redis_syncer):
        threading.Thread.__init__(self)
        self.consumer =  Consumer({
            'bootstrap.servers': g_cfg['KAFKA']['broker'],
            'group.id': g_cfg['KAFKA']['group'],
            'auto.offset.reset': 'earliest'})
        self.redis_syncer = redis_syncer

    def get_handler(self, action, table):
        return {
            'insert' : {
                't_device' : self.redis_syncer.insert_device,
            },
            'update' : {
                't_device' : self.redis_syncer.update_device,
            },
            'delete' : {
                't_device' : self.redis_syncer.delete_device,
            }
        }.get(action, {}).get(table)

    def process_msg(self, msg):
        event = json.loads(msg)
        handle = self.get_handler(event['action'], event['table'])
        if handle is None:
            g_logger.error('invalid msg {}'.format(msg))
            return
        handle(event)

    def run(self):
        self.consumer.subscribe(g_cfg['KAFKA']['topic'].split(','))
        while True:
            msg = self.consumer.poll(1.0)
            if msg:
                g_logger.debug(msg.value())
            if msg is None:
                continue
            if msg.error():
                g_logger.warn("Consumer error: {}".format(msg.error()))
                continue

            g_logger.info('Received message: {}'.format(msg.value().decode('utf-8')))
            self.process_msg(msg.value())

        self.consumer.close()

