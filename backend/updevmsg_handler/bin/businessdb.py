import MySQLdb
from db import BaseDb
from globals import g_logger, g_cfg
import arrow

class BusinessDb(BaseDb):
    def __init__(self, dbcfg):
        super(BusinessDb, self).__init__(dbcfg)

    def get_seqno_by_devid(self, dev_id):
        self.check()
        sql = '''select id from t_cmd_history where dev_id={} and status='0' order by create_time desc'''.format(dev_id)
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                g_logger.info(cursor._executed)
                row = cursor.fetchone()
                return 0, row[0]
        except Exception as e:
            g_logger.error(e)
            return -1,

    def update_cmd_history(self, updev_msg, id):
        self.check()
        resp_time = arrow.get(updev_msg.cmdrsp.rsptime).format('YYYY-MM-DD HH:mm:ss')
        sql = ''' update t_cmd_history set status='1', terminal_response='{}', response_time='{}'
                  where id={}
        '''.format(updev_msg.cmdrsp.rsp, resp_time, id)
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql)
                self.conn.commit()
                g_logger.info(cursor._executed)
                return 0
        except Exception as e:
            g_logger.error(e)
            return -1

    def update_device_iccid(self, updev_msg):
        self.check()
        tm = arrow.now().format('YYYY-MM-DD HH:mm:ss')
        sql_update_device = ''' update t_device set iccid='{}' where dev_id={} 
        '''.format(updev_msg.iccid.iccid, updev_msg.iccid.id)
        sql_insert_card = ''' insert into t_card(iccid, plat_start_time, create_time) 
        values ('{}', '{}', '{}') '''.format(updev_msg.iccid.iccid, tm, tm)
        try:
            with self.conn.cursor() as cursor:
                cursor.execute(sql_update_device)
                cursor.execute(sql_insert_card)
                self.conn.commit()
                g_logger.info(cursor._executed)
                return 0
        except Exception as e:
            g_logger.error(e)
            self.conn.rollback()
            return -1

