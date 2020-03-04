from globals import g_logger, g_cfg
from cassandra_op import CassandraOp
from cassandra_op import MileStat
    

def main():
    oCassandraOp = CassandraOp()

    count_infos = []
    ret = oCassandraOp.select_gps_dev_count(count_infos)
    if ret != 0:
        g_logger.error('select_gps_dev_count failed! ret={}'.format(ret))
        return ret
        
    for cinfo in count_infos:
        g_logger.debug('select_gps_dev_count success. dev_id={}, count={}'.format(cinfo.dev_id, cinfo.count))
        if cinfo.count < 2:
            continue

        gpsinfo = []
        ret = oCassandraOp.select_gps_info(cinfo.dev_id, gpsinfo)
        if ret != 0:
            g_logger.error('select_gps_info failed! dev_id={}, ret={}'.format(cinfo.dev_id, ret))
            continue
            
        milestat = MileStat(cinfo.dev_id, gpsinfo)
        milestat.proc_milestat()
        ret = oCassandraOp.insert_milestat(milestat)
        if ret != 0:
            g_logger.error('insert_milestat failed! dev_id={}, ret={}'.format(cinfo.dev_id, ret))

    return 0

if __name__ == '__main__':
    main()

