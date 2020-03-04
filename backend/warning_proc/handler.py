from globals import g_logger, g_cfg
from dev_pb2 import MsgType, UpDevMsg
from redis_op import RedisOp
from geopy.distance import geodesic
from shapely.geometry import Point
from shapely.geometry.polygon import Polygon

def isInCircleFence(lat,lng,fence_info):
    sp = (lat / 1000000, lng / 1000000)

    fence_param = list(map(float,fence_info["SHAPE_PARAM"].split(',')))
    ep = (fence_param[0], fence_param[1])
    distance = geodesic(sp, ep)
    g_logger.debug('circle fence.lat:{},lng:{},radus:{},distance:{}'.format(lat,lng,fence_param[2],distance.m))
    if distance.m <= fence_param[2]:
        return True
    return False
    
def isInPolygonFence(lat,lng,fence_info):
    point = Point(lng / 1000000, lat / 1000000)
    
    fence_param = []
    for point_str in fence_info["SHAPE_PARAM"].split(';'):
        point_cor = list(map(float,point_str.split(',')))
        fence_param.append(point_cor)

    polygon = Polygon(fence_param)
    return polygon.contains(point)
    
class Handler(object):
    def __init__(self):
        #self.db_mysql = BusinessDb(g_cfg['db_business_w'])
        pass

    def proc_gps(self, updev_msg):
        redis_op = RedisOp(g_cfg['redis'])
        fence_info = redis_op.getFenceInfoById(updev_msg.gps.id)
        if fence_info is None:
            g_logger.error('fence info not found, dev_id:{}'.format(updev_msg.gps.id))
            return 0

        fence_id = int(fence_info.get("fence_id",'-1'))
        if fence_id == -1:
            g_logger.error('fence info not exist, dev_id:{},fence_id:{}'.format(updev_msg.gps.id,fence_id))
            return 0
            
        validate_flag = int(fence_info.get('VALIDATE_FLAG','0'))
        if validate_flag != 1:
            g_logger.debug('fence is invalid. dev_id:{},fence_id:{},validate_flag:{}'.format(updev_msg.gps.id,fence_id,validate_flag))
            return 0

        shape_type = int(fence_info.get('SHAPE_TYPE','-1'))
        shape_param = fence_info.get('SHAPE_PARAM','')
        if shape_type == -1 or not shape_param:
            g_logger.debug('fence param error. dev_id:{},fence_id:{},shape_type:{},shape_param:{}'.format(updev_msg.gps.id,fence_id,shape_type,shape_param))
            return 0

        status = int(fence_info.get('status', '0'))
        cur_status = 0
        if shape_type == 1:
            if isInCircleFence(updev_msg.gps.lat,updev_msg.gps.lng,fence_info):
                cur_status = 1
            g_logger.debug('fence is circle. dev_id:{},fence_id:{},status:{},cur_status:{}'.format(updev_msg.gps.id,fence_id,status,cur_status))
        elif shape_type == 2:
            if isInPolygonFence(updev_msg.gps.lat,updev_msg.gps.lng,fence_info):
                cur_status = 1
            g_logger.debug('fence is polygon. dev_id:{},fence_id:{},status:{},cur_status:{}'.format(updev_msg.gps.id,fence_id,status,cur_status))
        else:
            cur_status = status

        alarm_type = 0
        if status != cur_status:
            redis_op.setFenceStatusById(updev_msg.gps.id, cur_status)

            if cur_status == 0:
                alarm_type = 8
            else:
                alarm_type = 24
        return alarm_type




