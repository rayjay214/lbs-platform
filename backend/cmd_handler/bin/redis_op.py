import redis

class RedisOp():
    def __init__(self, cfg):
        self.host = cfg['host']
        self.port = cfg['port']
        self.redis = redis.StrictRedis(host=self.host, port=self.port, charset='utf-8', decode_responses=True)
        self.pipe = self.redis.pipeline()

    def getDeviceInfoByImei(self, imei):
        dev_id = self.redis.get('imei:{}'.format(imei))
        return self.getDeviceInfoById(dev_id)


