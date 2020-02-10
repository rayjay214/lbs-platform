from cassandra.cluster import Cluster
from globals import g_logger, g_cfg

import arrow

class CassandraOp():
    def __init__(self):
        self.cluster = Cluster()
        self.session = self.cluster.connect('gpsinfo')

    def insert_alarm(self, alarm):
        send_tm = arrow.now().to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss')
        receive_tm = arrow.get(alarm.alarmtime).to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss')
        sql = '''insert into alarm(dev_id, course, lat, lng, receive_time, 
            send_time, speed, status, type_id) values({}, {}, {}, {}, '{}', 
            '{}', {}, '{}', {})'''.format(alarm.id, alarm.route, alarm.lat,
            alarm.lng, receive_tm, send_tm, alarm.speed, alarm.status.decode(), alarm.alarmtype)
        try:
            self.session.execute(sql)
            g_logger.debug(sql)
            return 0
        except Exception as e:
            g_logger.error(e)
            return -1