from businessdb import BusinessDb
from redis_syncer import RedisSyncer
from globals import g_logger, g_cfg
from kafka_consumer import KafkaConsumer

def main():
    db = BusinessDb(g_cfg['BUSINESS_DB'])
    redis_syncer = RedisSyncer(db)
    redis_syncer.init_fromdb()
    redis_syncer.daemon = True
    consumer = KafkaConsumer(redis_syncer)
    consumer.daemon = True
    redis_syncer.start()
    consumer.start()
    redis_syncer.join()
    consumer.join()


if __name__ == '__main__':
    main()

