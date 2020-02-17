from globals import g_logger, g_cfg
from businessdb import BusinessDb
import redis
import threading
import time

to_del_keys_in_events = ['action', 'table', 'schema']

class RedisSyncer(threading.Thread):
    def __init__(self, db):
        threading.Thread.__init__(self)
        self.root = None
        self.data_source = db
        self.redis = redis.Redis(host=g_cfg['REDIS']['host'], port=int(g_cfg['REDIS']['port']))
        self.pipe = self.redis.pipeline()
        self.lock = threading.Lock()

    def run(self):
        while True:
            time.sleep(2)
            with self.lock:
                self.pipe.execute()

    def set_device2redis(self, device):
        device_key = 'device:{}'.format(device['dev_id'])
        imei_key = 'imei:{}'.format(device['imei'])
        with self.lock:
            self.pipe.hmset(device_key, device)
            self.pipe.set(imei_key, device['dev_id'])
            self.commit()

    def del_device_from_redis(self, device):
        device_key = 'device:{}'.format(device['dev_id'])
        imei_key = 'imei:{}'.format(device['imei'])
        with self.lock:
            self.pipe.delete(device_key)
            self.pipe.delete(imei_key)
            self.commit()

    def init_fromdb(self):
        device_gen = self.data_source.load_all_device()
        for device in device_gen:
            self.set_device2redis(device)

    def commit(self):
        if len(self.pipe.command_stack) > int(g_cfg['REDIS']['maxbuffered_cmds']):
            print(len(self.pipe.command_stack))
            self.pipe.execute()

    '''methods below are called within other threads'''
    def insert_device(self, event):
        device = {k:v for k,v in event.items() if k not in to_del_keys_in_events}
        self.set_device2redis(device)

    def update_device(self, event):
        device = {k:v for k,v in event.items() if k not in to_del_keys_in_events and not k.startswith('old_')}
        self.set_device2redis(device)

    def delete_device(self, event):
        device = {k:v for k,v in event.items() if k not in to_del_keys_in_events}
        self.del_device_from_redis(device)

