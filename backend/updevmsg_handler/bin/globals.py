#!/usr/bin/env python
# -*- coding: utf-8 -*-

import configparser
import logging

#######INIT LOG################
logging.basicConfig(filename='../var/log/cmd_handler.log', format='%(process)d %(name)s %(asctime)s [%(levelname)s] %(filename)s:%(lineno)s %(funcName)s %(message)s',
    level=logging.INFO)
g_logger = logging.getLogger('cmd_handler')
###############################

g_cfg = configparser.ConfigParser()
g_cfg.read('../etc/cmd_handler.ini')
