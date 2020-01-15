#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import arrow
from bottle import install, request, route, response
from constants import ErrMsg, ErrCode
from globals import g_cfg
from globals import g_logger
from businessdb import BusinessDb
import traceback

@route('/ent/getEntInfoByEid')
def getEntInfoByEid():
    login_type = request.params.get('LOGIN_TYPE', None)
    login_id = request.params.get('LOGIN_ID', None)
    g_logger.info('{},{}'.format(login_type, login_id))
    errcode, data = 0, {}
    return errcode, data

