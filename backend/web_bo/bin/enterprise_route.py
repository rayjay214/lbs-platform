#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import arrow
from bottle import install, request, route, response
from constants import ErrMsg, ErrCode
from globals import g_cfg, g_logger
from businessdb import BusinessDb
import traceback
from customer_tree_pb2 import CustomerInfo
import itertools
from ctree_op import CtreeOp
from redis_op import RedisOp
from businessdb import BusinessDb
from kafka_op import KafkaOp
import xlrd
import datetime
from trans_coord import wgs84_to_bd09
from trans_coord import wgs84_to_gcj02
from cassandra_op import CassandraOp

ctree_op = CtreeOp(g_cfg['ctree'])
redis_op = RedisOp(g_cfg['redis'])
db_r = BusinessDb(g_cfg['db_business_r'])
db_w = BusinessDb(g_cfg['db_business_w'])
kafka_op = KafkaOp(g_cfg['kafka'])

@route('/ent/getEntInfoByEid')
def getEntInfoByEid():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    if eid is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    ctree_op = CtreeOp(g_cfg['ctree'])
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    if customer.eid == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    data['eid'] = customer.eid
    data['pid'] = customer.pid
    data['login_name'] = customer.login_name
    data['phone'] = customer.phone
    data['addr'] = customer.addr
    data['email'] = customer.email
    data['leaf'] = customer.is_leaf
    data['permission'] = customer.permission
    data['logo_url'] = customer.logo_url
    #deal with permission
    data['bms_permission'] = customer.permission[0]
    data['sensor_permission'] = customer.permission[1]
    data['gps_permission'] = customer.permission[2]
    return errcode, data

@route('/ent/getEntChildrenByEid')
def getEntChildrenByEid():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    if eid is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    records = []
    ctree_op = CtreeOp(g_cfg['ctree'])
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    data['eid'] = customer.eid
    data['pid'] = customer.pid
    data['text'] = '''{}({}/{})'''.format(customer.login_name, customer.own_dev_num, customer.total_dev_num)
    data['phone'] = customer.phone
    data['addr'] = customer.addr
    data['email'] = customer.email
    data['leaf'] = customer.is_leaf
    data['permission'] = customer.permission
    children, channel = ctree_op.getChildrenInfoByEid(int(eid))  #caller have to close channel manually
    for child in children:
        info = {'eid': child.eid,
                'text': '''{}({}/{})'''.format(child.login_name, child.own_dev_num, child.total_dev_num),
                'addr': child.addr, 'phone': child.phone, 'email': child.email, 'leaf': child.is_leaf}
        records.append(info)

    channel.close()
    data['records'] = records
    return errcode, data

@route('/ent/addEnt')
def addEnt():
    errcode, data = ErrCode.ErrOK, {}
    ent = {}
    ent['pid'] = request.params.get('pid', None)
    ent['login_name'] = request.params.login_name if len(request.params.login_name) != 0 else None
    ent['pwd'] = request.params.get('pwd', None)
    ent['addr'] = request.params.addr
    ent['email'] = request.params.get('email', '')
    ent['phone'] = request.params.get('phone', '')
    if None in (ent['pid'], ent['login_name'], ent['pwd']):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.add_ent(ent)
    if errcode == ErrCode.ErrOK:
        passwd, eid = db_w.get_entinfo_by_login_name(ent['login_name'])
        data['eid'] = eid
    return errcode, data

#peek a generator, mainly used to check if it is empty
def peek(iterable):
    try:
        first = next(iterable)
    except StopIteration:
        return None
    return first, itertools.chain([first], iterable)

@route('/ent/deleteEnt')
def deleteEnt():
    errcode, data = ErrCode.ErrOK, {}
    ent = {}
    ent['eid'] = request.params.get('eid', None)
    if ent['eid'] is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(ent['eid']))
    if not is_ancestor:
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    #the ent can be deleted only if it has no sub children and no devices attached
    customer = ctree_op.getCustomerInfoByEid(int(ent['eid']))
    if customer.own_dev_num > 0:
        errcode = ErrCode.ErrEntHasDevice
        return errcode, data
    children, channel = ctree_op.getChildrenInfoByEid(int(ent['eid']))
    res = peek(children)
    if res is None:
        g_logger.info('{} has no children accounts'.format(ent['eid']))
    else:
        channel.close()
        errcode = ErrCode.ErrEntHasChildren
        return errcode, data
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.delete_ent(ent)
    channel.close()
    return errcode, data

