from businessdb import BusinessDb
from globals import g_logger, g_cfg
from kafka_handler import CmdReqConsumer, CmdRespConsumer, CmdProducer
import queue

def main():
    myqueue = queue.Queue()
    cmdreq_consumer = CmdReqConsumer(myqueue)
    cmdreq_consumer.daemon = True
    cmdresp_consumer = CmdRespConsumer()
    cmdresp_consumer.daemon = True
    cmd_producer = CmdProducer(myqueue)
    cmd_producer.daemon = True
    cmd_producer.start()
    cmdreq_consumer.start()
    cmdresp_consumer.start()
    cmdreq_consumer.join()
    cmdresp_consumer.join()
    cmd_producer.join()

if __name__ == '__main__':
    main()

