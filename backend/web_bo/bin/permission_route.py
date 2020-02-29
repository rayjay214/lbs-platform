#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import time
import arrow
from bottle import install, request, route, response
import MySQLdb
import requests
from constants import ErrMsg, ErrCode
from globals import g_cfg, g_logger
import hashlib
from businessdb import BusinessDb
import traceback
from ctree_op import CtreeOp

def verify(callback):
    def wrapper(*args, **kwargs):
        ret = {}
        errcode, data, e = 0, {}, ''

        try:
            method = '/'.join(request.fullpath.split('/')[-2:])

            test = request.params.get('test', None)

            if method != 'login/loginByEnt' and test is None:
                access_token = request.get_cookie("access_token", None)
                if access_token is None:
                    access_token = request.params.get("access_token", None)

                errcode = validate_token(access_token)
                if errcode == ErrCode.ErrOK:
                    errcode, data = callback(*args, **kwargs)
                else:
                    g_logger.warning('permisson check fail. token:{}, method:{}'.format(access_token, method))
            else:
                errcode, data = callback(*args, **kwargs)

        except Exception as e:
            errcode = -1
            g_logger.error(traceback.format_exc())

        ret['errcode'] = errcode
        ret['data'] = data
        ret['msg'] = ErrMsg.get(errcode, 'unknown errcode')
        ret['success'] = False if errcode else True

        return ret
    return wrapper

install(verify)

def validate_token(access_token):
    if access_token is None:
        return ErrCode.ErrLackParam

    #parse token
    cursor = 1
    login_type = access_token[cursor:cursor+2]
    cursor += 2
    id_len = int(access_token[cursor:cursor+2])
    cursor += 2
    id = access_token[cursor:cursor+id_len]
    cursor += id_len
    time_len = int(access_token[cursor:cursor+2])
    cursor += 2
    time = access_token[cursor:cursor+time_len]
    cursor += time_len
    token_len = int(access_token[cursor:cursor+2])
    cursor += 2
    token = access_token[cursor:cursor+token_len]
    g_logger.info('{},{},{}'.format(id,time,token))

    #verify token
    check = hashlib.md5((id+time+g_cfg['master']['salt']).encode('utf-8')).hexdigest()
    g_logger.info(check)
    if check != token:
        return ErrCode.ErrTokenInvalid

    #check timeout
    time_now = arrow.now().timestamp
    if (time_now - int(time)) > 86400:
        return ErrCode.ErrTokenTimeout

    request.params['LOGIN_TYPE'] = str(int(login_type))
    request.params['LOGIN_ID'] = id
    return ErrCode.ErrOK

def get_v_length(value):
    strlen = str(len(value))
    rst = strlen
    if len(strlen) == 0:
        rst = '00'
    elif len(strlen) == 1:
        rst = '0' + strlen
    elif len(strlen) > 2:
        rst = 'ff'
    return rst


# login by enterprise
@route('/login/loginByEnt', method=['GET', 'POST'])
def enterprise_login():
    errcode, data = ErrCode.ErrOK, {}
    login_name = request.params.get('login_name', None)
    time = request.params.get('time', None)
    signature = request.params.get('signature', None) #md5(md5(passwd) + time)
    if login_name is None or time is None or signature is None:
        errcode = ErrCode.ErrLackParam
        data['msg'] = ErrMsg[errcode]
        return errcode, data

    # get passwd from db, make md5, compare with signature
    db = BusinessDb(g_cfg['db_business_r'])
    passwd,eid = db.get_entinfo_by_login_name(login_name)
    if passwd is None:
        errcode = ErrCode.ErrDataNotFound
        data['msg'] = ErrMsg[errcode]
        return errcode, data

    temp = hashlib.md5(passwd.encode('utf-8')).hexdigest()
    calc_token = hashlib.md5((temp+time).encode('utf-8')).hexdigest()
    g_logger.debug('signature:{}, calc_token:{}'.format(signature, calc_token))
    if signature != calc_token:
        errcode = ErrCode.ErrPwdNotMatch
        data['msg'] = ErrMsg[errcode]
        return errcode, data

    # generate access_token (ver+id_type+id+time+md5(id+time+salt))
    version = '1'
    login_by_ent = '00'
    check = hashlib.md5((str(eid)+time+g_cfg['master']['salt']).encode('utf-8')).hexdigest()
    access_token = version+login_by_ent+get_v_length(str(eid))+str(eid)+get_v_length(time)+time+get_v_length(check)+check
    response.set_cookie('access_token', access_token, path='/', expires=arrow.now().timestamp + 86400, max_age=86400)
    #response.status = 302
    data['access_token'] = access_token
    data['eid'] = eid

    #get logo
    ctree_op = CtreeOp(g_cfg['ctree'])
    customer = ctree_op.getCustomerInfoByLName(login_name)
    data['logo_url'] = customer.logo_url
    if eid == 8888:
        data['url'] = '/home/yjcenter'
    else:
        data['url'] = '/home'
    return errcode, data

@route('/login/permissionTest', method=['GET'])
def per_test():
    login_type = request.params.get('LOGIN_TYPE', None)
    login_id = request.params.get('LOGIN_ID', None)
    g_logger.info('{},{}'.format(login_type, login_id))
    errcode, data = 0, {}
    return errcode, data

