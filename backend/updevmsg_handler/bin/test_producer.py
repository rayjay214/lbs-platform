from confluent_kafka import Producer
from dev_pb2 import MsgType, UpDevMsg

p = Producer({'bootstrap.servers': '127.0.0.1:9092'})

updev_msg = UpDevMsg()
updev_msg.msgtype = MsgType.kCommandRsp
updev_msg.cmdrsp.id=44
updev_msg.cmdrsp.rsptime=1581056109
updev_msg.cmdrsp.rsp='ok'
data = updev_msg.SerializeToString()

p.produce('gw.cmdrsp', data)
p.flush()