@route('/ent/updateEnt')
def updateEnt():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    pid = request.params.get('pid', None)
    phone = request.params.get('phone', None)
    addr = request.params.addr if len(request.params.addr) != 0 else None
    email = request.params.get('email', None)
    bms_permission = request.params.get('bms_permission', '0')
    sensor_permission = request.params.get('sensor_permission', '0')
    gps_permission = request.params.get('gps_permission', '1')
    if eid is None:
        g_logger.warn('eid is none')
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_r = BusinessDb(g_cfg['db_business_r'])
    errcode, ent = db_r.get_ent_by_eid(eid)
    if errcode != ErrCode.ErrOK:
        data['msg'] = ErrMsg[errcode]
        return errcode, data
    ent['pid'] = pid if pid is not None else ent['pid']
    ent['phone'] = phone if phone is not None else ent['phone']
    ent['addr'] = addr if addr is not None else ent['addr']
    ent['email'] = email if email is not None else ent['email']
    ent['logo_url'] = ent['logo_url']
    ent['permission'] = bms_permission + sensor_permission + gps_permission
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.update_ent(ent)
    return errcode, data

@route('/ent/getSubDeviceInfo')
def getSubDeviceInfo():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    pageno = request.params.get('pageno', 0)
    pagesize = request.params.get('pagesize', 20)
    if eid is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    ctree_op = CtreeOp(g_cfg['ctree'])
    redis_op = RedisOp(g_cfg['redis'])
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    dev_infos = redis_op.getDeviceInfos(customer.dev_ids, int(pageno), int(pagesize))
    data['total_cnt'] = len(customer.dev_ids)
    data['records'] = dev_infos
    return errcode, data

@route('/ent/searchEntByLName')
def searchEntByLName(): #todo, support vague query
    errcode, data = ErrCode.ErrOK, {}
    login_name = request.params.login_name
    if login_name is None or login_name == '':
        errcode = ErrCode.ErrLackParam
        return errcode, data
    ctree_op = CtreeOp(g_cfg['ctree'])
    customer = ctree_op.getCustomerInfoByLName(login_name)
    if customer.eid == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    #check permission
    login_id = request.params.get('LOGIN_ID')
    is_ancestor = ctree_op.isAncestor(int(login_id), customer.eid)
    if not is_ancestor and int(login_id) != customer.eid:
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    data['eid'] = customer.eid
    data['pid'] = customer.pid
    data['login_name'] = customer.login_name
    data['phone'] = customer.phone
    data['addr'] = customer.addr
    data['email'] = customer.email
    data['leaf'] = customer.is_leaf
    #get ancestor info
    ancestor, channel = ctree_op.getAncestorInfo(customer.eid)
    ancestors = []
    for node in ancestor:
        info = {'eid': node.eid,
                'text': '''{}({}/{})'''.format(node.login_name, node.own_dev_num, node.total_dev_num),
                'addr': node.addr, 'phone': node.phone, 'email': node.email}
        ancestors.append(info)
    data['ancestors'] = ancestors
    channel.close()
    return errcode, data

@route('/ent/getRunInfoByEid')
def getRunInfoByEid():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    if eid is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(eid))
    if not is_ancestor and int(login_id) != int(eid):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    redis_op = RedisOp(g_cfg['redis'])
    run_infos = redis_op.getDeviceRunInfos(customer.dev_ids)
    g_logger.info('run:{}, dev_ids:{}'.format(run_infos, customer.dev_ids))
    if run_infos is None or len(run_infos) == 0 or len(run_infos[0]) == 0:
        errcode = ErrCode.ErrDataNotFound
        return errcode, data
    map_type = request.params.get('map_type', None)
    map_type = 'baidu'
    #add dev_name
    dev_infos = redis_op.getDeviceInfos(customer.dev_ids)
    dict_devid_devname = {}
    for info in dev_infos:
        dict_devid_devname[info['dev_id']] = info['dev_name']

    dealt_run_infos = []
    for info in run_infos:
        if map_type == 'baidu':
            lon, lat = wgs84_to_bd09(float(info['longitude'])/1000000, float(info['latitude'])/1000000)
            info['longitude'] = lon
            info['latitude'] = lat
        elif map_type == 'amap':
            lon, lat = wgs84_to_gcj02(float(info['longitude']) / 1000000, float(info['latitude']) / 1000000)
            info['longitude'] = lon
            info['latitude'] = lat
        else:
            info['longitude'] = float(info['longitude']) / 1000000
            info['latitude'] = float(info['longitude']) / 1000000

        info['dev_name'] = dict_devid_devname[info['devid']]
        info['dev_status'] = 'online'
        maxtime = max(int(info['gps_time']), int(info['sys_time']))
        now = arrow.now().timestamp
        if now - maxtime > int(g_cfg['master']['offline_interval']):
            info['dev_status'] = 'offline'
            info['offline_time'] = now - maxtime
            dealt_run_infos.append(info)
            continue
        static_time = now - int(info['sys_time'])
        if static_time > int(g_cfg['master']['static_interval']):
            info['dev_status'] = 'static'
            info['static_time'] = static_time
        dealt_run_infos.append(info)

    data = dealt_run_infos
    return errcode, data

