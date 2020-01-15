import MySQLdb
from db import BaseDb
from globals import g_logger, g_cfg

class BusinessDb(BaseDb):
    def __init__(self, dbcfg):
        super(BusinessDb, self).__init__(dbcfg)

    def get_entinfo_by_login_name(self, login_name):
        self.check()
        sql = '''select pwd,eid from t_enterprise where login_name = '{}' '''.format(login_name)
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.debug(cursor._executed)
            row = cursor.fetchone()
            return row[0], row[1]

    def load_all_ent(self):
        self.check()
        sql = '''select eid, pid, login_name from t_enterprise order by eid'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            ents = []
            rows = cursor.fetchall()
            for row in rows:
                ent = {
                    'eid' : row[0],
                    'pid' : row[1],
                    'login_name' : row[2]
                }
                ents.append(ent)

        return ents

    def load_all_device(self):
        self.check()
        sql = '''select dev_id, eid from t_device order by dev_id'''
        with self.conn.cursor() as cursor:
            cursor.execute(sql)
            g_logger.info(cursor._executed)
            devices = []
            rows = cursor.fetchall()
            for row in rows:
                device = {
                    'dev_id' : row[0],
                    'eid' : row[1],
                }
                devices.append(device)

        return devices


