import MySQLdb
from db import BaseDb
from globals import g_logger, g_cfg

class BusinessDb(BaseDb):
    def __init__(self, dbcfg):
        super(BusinessDb, self).__init__(dbcfg)

    def load_all_device(self):
        self.check()
        sql = '''select dev_id,imei,iccid,dev_name,eid,plateno,product_type,remark from t_device order by dev_id'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            while True:
                rows = cursor.fetchmany(size=100)
                if rows is None:
                    break
                for row in rows:
                    device = {
                        'dev_id' : row[0],
                        'imei' : row[1],
                        'iccid' : row[2],
                        'dev_name' : row[3],
                        'eid' : row[4],
                        'plateno' : row[5],
                        'product_type' : row[6],
                        'remark' : row[7]
                    }
                    yield device
                if len(rows) < 100:
                    break

    def load_all_cards(self):
        self.check()
        sql = '''select iccid,msisdn,package,manufacturer,create_time,plat_expire_time from t_card order by iccid'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            while True:
                rows = cursor.fetchmany(size=100)
                if rows is None:
                    break
                for row in rows:
                    card = {
                        'iccid' : row[0],
                        'msisdn' : row[1],
                        'package' : row[2],
                        'manufacturer' : row[3],
                        'create_time' : row[4],
                        'plat_expire_time' : row[5],
                    }
                    yield card
                if len(rows) < 100:
                    break
