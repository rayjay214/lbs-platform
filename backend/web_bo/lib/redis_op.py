import redis

class RedisOp():
    def __init__(self, cfg):
        self.host = cfg['host']
        self.port = cfg['port']
        self.redis = redis.StrictRedis(host=self.host, port=self.port, charset='utf-8', decode_responses=True)
        self.pipe = self.redis.pipeline()

    def getDeviceInfos(self, dev_ids, pageno=0, pagesize=20):
        for dev_id in dev_ids[pageno*pagesize:pageno*pagesize+pagesize]:
            self.pipe.hgetall('device:{}'.format(dev_id))
        dev_infos = self.pipe.execute()
        return dev_infos

    def getDeviceInfoById(self, dev_id):
        dev_info = self.redis.hgetall('device:{}'.format(dev_id))
        return dev_info

    def getDeviceInfoByImei(self, imei):
        dev_id = self.redis.get('imei:{}'.format(imei))
        return self.getDeviceInfoById(dev_id)