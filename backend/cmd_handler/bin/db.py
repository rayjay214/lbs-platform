#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import MySQLdb
from globals import g_logger, g_cfg

class BaseDb(object):
    def __init__(self, dbcfg):
        self.host = g_cfg['MYSQL']['host']
        self.port = int(g_cfg['MYSQL']['port'])
        self.user = dbcfg['user']
        self.passwd = dbcfg['passwd']
        self.db = dbcfg['db']
        self.charset = dbcfg['charset']
        self.conn = None

    def check(self):
        if self.conn is None or not self.conn.open:
            self.connect_db()

    def connect_db(self):
        '''connect db'''
        try:
            self.conn = MySQLdb.connect(host=self.host, user=self.user, passwd=self.passwd,
                                        db=self.db, port=self.port, charset=self.charset)
        except Exception as e:
            g_logger.info('db connect failed. ' + self.host + ' ' + str(self.port) + ' ' + self.user + ' ' + self.passwd + ' ' + self.db)
            g_logger.info('exception:' + str(e))
            raise e

    def disconnect_db(self):
        '''disconnect db'''
        if self.conn and self.conn.open:
            self.conn.close()

