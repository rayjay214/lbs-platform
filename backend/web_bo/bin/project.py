#!/usr/bin/env python
# -*- coding: utf-8 -*-

import bottle

from globals import g_cfg
from globals import g_logger
import route

if __name__ == "__main__":
    g_logger.info('###############manage project is starting ##########')

    if g_cfg['master']['multi-thread'] == 'yes':
        bottle.run(server='paste', host=g_cfg['master']['host'], port=int(g_cfg['master']['port']), debug=False)
        g_logger.info('###############manage project is multi-thread ##########')
    else:
        g_logger.info('###############manage project is single-thread ##########')
        bottle.run(host=g_cfg['master']['host'], port=int(g_cfg['master']['port']), debug=True)
