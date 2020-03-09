#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import arrow
from bottle import install, request, route, response
from constants import ErrMsg, ErrCode
from globals import g_cfg, g_logger
import traceback
from dev_pb2 import DownDevMsg, MsgType
import chardet
from ctree_op import CtreeOp
from redis_op import RedisOp
from businessdb import BusinessDb
from kafka_op import KafkaOp
from trans_coord import wgs84_to_bd09
from trans_coord import wgs84_to_gcj02
from cassandra_op import CassandraOp

@route('/device/importDevices')
def importDevices():
    errcode, data = ErrCode.ErrOK, {}
    target_eid = request.params.get('target_eid', None)
    product_type = request.params.get('product_type', None)
    dev_infos = request.params.get('device_infos', None)
    if None in (target_eid, product_type, dev_infos):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    dev_infos.replace(' ', '').replace('\r\n', ';').replace('\r', ';').replace('\n', ';')
    filtered_dev_infos = ';'.join([v for v in dev_infos.split(';') if len(v) > 0])
    rows = filtered_dev_infos.split(';')
    insert_rows = []
    for row in rows:
        # imei, dev_name, eid, product_type
        insert_data = (str(row), 'YJ-' + str(row)[-5:], int(target_eid), str(product_type))
        insert_rows.append(insert_data)
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.import_devices(insert_rows)
    return errcode, data

@route('/device/getDevInfoById')
def getDevInfoById():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    if dev_id is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    data = dev_info
    return errcode, data

@route('/device/searchByImei')
def searchDeviceByImei():
    errcode, data = ErrCode.ErrOK, {}
    imei = request.params.get('imei', None)
    if imei is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoByImei(imei)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    data = dev_info
    # get ancestor info
    ancestor, channel = ctree_op.getAncestorInfo(int(dev_info['eid']))
    ancestors = []
    for node in ancestor:
        info = {'eid': node.eid,
                'text': '''{}({}/{})'''.format(node.login_name, node.own_dev_num, node.total_dev_num),
                'addr': node.addr, 'phone': node.phone, 'email': node.email}
        ancestors.append(info)
    # add owner info
    owner = ctree_op.getCustomerInfoByEid(int(dev_info['eid']))
    info = {'eid': owner.eid,
            'text': '''{}({}/{})'''.format(owner.login_name, owner.own_dev_num, owner.total_dev_num),
            'addr': owner.addr, 'phone': owner.phone, 'email': owner.email}
    ancestors.append(info)
    data['ancestors'] = ancestors
    channel.close()
    return errcode, data

@route('/device/searchImeiByPattern')
def searchImeiByPattern():
    errcode, data = ErrCode.ErrOK, {}
    pattern = request.params.get('pattern', None)
    if pattern is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    imeis = redis_op.scanImeisByPattern(pattern)
    if imeis is None or len(imeis) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    dev_infos = redis_op.getDeviceInfoByimeis(imeis)
    authorized_imeis = []
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    for dev_info in dev_infos:
        is_ancestor = ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
        if is_ancestor or int(login_id) == int(dev_info['eid']):
            authorized_imeis.append(dev_info['imei'])
    data = authorized_imeis
    return errcode, data

