// 分片定义
#ifndef _GM_SHARDING_H
#define _GM_SHARDING_H

#include <stdint.h>
#include "BusShard.h"
// Read me
// 本文件非常重要，未经许可，不得改动本文件。
// DO NOT modify this file unless you were approved by CTO!!!!!



enum GM_SHARDING_ENUM
{
////////////////////////////////////////////////////////////////////////////////
// 出于历史原因，有许多不应该被定义分片的组件定义了一些分片ID，下面这部分分片ID属于不应该被使用的，请使用了这些分片ID的组件尽快修改掉，不要再用了
// 2015年11月1日前必须清理掉这些非法的分片ID
	SHARDING_GOOCAR_GPD = 3,
	SHARDING_GOOCAR_CPPBO = 4,
	SHARDING_GOOCAR_OPENAPIBO = 5,
	SHARDING_GOOCAR_MOVEBO = 7,
	SHARDING_BUS_GTCGSE = 51,
	SHARDING_COOMIX_GPD = 103,
	SHARDING_COOMIX_WHEREBO = 104,
	SHARDING_COMMON_GDS = 201,
	SHARDING_COMMON_GPNS = 202,
// 有误的分片ID到此结束
////////////////////////////////////////////////////////////////////////////////
  
  
	// 请在此按序定义各类需分片组件的类型，但枚举值应在1到255之间，且不可与已经存在的值冲突（也不能和上面误定义的值冲突）
	SHARDING_GOOCAR_GFS = 1,
	SHARDING_GOOCAR_GMC = 2,
	SHARDING_BUS_AREA = 6,	      // 公交业务通用分片规则，按地域分
	SHARDING_GOOCAR_GGB = 8,
	SHARDING_COOMIX_GFS = 101,
	SHARDING_COOMIX_GMC = 102,
	SHARDING_MAX_NUMBER = 255
};

// 请在此定义各类组件每个分片的容量大小
// 爱车安：GFS分片200万，GMC分片1000万


#define GM_SHARDING_VAL_SHARDING_GOOCAR_GFS  2000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_GMC  10000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_GGB  10000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_GPD  10000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_CPPBO 10000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_OPENAPIBO 10000000
#define GM_SHARDING_VAL_SHARDING_GOOCAR_MOVEBO 10000000
#define GM_SHARDING_VAL_SHARDING_BUS_GTCGSE 10000000
#define GM_SHARDING_VAL_SHARDING_COOMIX_GFS  20000000
#define GM_SHARDING_VAL_SHARDING_COOMIX_GMC  10000000
#define GM_SHARDING_VAL_SHARDING_COOMIX_GPD  10000000
#define GM_SHARDING_VAL_SHARDING_COOMIX_WHEREBO 20000000
#define GM_SHARDING_VAL_SHARDING_COMMON_GDS  10000000
#define GM_SHARDING_VAL_SHARDING_COMMON_GPNS  10000000

// Function:  获取某规则下，id对应的分片号
// Input:   x   分片规则号，在GM_SHARDING_ENUM中定义
// Input:   id  需计算分片的id
// Return:  >=0时，为该id对应的分片号； <0时表示出错
inline int GetGMShardingIdx(int x, uint64_t id)
{
    switch(x)
    {
    case SHARDING_GOOCAR_GFS:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_GFS;
    case SHARDING_GOOCAR_GMC:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_GMC;
	case SHARDING_GOOCAR_GGB:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_GGB;
    case SHARDING_GOOCAR_GPD:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_GPD;
    case SHARDING_GOOCAR_CPPBO:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_CPPBO;
    case SHARDING_GOOCAR_OPENAPIBO:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_OPENAPIBO;
    case SHARDING_BUS_AREA:
        return BUS_SHARD::getBusShardByUserid(id);
    case SHARDING_GOOCAR_MOVEBO:
        return id/GM_SHARDING_VAL_SHARDING_GOOCAR_MOVEBO;                
    case SHARDING_COOMIX_GFS:
        return id/GM_SHARDING_VAL_SHARDING_COOMIX_GFS;
    case SHARDING_COOMIX_GMC:
        return id/GM_SHARDING_VAL_SHARDING_COOMIX_GMC;
    case SHARDING_COOMIX_GPD:
        return id/GM_SHARDING_VAL_SHARDING_COOMIX_GPD;
    case SHARDING_COOMIX_WHEREBO:
        return id/GM_SHARDING_VAL_SHARDING_COOMIX_WHEREBO;
    case SHARDING_COMMON_GDS:
        return id/GM_SHARDING_VAL_SHARDING_COMMON_GDS;
    case SHARDING_COMMON_GPNS:
        return id/GM_SHARDING_VAL_SHARDING_COMMON_GPNS;
    default:
        return -1;
    }
}


// Function:  获取GFS分片规则下，对应的分片号
// Input:   id  需计算分片的id
// Input:   rpttime  GPS数据包的report time
// Return:  返回2位数字：第一位时间分片，第二位是Id分片； <0时表示出错
#define VERSION1_GFS_SHARDING_UTC_ADJUST  (50*86400) //根据切换日期计算出来的偏移值，1473120000（09/06/2016）切换分片0到1
#define VERSION1_GFS_SHARDING_DURATION (100*86400)   //100天1个Gns分片，实际分片可通过Gns调整
#define CABINET_UID_SIZE  10000000  // 每个机柜的设备ID数限制

/*
+50再除以100, 当时为了第一个分片数据量和其它分片保持一致才加的。
不然算空间预留就比较麻烦。
一个分片100天的话。 不加50第一分片50天就换新分片。
*/
inline int GetGFSShardingIdx( uint64_t id, uint64_t rpttime)
{
    int idSharding = (id%CABINET_UID_SIZE)/GM_SHARDING_VAL_SHARDING_GOOCAR_GFS;
    int utcSharding = (rpttime + VERSION1_GFS_SHARDING_UTC_ADJUST)/VERSION1_GFS_SHARDING_DURATION%10;
    return utcSharding*10+idSharding;
}

// 重载，输出分片最后时间
inline int GetGFSShardingIdx( uint64_t id, uint64_t rpttime, uint64_t &endtime)
{
    int idSharding  = (id%CABINET_UID_SIZE)/GM_SHARDING_VAL_SHARDING_GOOCAR_GFS;
    uint64_t block  = (rpttime + VERSION1_GFS_SHARDING_UTC_ADJUST)/VERSION1_GFS_SHARDING_DURATION;
    int utcSharding = block % 10;
    endtime = (block + 1) * VERSION1_GFS_SHARDING_DURATION - VERSION1_GFS_SHARDING_UTC_ADJUST;
    return utcSharding*10+idSharding;
}

inline int GetImeiShardingIdx(std::string strImei)
{
    // 不校验，调用者确保参数正确
    uint64_t shardTarget = atoll(strImei.substr(0, 11).c_str());
    return shardTarget % 11;
}

#endif  // _GM_SHARDING_H


