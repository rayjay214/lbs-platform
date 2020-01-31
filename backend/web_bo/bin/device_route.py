#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import arrow
from bottle import install, request, route, response
from constants import ErrMsg, ErrCode
from globals import g_cfg, g_logger
from external_op import g_ctree_op, g_redis_op, g_db_r, g_db_w
import traceback


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
    errcode = g_db_w.imoort_devices(insert_rows)
    return errcode, data


@route('/device/searchByImei')
def searchDeviceByImei():
    errcode, data = ErrCode.ErrOK, {}
    imei = request.params.get('imei', None)
    if imei is None:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    dev_info = g_redis_op.getDeviceInfoByImei(imei)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    is_ancestor = g_ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    data = dev_info
    # get ancestor info
    ancestor, channel = g_ctree_op.getAncestorInfo(int(dev_info['eid']))
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
    dev_info = g_redis_op.getDeviceInfoById(dev_id)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    is_ancestor = g_ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    run_info = g_redis_op.getDeviceRunInfoById(dev_id)
    if run_info is None or len(run_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    data = run_info
    return errcode, data

@route('/device/getBmsInfoByDevid')
def getBmsInfoByDevid():
    errcode, data = ErrCode.ErrOK, {}
    dev_id = request.params.get('dev_id', None)
    if dev_id is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    dev_info = g_redis_op.getDeviceInfoById(dev_id)
    if dev_info is None or len(dev_info) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    is_ancestor = g_ctree_op.isAncestor(int(login_id), int(dev_info['eid']))
    if not is_ancestor and int(login_id) != int(dev_info['eid']):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    bms_info = g_redis_op.getBmsInfoById(dev_id)
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
    errcode, cmd_list = g_db_r.get_cmdlist_by_type(product_type)
    if errcode != ErrCode.ErrOK:
        return errcode, data
    data = cmd_list
    return errcode, data