@route('/device/getRunInfoByDevid')
def getRunInfoByDevid():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    if dev_id is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    run_info = redis_op.getDeviceRunInfoById(dev_id)
    if run_info is None or len(run_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    #translate coord
    map_type = request.params.get('map_type', None)
    if map_type == 'baidu':
        lon, lat = wgs84_to_bd09(float(run_info['longitude'])/1000000, float(run_info['latitude'])/1000000)
        run_info['longitude'] = lon
        run_info['latitude'] = lat
    elif map_type == 'amap':
        lon, lat = wgs84_to_gcj02(float(run_info['longitude']) / 1000000, float(run_info['latitude']) / 1000000)
        run_info['longitude'] = lon
        run_info['latitude'] = lat
    else:
        run_info['longitude'] = float(run_info['longitude']) / 1000000
        run_info['latitude'] = float(run_info['longitude']) / 1000000

    data = run_info
    #calc dev_status
    data['dev_status'] = 'online'
    maxtime = max(int(run_info['gps_time']), int(run_info['sys_time']))
    now = arrow.now().timestamp
    if now - maxtime > int(g_cfg['master']['offline_interval']):
        data['dev_status'] = 'offline'
        data['offline_time'] = now - maxtime
        return errcode, data
    static_time = now - int(run_info['sys_time'])
    if static_time > int(g_cfg['master']['static_interval']):
        data['dev_status'] = 'static'
        data['static_time'] = static_time
    return errcode, data

@route('/device/getBmsInfoByDevid')
def getBmsInfoByDevid():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    if dev_id is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    bms_info = redis_op.getBmsInfoById(dev_id)
    if bms_info is None or len(bms_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    dev_info = redis_op.getDeviceInfoById(dev_id)
    bms_info['first_online_time'] = dev_info['CREATE_TIME']
    bms_info['imei'] = dev_info['imei']
    bms_info['production_date'] = arrow.get(dev_info['production_date']).format('YYYY-MM-DD')
    bms_info['product_type'] = dev_info['product_type']
    bms_info['manufacturer'] = dev_info['manufacturer']
    data = bms_info
    return errcode, data

@route('/device/getCmdListByType')
def getCmdListByType():
    errcode, data = ErrCode.ErrOK, {}
    product_type = request.params.get('product_type', None)
    if product_type is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_r = BusinessDb(g_cfg['db_business_r'])
    errcode, cmd_list = db_r.get_cmdlist_by_type(product_type)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    data = cmd_list
    return errcode, data

def get_protocol_by_type(product_type):
    for key in g_cfg['protocol']:
        types = g_cfg['protocol'][key].split(',')
        if product_type in types:
            return key
    return None

@route('/device/sendCmd')
def sendCmd():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    cmd_id = request.params.get('cmd_id', None)
    cmd_name = request.params.cmd_name  #has to be this way, refer to https://segmentfault.com/q/1010000008386616
    cmd_content = request.params.get('cmd_content', None)
    if None in (dev_id, cmd_id, cmd_content):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    if cmd_name is None or cmd_name == '':
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    #insert t_cmd_history
    cmd_info = {'dev_id':dev_id, 'cmd_id':cmd_id, 'cmd_name':cmd_name,
            'cmd_content':cmd_content, 'eid':login_id}
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode, id = db_w.insert_cmd_history(cmd_info)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    data['id'] = id
    #write to kafka for cmd_handler module
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    protocol = get_protocol_by_type(dev_info['product_type'])
    if protocol is None:
        errcode = ErrCode.ErrTypeNotSupported
        return errcode, data
    down_devmsg = DownDevMsg()
    down_devmsg.msgtype = MsgType.kCommandReq
    down_devmsg.cmdreq.id = int(dev_id)
    down_devmsg.cmdreq.imei = dev_info['imei']
    down_devmsg.cmdreq.seq = id
    down_devmsg.cmdreq.reqtime = arrow.now().timestamp
    down_devmsg.cmdreq.content = cmd_content
    down_devmsg.cmdreq.protocol = protocol
    str = down_devmsg.SerializeToString()
    kafka_op = KafkaOp(g_cfg['kafka'])
    kafka_op.produce_cmd(str)
    return errcode, data

@route('/device/sendBmsRelayCmd')
def sendBmsRelayCmd():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    cmd_id = 100
    cmd_name = 'BMS断油电'  #has to be this way, refer to https://segmentfault.com/q/1010000008386616
    cmd_content = request.params.get('cmd_content', None)
    if None in (dev_id, cmd_id, cmd_content):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    #insert t_cmd_history
    cmd_info = {'dev_id':dev_id, 'cmd_id':cmd_id, 'cmd_name':cmd_name,
            'cmd_content':cmd_content, 'eid':login_id}
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode, id = db_w.insert_cmd_history(cmd_info)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    data['id'] = id
    redis_op = RedisOp(g_cfg['redis'])
    #REALY,0, only modify status in redis, no need to send cmd
    flag = cmd_content[-2:-1]
    if flag not in ('0', '1'):
        errcode = ErrCode.ErrParamInvalid
        return errcode, data
    redis_op.setBmsRelayStatus(dev_id, flag)
    #write to kafka for cmd_handler module
    dev_info = redis_op.getDeviceInfoById(dev_id)
    protocol = get_protocol_by_type(dev_info['product_type'])
    if protocol is None:
        errcode = ErrCode.ErrTypeNotSupported
        return errcode, data
    down_devmsg = DownDevMsg()
    down_devmsg.msgtype = MsgType.kCommandReq
    down_devmsg.cmdreq.id = int(dev_id)
    down_devmsg.cmdreq.imei = dev_info['imei']
    down_devmsg.cmdreq.seq = id
    down_devmsg.cmdreq.reqtime = arrow.now().timestamp
    down_devmsg.cmdreq.content = cmd_content
    down_devmsg.cmdreq.protocol = protocol
    str = down_devmsg.SerializeToString()
    kafka_op = KafkaOp(g_cfg['kafka'])
    kafka_op.produce_cmd(str)
    return errcode, data

@route('/device/getCmdRsp')
def getCmdRsp():
    errcode, data = ErrCode.ErrOK, {}
    id = request.params.get('id', None)
    if id is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_r = BusinessDb(g_cfg['db_business_r'])
    errcode, response = db_r.get_cmd_rsp_by_id(id)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    if response is None or response == '':
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    data['response'] = response
    return errcode, data

@route('/device/updateDevice')
def updateDevice():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    product_type = request.params.get('product_type', None)
    dev_name = request.params.dev_name if len(request.params.dev_name) != 0 else None
    remark = request.params.remark if len(request.params.remark) != 0 else None
    if dev_id is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    dev_info['product_type'] = product_type if product_type is not None else dev_info['product_type']
    dev_info['dev_name'] = dev_name if dev_name is not None else dev_info['dev_name']
    dev_info['remark'] = remark if remark is not None else dev_info['remark']
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.update_device(dev_info)
    return errcode, data

@route('/device/getLocationInfo')
def getLocationInfo():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    begin_tm = request.params.get('begin_tm', None)
    end_tm = request.params.get('end_tm', None)
    map_type = request.params.get('map_type', None)
    limit = request.params.get('limit', 1000)
    if None in (dev_id, begin_tm, end_tm):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    cassandra_op = CassandraOp()
    gpsinfos = cassandra_op.getGpsInfoByTimeRange(dev_id, begin_tm, end_tm, limit)
    if gpsinfos is None:
        errcode = ErrCode.ErrMysqlError
        return errcode, data
    #adjust coord
    for info in gpsinfos:
        if map_type == 'baidu':
            lng, lat = wgs84_to_bd09(info['lng'], info['lat'])
            info['lng'] = lng
            info['lat'] = lat
        elif map_type == 'amap':
            lng, lat = wgs84_to_gcj02(info['lng'], info['lat'])
            info['lng'] = lng
            info['lat'] = lat
        else:
            continue
    if len(gpsinfos) < int(limit):
        data['resEndTime'] = end_tm
    else:
        data['resEndTime'] = gpsinfos[-1]['report_time'] + 1

    data['infos'] = gpsinfos
    return ErrCode.ErrOK, data

@route('/device/saveFenceInfo')
def saveFenceInfo():
    errcode, data = ErrCode.ErrOK, {}
    fence = {}
    fence['dev_id'] = request.params.get('dev_id', None)
    fence['validate_flag'] = request.params.get('validate_flag', None)
    fence['shape_type'] = request.params.get('shape_type', None)
    fence['shape_param'] = request.params.get('shape_param', None)
    fence['alarm_type'] = '2'
    if None in (fence['dev_id'], fence['validate_flag'], fence['shape_type'], fence['shape_param']):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.update_fence(fence)
    return errcode, data

@route('/device/getFenceInfo')
def getFenceInfo():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    if dev_id is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    fence = redis_op.getFenceInfoByDevId(dev_id)
    if fence is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    data['fence'] = fence
    return errcode, data

@route('/device/getAlarmByDevId')
def getAlarmByDevId():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    begin_tm = request.params.get('begin_tm', None)
    end_tm = request.params.get('end_tm', None)
    map_type = request.params.get('map_type', None)
    read_flag = request.params.get('read_flag', -1)
    if None in (dev_id, begin_tm, end_tm):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    cassandra_op = CassandraOp()
    dev_ids = []
    dev_ids.append(dev_id)
    alarminfos = cassandra_op.getAlarmByTimeRange(dev_ids, begin_tm, end_tm, read_flag)
    if alarminfos is None:
        errcode = ErrCode.ErrMysqlError
        return errcode, data
    data['alarm_infos'] = alarminfos
    return errcode, data

@route('/device/getMilestatByDevId')
def getMilestatByDevId():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    begin_tm = request.params.get('begin_tm', None)
    end_tm = request.params.get('end_tm', None)
    if None in (dev_id, begin_tm, end_tm):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    cassandra_op = CassandraOp()
    dev_ids = []
    dev_ids.append(dev_id)
    milestat_infos = cassandra_op.getMilestatByTimeRange(dev_ids, begin_tm, end_tm)
    if milestat_infos is None:
        errcode = ErrCode.ErrMysqlError
        return errcode, data
    data['milestat_infos'] = milestat_infos
    return errcode, data
