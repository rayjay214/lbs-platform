from cassandra.cluster import Cluster
from globals import g_logger, g_cfg
import arrow
import traceback

class CassandraOp():
    def __init__(self):
        self.cluster = Cluster()
        self.session = self.cluster.connect('gpsinfo')

    def getGpsInfoByTimeRange(self, dev_id, begin_ts, end_ts, limit):
        begin_tm = arrow.get(int(begin_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        end_tm = arrow.get(int(end_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        sql = '''select * from gps where dev_id={} and report_time>='{}' and report_time<'{}'
                 order by report_time asc limit {}'''.format(dev_id, begin_tm, end_tm, limit)
        try:
            g_logger.info(sql)
            gpsinfos = []
            rows = self.session.execute(sql)
            for row in rows:
                gpsinfo = {
                    'dev_id' : row[0],
                    'report_time' : arrow.get(row[1]).timestamp,
                    'direction' : row[2],
                    'lat' : float(row[3])/1000000,
                    'lng' : float(row[4])/1000000,
                    'speed' : row[7],
                    'sys_time' : arrow.get(row[8]).timestamp
                }
                gpsinfos.append(gpsinfo)
            return gpsinfos
        except Exception as e:
            g_logger.error(traceback.format_exc())
            g_logger.error(e)
            return None

    def getAlarmByTimeRange(self, dev_ids, begin_ts, end_ts, read_flag=-1):
        begin_tm = arrow.get(int(begin_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        end_tm = arrow.get(int(end_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        str_dev_ids = ','.join([str(i) for i in dev_ids])
        if read_flag == 0:
            sql = '''select dev_id,receive_time,course,lat,lng,send_time,speed,status,type_id from alarm
                    where dev_id in ({}) and receive_time>='{}' and receive_time<'{}'
                    and read_flag=0 allow filtering
                    '''.format(str_dev_ids, begin_tm, end_tm)
        else:
            sql = '''select dev_id,receive_time,course,lat,lng,send_time,speed,status,type_id from alarm
                    where dev_id in ({}) and receive_time>='{}' and receive_time<'{}'
                    '''.format(str_dev_ids, begin_tm, end_tm)
        try:
            g_logger.info(sql)
            alarm_infos = []
            rows = self.session.execute(sql)
            for row in rows:
                alarm = {
                    'dev_id' : row[0],
                    'receive_time' : arrow.get(row[1]).to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss'),
                    'course' : row[2],
                    'lat' : float(row[3])/1000000,
                    'lng' : float(row[4])/1000000,
                    'send_time' : arrow.get(row[5]).to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss'),
                    'speed' : row[6],
                    'status' : row[7],
                    'type_id' : row[8]
                }
                alarm_infos.append(alarm)
            return alarm_infos
        except Exception as e:
            g_logger.error(traceback.format_exc())
            g_logger.error(e)
            return None

    def getMilestatByTimeRange(self, dev_ids, begin_ts, end_ts):
        begin_tm = arrow.get(int(begin_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        end_tm = arrow.get(int(end_ts)).format('YYYY-MM-DD HH:mm:ss ZZ')
        str_dev_ids = ','.join([str(i) for i in dev_ids])
        sql = '''select dev_id,stat_date,mileage from milestat
                where dev_id in ({}) and stat_date>='{}' and stat_date<'{}'
                '''.format(str_dev_ids, begin_tm, end_tm)
        try:
            g_logger.info(sql)
            milestat_infos = []
            rows = self.session.execute(sql)
            for row in rows:
                info = {
                    'dev_id' : row[0],
                    'stat_date' : arrow.get(row[1]).to('Asia/Shanghai').format('YYYY-MM-DD HH:mm:ss'),
                    'mileage' : row[2]
                }
                milestat_infos.append(info)
            return milestat_infos
        except Exception as e:
            g_logger.error(traceback.format_exc())
            g_logger.error(e)
            return None
