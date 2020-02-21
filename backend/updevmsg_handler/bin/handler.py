from globals import g_logger, g_cfg
from dev_pb2 import MsgType, UpDevMsg
from businessdb import BusinessDb
from redis_op import RedisOp
from cassandra_op import CassandraOp

class Handler(object):
    def __init__(self):
        #self.db_mysql = BusinessDb(g_cfg['db_business_w'])
        pass

    def proc_cmdresp(self, updev_msg):
        db_w = BusinessDb(g_cfg['db_business_w'])
        errcode, id = db_w.get_seqno_by_devid(updev_msg.cmdrsp.id)
        if errcode != 0:
            g_logger.error('get cmd_history id failed, dev_id:{}'.format(updev_msg.cmdrsp.id))
            return
        errcode = db_w.update_cmd_history(updev_msg, id)
        if errcode != 0:
            g_logger.error('update cmd_history failed, dev_id:{}'.format(updev_msg.cmdrsp.id))

    def proc_alarm(self, updev_msg):
        cassandra_op = CassandraOp()
        errcode = cassandra_op.insert_alarm(updev_msg.alarm)
        if errcode != 0:
            g_logger.error('{} insert alarm failed'.format(updev_msg.alarm.id))

    def proc_iccid(self, updev_msg):
        db_w = BusinessDb(g_cfg['db_business_w'])
        redis_op = RedisOp(g_cfg['redis'])
        dev_info = redis_op.getDeviceInfoById(updev_msg.iccid.id)
        if dev_info is None:
            g_logger.error('dev into not found, dev_id:{}'.format(updev_msg.iccid.id))
        if dev_info['iccid'] == updev_msg.iccid.iccid:
            g_logger.debug('iccid:{} is same'.format(dev_info['iccid']))
            return
        elif dev_info['iccid'] == '':
            #update t_device
            #insert t_card
            errcode = db_w.update_device_iccid(updev_msg)
            if errcode != 0:
                g_logger.error('update iccid failed')
        elif dev_info['iccid'] != updev_msg.iccid.iccid:   #change card
            #update t_device
            #insert t_card
            errcode = db_w.update_device_iccid(updev_msg)
            if errcode != 0:
                g_logger.error('update iccid failed')
            #other




