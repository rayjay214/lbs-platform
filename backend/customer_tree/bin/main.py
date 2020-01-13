from businessdb import BusinessDb
from ctree import CustomerTree
from ctree_servicer import CTreeServicer, GrpcThread
from globals import g_logger, g_cfg
from kafka_consumer import KafkaConsumer
from readerwriterlock import rwlock

def main():
    db = BusinessDb(g_cfg['BUSINESS_DB'])
    ctree_lock = rwlock.RWLockWrite()
    ctree = CustomerTree(db, ctree_lock)
    ctree.init_fromdb()
    ctree.dump_tree()
    servicer = GrpcThread(ctree, ctree_lock)
    servicer.daemon = True
    consumer = KafkaConsumer(ctree)
    consumer.daemon = True
    servicer.start()
    consumer.start()
    servicer.join()
    consumer.join()


if __name__ == '__main__':
    main()

