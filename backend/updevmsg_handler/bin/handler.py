from globals import g_logger, g_cfg
from dev_pb2 import MsgType, UpDevMsg
from businessdb import BusinessDb

'''
message UpDevMsg
{
    MsgType msgtype = 1;
    oneof msgbody
    {
        GpsMsg gps =2;
        HbMsg hb = 3;
        AlarmMsg alarm = 4;
        CommandRsp cmdrsp = 5;
        IccidMsg iccid = 6;
        LbsMsg lbs = 7;
        WifiMsg wifi = 8;
    };
};
message CommandRsp
{
    uint64 id = 1;
    string imei = 2;
    sint32 seq = 3;
    uint64 rsptime=4;
    string rsp = 5;
};
message IccidMsg
{
    uint64 id = 1;
    string imei = 2;
    sint32 seq = 3;
    string iccid = 4;
};
message AlarmMsg
{
    uint64 id = 1;
    string imei = 2;
    uint64 alarmtime = 3;
    sint32 alarmtype = 4;
    sint32 lng = 5;
    sint32 lat = 6;
    sint32 speed = 7;
    sint32 route = 8;
    bytes status= 9;
};
'''

class Handler(object):
    def __init__(self):
        self.db_mysql = BusinessDb(g_cfg['db_business_w'])

    def proc_cmdresp(self, updev_msg):
        errcode, id = self.db_mysql.get_seqno_by_devid(updev_msg.cmdrsp.id)
        if errcode != 0:
            g_logger.error('get cmd_history id failed, dev_id:{}'.format(updev_msg.cmdrsp.id))
            return
        errcode = self.db_mysql.update_cmd_history(updev_msg, id)
        if errcode != 0:
            g_logger.error('update cmd_history failed, dev_id:{}'.format(updev_msg.cmdrsp.id))

    def proc_alarm(self, updev_msg):
        pass

    def proc_iccid(self, updev_msg):
        pass



