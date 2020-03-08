from globals import g_logger, g_cfg
from consumer import UpDevMsgConsumer

def main():
    consumer = UpDevMsgConsumer()
    consumer.daemon = True
    consumer.start()
    consumer.join()

if __name__ == '__main__':
    main()

