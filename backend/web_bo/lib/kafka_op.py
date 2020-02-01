import dev_pb2
from globals import g_logger, g_cfg
from confluent_kafka import Producer

class KafkaOp():
    def __init__(self, cfg):
        self.broker = cfg['broker']
        self.cmd_topic = cfg['cmd_topic']
        self.producer = Producer({'bootstrap.servers': self.broker})

    def delivery_report(self, err, msg):
        if err is not None:
            g_logger.info('Message delivery failed: {}'.format(err))
        else:
            g_logger.info('Message delivered to {} [{}]'.format(msg.topic(), msg.partition()))

    def produce_cmd(self, str):
        self.producer.produce(self.cmd_topic, str, callback=self.delivery_report)