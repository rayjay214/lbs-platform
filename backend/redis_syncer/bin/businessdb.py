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
