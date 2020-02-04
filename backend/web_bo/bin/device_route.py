#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import arrow
from bottle import install, request, route, response
from constants import ErrMsg, ErrCode
from globals import g_cfg, g_logger
import traceback
import dev_pb2
import chardet
from ctree_op import CtreeOp
from redis_op import RedisOp
from businessdb import BusinessDb
from kafka_op import KafkaOp

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
    errcode = db_w.imoort_devices(insert_rows)
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
    data['ancestors'] = ancestors
    channel.close()
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
    if static_time > run_info['static_interval']:
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
    db_w = BusinessDb(g_cfg['db_business_r'])
    errcode, id = db_w.insert_cmd_history(cmd_info)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    data['id'] = id
    #write to kafka for cmd_handler module
    redis_op = RedisOp(g_cfg['redis'])
    dev_info = redis_op.getDeviceInfoById(dev_id)
    down_devmsg = dev_pb2.DownDevMsg()
    down_devmsg.msgtype = dev_pb2.MsgType.kCommandReq
    down_devmsg.cmdreq.id = int(dev_id)
    down_devmsg.cmdreq.imei = dev_info['imei']
    down_devmsg.cmdreq.seq = id
    down_devmsg.cmdreq.reqtime = arrow.now().timestamp
    down_devmsg.cmdreq.content = cmd_content
    str = down_devmsg.SerializeToString()
    kafka_op = KafkaOp(g_cfg['kafka'])
    kafka_op.produce_cmd(str)
    return errcode, data
