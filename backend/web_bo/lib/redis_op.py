import redis

class RedisOp():
    def __init__(self, cfg):
        self.host = cfg['host']
        self.port = cfg['port']
        self.redis = redis.Redis(host=self.host, port=self.port)
        self.pipe = self.redis.pipeline()

    def getDeviceInfos(self, dev_ids):
        for dev_id in dev_ids:
            self.pipe.hgetall('device:{}'.format(dev_id))
        dev_infos = self.pipe.execute()
        return dev_infos