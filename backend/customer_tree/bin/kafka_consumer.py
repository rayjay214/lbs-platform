from confluent_kafka import Consumer, KafkaError
import threading
from globals import g_logger, g_cfg
import json

class KafkaConsumer(threading.Thread):
    def __init__(self, ctree):
        threading.Thread.__init__(self)
        self.consumer =  Consumer({
            'bootstrap.servers': g_cfg['KAFKA']['broker'],
            'group.id': g_cfg['KAFKA']['group'],
            'auto.offset.reset': 'earliest'})
        self.ctree = ctree

    def get_handler(self, action, table):
        return {
            'insert' : {
                't_device' : self.ctree.insert_device,
                't_enterprise' : self.ctree.insert_ent
            },
            'update' : {
                't_device' : self.ctree.update_device,
                't_enterprise' : self.ctree.update_ent
            },
            'delete' : {
                't_device' : self.ctree.delete_device,
                't_enterprise' : self.ctree.delete_ent
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
            msg = self.consumer.poll(2.0)
            if msg:
                print(msg.value())
            if msg is None:
                continue
            if msg.error():
                g_logger.info("Consumer error: {}".format(msg.error()))
                continue

            g_logger.info('Received message: {}'.format(msg.value().decode('utf-8')))
            self.process_msg(msg.value())

        self.consumer.close()

