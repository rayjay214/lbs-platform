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

    def getDeviceRunInfoById(self, dev_id):
        run_info = self.redis.hgetall('devruninfo:{}'.format(dev_id))
        return run_info

    def getBmsInfoById(self, dev_id):
        bms_info = self.redis.hgetall('bms:{}'.format(dev_id))
        return bms_info

    def getDeviceRunInfos(self, dev_ids):
        for dev_id in dev_ids:
            self.pipe.hgetall('devruninfo:{}'.format(dev_id))
        run_infos = self.pipe.execute()
        filtered_run_infos = [i for i in run_infos if len(i) > 0]
        return filtered_run_infos

    def setBmsRelayStatus(self, dev_id, flag):
        self.redis.hset('bms:{}'.format(dev_id), 'relay_status', flag)