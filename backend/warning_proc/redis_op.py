import redis

class RedisOp():
    def __init__(self, cfg):
        self.host = cfg['host']
        self.port = cfg['port']
        self.redis = redis.StrictRedis(host=self.host, port=self.port, charset='utf-8', decode_responses=True)
        self.pipe = self.redis.pipeline()

    def getFenceInfoById(self, dev_id):
        dev_info = self.redis.hgetall('fence:{}'.format(dev_id))
        return dev_info

    def setFenceStatusById(self, dev_id, status):
        key = 'fence:{}'.format(dev_id)
        val = str(status)
        self.redis.hset(key,'status',val)
        return 0


