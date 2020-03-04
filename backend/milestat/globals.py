#!/usr/bin/env python
# -*- coding: utf-8 -*-

import configparser
import logging

#######INIT LOG################
logging.basicConfig(filename='../var/log/mile_stat.log', format='%(process)d %(name)s %(asctime)s [%(levelname)s] %(filename)s:%(lineno)s %(funcName)s %(message)s',
    level=logging.INFO)
g_logger = logging.getLogger('mile_stat')
###############################

g_cfg = configparser.ConfigParser()
g_cfg.read('../etc/mile_stat.ini')
