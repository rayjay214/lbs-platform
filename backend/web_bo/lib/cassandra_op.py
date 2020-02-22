from cassandra.cluster import Cluster
from globals import g_logger, g_cfg
import arrow

class CassandraOp():
    def __init__(self):
        self.cluster = Cluster()
        self.session = self.cluster.connect('gpsinfo')

    def getGpsInfoByTimeRange(self, dev_id, begin_tm, end_tm):
        send_tm = arrow.get(begin_tm).format('YYYY-MM-DD HH:mm:ss ZZ')
        end_tm = arrow.get(end_tm).format('YYYY-MM-DD HH:mm:ss ZZ')
        sql = '''select * from gps where dev_id={} and report_time>='{}' and report_time<'{}'
                 order by report_time asc limit 1000'''.format(dev_id, begin_tm, end_tm)
        try:
            gpsinfos = []
            rows = self.session.execute(sql)
            for row in rows:
                gpsinfo = {
                    'dev_id' : row[0],
                    'report_time' : arrow.get(row[1].timestamp),
                    'direction' : row[2],
                    'lat' : row[3],
                    'lon' : row[4],
                    'speed' : row[7],
                    'sys_time' : arrow.get(row[8].timestamp)
                }
                gpsinfos.append(gpsinfo)
            return gpsinfos
        except Exception as e:
            g_logger.error(e)
            return None