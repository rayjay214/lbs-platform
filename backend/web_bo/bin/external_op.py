import sys
if '../lib' not in sys.path:
    sys.path.append('../lib')

from globals import g_logger, g_cfg
from ctree_op import CtreeOp
from redis_op import RedisOp
from businessdb import BusinessDb

g_ctree_op = CtreeOp(g_cfg['ctree'])
g_redis_op = RedisOp(g_cfg['redis'])
g_db_r = BusinessDb(g_cfg['db_business_r'])
g_db_w = BusinessDb(g_cfg['db_business_w'])
