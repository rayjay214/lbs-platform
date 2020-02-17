from businessdb import BusinessDb
from globals import g_cfg, g_logger
import redis

r = redis.Redis(host='127.0.0.1', port=8800)
db = BusinessDb(g_cfg['BUSINESS_DB'])
dev_gen = db.load_all_device()
pipe = r.pipeline()
for device in dev_gen:
    device_key = 'device:{}'.format(device['dev_id'])
    imei_key = 'imei:{}'.format(device['imei'])
    #r.hmset(device_key, device)
    #r.set(imei_key, device['dev_id'])
    pipe.hmset(device_key, device)
    pipe.set(imei_key, device['dev_id'])
    print(device)

print(pipe.command_stack)
print(len(pipe.command_stack))
pipe.execute()

