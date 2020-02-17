#!/usr/bin/env python
# -*- coding: utf-8 -*-

import configparser
import logging

#######INIT LOG################
logging.basicConfig(filename='../var/log/redis_syncer.log', format='%(process)d %(thread)d %(name)s %(asctime)s [%(levelname)s] %(filename)s:%(lineno)s %(funcName)s %(message)s',
    level=logging.INFO)
g_logger = logging.getLogger('redis_syncer')
###############################

g_cfg = configparser.ConfigParser()
g_cfg.read('../etc/redis_syncer.ini')
