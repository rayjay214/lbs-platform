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
    email  = request.params.get('email', None)
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
    login_name = request.params.get('login_name', None)
    if login_name is None:
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
    data = run_infos
    return errcode, data
