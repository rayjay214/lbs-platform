#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import requests
from bottle import app, error, response, install, request
from globals import g_cfg, g_logger
import constants
import permission_route
import enterprise_route
import device_route

def enable_cors():
    response.headers['Access-Control-Allow-Credentials'] = 'true'
    response.headers['Access-Control-Allow-Headers'] = 'Content-Type,X-Requested-With'
    response.headers['Access-Control-Allow-Methods'] = 'POST, GET, OPTIONS'
    response.headers['Access-Control-Allow-Origin'] = 'http://webbo.yunjiwulian.com'
    response.headers['P3P'] = 'CP=CAO PSA OUR'

if g_cfg['master']['cors'] == 'yes':
    app().add_hook('after_request', enable_cors)

@error(404)
def error404(error):
    ret = {}
    ret['errcode'] = -1
    ret['data'] = {}
    ret['success'] = False
    return json.dumps(ret)