@route('/ent/uploadLogo', method=['GET', 'POST'])
def uploadLogo():
    errcode, data = ErrCode.ErrOK, {}
    login_id = request.params.get('LOGIN_ID', None)
    if int(login_id) != 10000:
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    eid = request.params.get('eid', None)
    file_logo = request.files.get('file_logo', None)
    file_name = request.params.file_name if len(request.params.file_name) != 0 else None
    if None in (eid, file_logo, file_name):
        g_logger.info('{},{},{}'.format(eid, file_logo, file_name))
        errcode = ErrCode.ErrLackParam
        return errcode, data
    base_path = g_cfg['master']['logo_dir']
    file_path = base_path + eid + '_' + file_name
    file_logo.save(file_path, overwrite=True)
    #logo_url save to db
    logo_url = g_cfg['master']['host_url'] + g_cfg['master']['logo_save_path'] + eid + '_' + file_name
    db_r = BusinessDb(g_cfg['db_business_r'])
    errcode, ent = db_r.get_ent_by_eid(eid)
    if errcode != ErrCode.ErrOK:
        data['msg'] = ErrMsg[errcode]
        return errcode, data
    ent['logo_url'] = logo_url
    db_w = BusinessDb(g_cfg['db_business_w'])
    errcode = db_w.update_ent(ent)
    return errcode, data

@route('/ent/updateCardByFile', method=['GET', 'POST'])
def updateCardByFile():
    errcode, data = ErrCode.ErrOK, {}
    login_id = request.params.get('LOGIN_ID', None)
    if int(login_id) != 10000:
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    file_upload = request.files.get('cardinfo', None)
    if file_upload is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    db_w = BusinessDb(g_cfg['db_business_w'])
    workbook = xlrd.open_workbook(file_contents=file_upload.file.read())
    sheet = workbook.sheet_by_index(0)
    rows = sheet.nrows
    for rowno in range(1, rows):
        row = sheet.row_values(rowno)
        datetime_row4 = datetime.datetime(*xlrd.xldate_as_tuple(row[4], workbook.datemode))
        expire_time = arrow.get(datetime_row4).format('YYYY-MM-DD HH:mm:ss')
        card = {'iccid' : row[0],
                'msisdn' : row[1],
                'manufacturer' : row[2],
                'package' : row[3],
                'plat_expire_time' : expire_time
        }
        errcode = db_w.update_card_info(card)
        if errcode != ErrCode.ErrOK:
            g_logger.error('{} process failed, errcode:{}'.format(rowno, errcode))

    g_logger.info('{} lines processed'.format(rows))
    return errcode, data

@route('/ent/getCardInfoByIccid')
def getCardInfoByIccid():
    errcode, data = ErrCode.ErrOK, {}
    #todo validate
    iccid = request.params.get('iccid', None)
    if iccid is None:
        errcode = ErrCode.ErrLackParam
        return errcode, data
    redis_op = RedisOp(g_cfg['redis'])
    card_info = redis_op.getCardInfoByIccid(iccid)
    data['card'] = card_info
    return ErrCode.ErrOK, data

@route('/ent/getAlarmByEid')
def getAlarmByEid():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    begin_tm = request.params.get('begin_tm', None)
    end_tm = request.params.get('end_tm', None)
    map_type = request.params.get('map_type', None)
    if None in (eid, begin_tm, end_tm):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(eid))
    if not is_ancestor and int(login_id) != int(eid):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    cassandra_op = CassandraOp()
    alarminfos = cassandra_op.getAlarmByTimeRange(customer.dev_ids, begin_tm, end_tm, read_flag)
    if alarminfos is None:
        errcode = ErrCode.ErrMysqlError
        return errcode, data
    data['alarm_infos'] = alarminfos
    return errcode, data

@route('/ent/getMileStatByEid')
def getMilestatByEid():
    errcode, data = ErrCode.ErrOK, {}
    eid = request.params.get('eid', None)
    begin_tm = request.params.get('begin_tm', None)
    end_tm = request.params.get('end_tm', None)
    if None in (eid, begin_tm, end_tm):
        errcode = ErrCode.ErrLackParam
        return errcode, data
    login_id = request.params.get('LOGIN_ID')
    ctree_op = CtreeOp(g_cfg['ctree'])
    is_ancestor = ctree_op.isAncestor(int(login_id), int(eid))
    if not is_ancestor and int(login_id) != int(eid):
        errcode = ErrCode.ErrNoPermission
        return errcode, data
    customer = ctree_op.getCustomerInfoByEid(int(eid))
    cassandra_op = CassandraOp()
    milestat_infos = cassandra_op.getMileStatByTimeRange(customer.dev_ids, begin_tm, end_tm)
    if milestat_infos is None:
        errcode = ErrCode.ErrMysqlError
        return errcode, data
    data['milestat_infos'] = milestat_infos
    return errcode, data
