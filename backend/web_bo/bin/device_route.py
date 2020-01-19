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
    return errcode, data

