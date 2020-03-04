from cassandra.cluster import Cluster
from geopy.distance import geodesic
from globals import g_logger, g_cfg

import arrow

class MileStat(object):
    def __init__(self, dev_id, gpsinfo):
        self.dev_id = dev_id
        self.gpsinfo = gpsinfo
        self.datetime = ''
        self.mileage = 0

    def calc_distance(self, base, gps):
        sp = (base.latitude / 1000000, base.longitude / 1000000)
        ep = (gps.latitude / 1000000, gps.longitude / 1000000)
        distance = geodesic(sp, ep)
        return int(distance.m)

    def proc_milestat(self):
        base = None
        for i, gps in enumerate(self.gpsinfo, 1):
            if gps.longitude == 0 and gps.latitude == 0:
                continue

            if gps.speed == -1:
                continue
                
            if not base:
                base = gps
                self.datetime = base.report_time.date().isoformat()
                
            if gps.longitude == base.longitude and gps.latitude == base.latitude:
                continue;
                
            diff_time = (gps.report_time - base.report_time).seconds
            if diff_time < 0:
                continue

            dis = self.calc_distance(base, gps)
            self.mileage =  self.mileage + dis

            str_bt = base.report_time.strftime('%Y-%m-%d %H:%M:%S')
            str_gt = gps.report_time.strftime('%Y-%m-%d %H:%M:%S')

            debug_str = '''calc info. base:report_time={},lng={},lat={} 
            gps:report_time={},lng={},lat={}, dis={}, mileage={}'''.format(str_bt, base.longitude,base.latitude,str_gt, gps.longitude,gps.latitude, dis, self.mileage)
            g_logger.debug(debug_str)

            base = gps
        debug_str = '''mile calc result. dev_id={}, datetime={}, mileage={}, gpsinfo size={}'''.format(self.dev_id, self.datetime, self.mileage, len(self.gpsinfo))
        g_logger.info(debug_str)
        return 0


class CassandraOp():
    def __init__(self):
        self.cluster = Cluster()
        self.session = self.cluster.connect('gpsinfo')

    def __del__(self):
        self.cluster.shutdown()

    def select_gps_dev_count(self, rows):
        today = arrow.now().format('YYYY-MM-DD')
        yesterday = arrow.now().shift(days=-1).format('YYYY-MM-DD')
        sql = '''select dev_id, count(*) from gps where report_time > '{}' and report_time < '{}' group by dev_id ALLOW FILTERING'''   
        sql = sql.format(yesterday, today)
        g_logger.debug('select_gps_dev_count: sql=' + sql)
        
        try:
            res = self.session.execute(sql)
            for row in res:
                rows.append(row)
            #return res
        except Exception as e:
            g_logger.error(e)
            return -1
        return 0

    def select_gps_info(self, dev_id, rows):
        today = arrow.now().format('YYYY-MM-DD')
        yesterday = arrow.now().shift(days=-1).format('YYYY-MM-DD')
        sql = '''select * from gps where dev_id={} and report_time > '{}' and report_time < '{}' order by report_time asc;'''
        sql = sql.format(dev_id, yesterday, today)
        g_logger.debug('select_gps_info: sql=' + sql)
        
        try:
            res = self.session.execute(sql)
            for row in res:
                rows.append(row)
            #return res
        except Exception as e:
            g_logger.error(e)
            return -1
        return 0

    def insert_milestat(self, milestat):
        #stat_date = arrow.get(milestat.datetime).to('local').format('YYYY-MM-DD')
        if milestat.datetime is None or milestat.mileage <= 0:
            g_logger.warn('invalid milestat: dev_id={}, datetime={}, mileage={}'.format(milestat.dev_id, milestat.datetime, milestat.mileage))
            return 0
            
        sql = '''insert into milestat(dev_id, stat_date, mileage) values({}, '{}', {})'''.format(milestat.dev_id, milestat.datetime, milestat.mileage)
        g_logger.debug('insert_milestat: sql=' + sql)
        try:
            self.session.execute(sql)
            g_logger.debug(sql)
            return 0
        except Exception as e:
            g_logger.error(e)
            return -1
            
