from confluent_kafka import Producer
from dev_pb2 import MsgType, UpDevMsg

p = Producer({'bootstrap.servers': '127.0.0.1:9092'})

updev_msg = UpDevMsg()
'''
updev_msg.msgtype = MsgType.kCommandRsp
updev_msg.cmdrsp.id=44
updev_msg.cmdrsp.rsptime=1581056109
updev_msg.cmdrsp.rsp='ok'
'''
updev_msg.msgtype = MsgType.kAlarmMsg
updev_msg.alarm.id=23
updev_msg.alarm.alarmtime=1581326788
updev_msg.alarm.alarmtype=1
updev_msg.alarm.lat=23901713
updev_msg.alarm.lng=112777341
updev_msg.alarm.route=20
updev_msg.alarm.speed=10
updev_msg.alarm.status='0'.encode()

data = updev_msg.SerializeToString()

#p.produce('gw.cmdrsp', data)
p.produce('gw.alarm', data)
p.flush()
