#!/usr/bin/env python
# -*- coding: utf-8 -*-

import configparser
import logging

#######INIT LOG################
logging.basicConfig(filename='../var/log/warning_processor.log', format='%(process)d %(name)s %(asctime)s [%(levelname)s] %(filename)s:%(lineno)s %(funcName)s %(message)s',
    level=logging.DEBUG)
g_logger = logging.getLogger('warning_processor')
###############################

g_cfg = configparser.ConfigParser()
g_cfg.read('../etc/warning_processor.ini')
