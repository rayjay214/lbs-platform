#!/usr/bin/env python
# -*- coding: utf-8 -*-

import configparser
import logging
import sys

#######INIT LOG################
logging.basicConfig(filename='../var/log/web_bo.log', format='%(process)d %(name)s %(asctime)s [%(levelname)s] %(filename)s:%(lineno)s %(funcName)s %(message)s',
    level=logging.INFO)
g_logger = logging.getLogger('web_bo')
###############################

g_cfg = configparser.ConfigParser()
g_cfg.read('../etc/web_bo.ini')

if '../lib' not in sys.path:
    sys.path.append('../lib')
