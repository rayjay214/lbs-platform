// ��Ƭ����
#ifndef _GM_SHARDING_H
#define _GM_SHARDING_H

#include <stdint.h>
#include "BusShard.h"
// Read me
// ���ļ��ǳ���Ҫ��δ����ɣ����øĶ����ļ���
// DO NOT modify this file unless you were approved by CTO!!!!!



enum GM_SHARDING_ENUM
{
////////////////////////////////////////////////////////////////////////////////
// ������ʷԭ������಻Ӧ�ñ������Ƭ�����������һЩ��ƬID�������ⲿ�ַ�ƬID���ڲ�Ӧ�ñ�ʹ�õģ���ʹ������Щ��ƬID����������޸ĵ�����Ҫ������
// 2015��11��1��ǰ�����������Щ�Ƿ��ķ�ƬID
	SHARDING_GOOCAR_GPD = 3,
	SHARDING_GOOCAR_CPPBO = 4,
	SHARDING_GOOCAR_OPENAPIBO = 5,
	SHARDING_GOOCAR_MOVEBO = 7,
	SHARDING_BUS_GTCGSE = 51,
	SHARDING_COOMIX_GPD = 103,
	SHARDING_COOMIX_WHEREBO = 104,
	SHARDING_COMMON_GDS = 201,
	SHARDING_COMMON_GPNS = 202,
// ����ķ�ƬID���˽���
////////////////////////////////////////////////////////////////////////////////
  
  
	// ���ڴ˰�����������Ƭ��������ͣ���ö��ֵӦ��1��255֮�䣬�Ҳ������Ѿ����ڵ�ֵ��ͻ��Ҳ���ܺ����������ֵ��ͻ��
	SHARDING_GOOCAR_GFS = 1,
	SHARDING_GOOCAR_GMC = 2,
	SHARDING_BUS_AREA = 6,	      // ����ҵ��ͨ�÷�Ƭ���򣬰������
	SHARDING_GOOCAR_GGB = 8,
	SHARDING_COOMIX_GFS = 101,
	SHARDING_COOMIX_GMC = 102,
	SHARDING_MAX_NUMBER = 255
};

// ���ڴ˶���������ÿ����Ƭ��������С
// ��������GFS��Ƭ200��GMC��Ƭ1000��


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

// Function:  ��ȡĳ�����£�id��Ӧ�ķ�Ƭ��
// Input:   x   ��Ƭ����ţ���GM_SHARDING_ENUM�ж���
// Input:   id  ������Ƭ��id
// Return:  >=0ʱ��Ϊ��id��Ӧ�ķ�Ƭ�ţ� <0ʱ��ʾ����
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


// Function:  ��ȡGFS��Ƭ�����£���Ӧ�ķ�Ƭ��
// Input:   id  ������Ƭ��id
// Input:   rpttime  GPS���ݰ���report time
// Return:  ����2λ���֣���һλʱ���Ƭ���ڶ�λ��Id��Ƭ�� <0ʱ��ʾ����
#define VERSION1_GFS_SHARDING_UTC_ADJUST  (50*86400) //�����л����ڼ��������ƫ��ֵ��1473120000��09/06/2016���л���Ƭ0��1
#define VERSION1_GFS_SHARDING_DURATION (100*86400)   //100��1��Gns��Ƭ��ʵ�ʷ�Ƭ��ͨ��Gns����
#define CABINET_UID_SIZE  10000000  // ÿ��������豸ID������

/*
+50�ٳ���100, ��ʱΪ�˵�һ����Ƭ��������������Ƭ����һ�²żӵġ�
��Ȼ��ռ�Ԥ���ͱȽ��鷳��
һ����Ƭ100��Ļ��� ����50��һ��Ƭ50��ͻ��·�Ƭ��
*/
inline int GetGFSShardingIdx( uint64_t id, uint64_t rpttime)
{
    int idSharding = (id%CABINET_UID_SIZE)/GM_SHARDING_VAL_SHARDING_GOOCAR_GFS;
    int utcSharding = (rpttime + VERSION1_GFS_SHARDING_UTC_ADJUST)/VERSION1_GFS_SHARDING_DURATION%10;
    return utcSharding*10+idSharding;
}

// ���أ������Ƭ���ʱ��
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
    // ��У�飬������ȷ��������ȷ
    uint64_t shardTarget = atoll(strImei.substr(0, 11).c_str());
    return shardTarget % 11;
}

#endif  // _GM_SHARDING_H


