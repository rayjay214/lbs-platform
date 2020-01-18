import MySQLdb
from db import BaseDb
from globals import g_logger, g_cfg

class BusinessDb(BaseDb):
    def __init__(self, dbcfg):
        super(BusinessDb, self).__init__(dbcfg)

    def load_all_ent(self):
        self.check()
        sql = '''select eid,pid,login_name,phone,addr,email from t_enterprise order by eid'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            max_fetch = 100
            while True:
                rows = cursor.fetchmany(size=max_fetch)
                if rows is None:
                    break
                for row in rows:
                    ent = {
                        'eid' : row[0],
                        'pid' : row[1],
                        'login_name' : row[2],
                        'phone' : row[3],
                        'addr' : row[4],
                        'email' : row[5]
                    }
                    yield ent
                if len(rows) < max_fetch:
                    break


    def load_all_device(self):
        self.check()
        sql = '''select dev_id, eid from t_device order by dev_id'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            max_fetch = 100
            while True:
                rows = cursor.fetchmany(size=max_fetch)
                if rows is None:
                    break
                for row in rows:
                    device = {
                        'dev_id' : row[0],
                        'eid' : row[1],
                    }
                    yield device
                if len(rows) < max_fetch:
                    break



