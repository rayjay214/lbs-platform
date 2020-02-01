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

    def get_cmdlist_by_type(self, product_type):
        self.check()
        sql = ''' SELECT t2.* FROM t_type_cmd t1
                INNER JOIN t_cmd_format t2 ON t1.cmd_id = t2.cmd_id
                WHERE t1.product_type = '{}';
        '''.format(product_type)
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.info(cursor._executed)
                rows = cursor.fetchall()
                cmds = []
                for row in rows:
                    cmd = {
                        'cmd_id': row[0],
                        'name': row[1],
                        'tw_name': row[2],
                        'en_name': row[3],
                        'pri': row[4],
                        'head': row[5],
                        'tail': row[6],
                        'sp': row[7],
                        'check': row[8],
                        'group': row[9],
                        'remark': row[10],
                        'param': row[11]
                    }
                    cmds.append(cmd)
                return ErrCode.ErrOK, cmds
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError, None

    def insert_cmd_history(self, cmd_info: dict):
        self.check()
        sql = '''insert into t_cmd_history(dev_id, eid, cmd_name, cmd_content, cmd_id)
            values ({}, {}, '{}', '{}', {})
        '''.format(cmd_info['dev_id'], cmd_info['eid'], cmd_info['cmd_name'], cmd_info['cmd_content'], cmd_info['cmd_id'])
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.debug(cursor._executed)
                self.conn.commit()
                id = cursor.lastrowid
                return ErrCode.ErrOK, id
        except Exception as e:
            g_logger.error(e)
            return ErrCode.ErrMysqlError, 0