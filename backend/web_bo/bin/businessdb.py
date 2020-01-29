import MySQLdb
from db import BaseDb
from globals import g_logger, g_cfg
from constants import ErrCode

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

    def get_ent_by_eid(self, eid) -> tuple:
        self.check()
        sql = '''select * from t_enterprise where eid={}'''.format(eid)
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.debug(cursor._executed)
                row = cursor.fetchone()
                ent = {
                    'eid' : row[0],
                    'pid' : row[1],
                    'phone' : row[2],
                    'login_name' : row[3],
                    'pwd' : row[4],
                    'addr' : row[5],
                    'email' : row[6]
                }
                return ErrCode.ErrOK, ent
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError, None

    def add_ent(self, ent: dict):
        self.check()
        sql = '''insert into t_enterprise(pid, phone, login_name, pwd, addr, email)
            values ({}, '{}', '{}', '{}', '{}', '{}')
        '''.format(ent['pid'], ent['phone'], ent['login_name'], ent['pwd'], ent['addr'], ent['email'])
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.debug(cursor._executed)
                self.conn.commit()
                return ErrCode.ErrOK
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError

    def delete_ent(self, ent: dict):
        self.check()
        sql = '''delete from t_enterprise where eid={}'''.format(ent['eid'])
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.debug(cursor._executed)
                self.conn.commit()
                return ErrCode.ErrOK
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError

    def update_ent(self, ent: dict):
        self.check()
        sql = '''update t_enterprise set pid={}, phone='{}', addr='{}', email='{}' where eid={};
            '''.format(ent['pid'], ent['phone'], ent['addr'], ent['email'], ent['eid'])
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.info(cursor._executed)
                self.conn.commit()
                return ErrCode.ErrOK
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError

    def imoort_devices(self, rows: list):
        self.check()
        sql = 'insert into t_device (imei, dev_name, eid, product_type) values (%s, %s, %s, %s)'
        try:
            with self.conn.cursor() as cursor:
                cursor.executemany(sql, rows)
                g_logger.info(cursor._executed)
                self.conn.commit()
                return ErrCode.ErrOK
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError
