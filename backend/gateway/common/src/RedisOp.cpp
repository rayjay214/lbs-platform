
#include <sstream>
#include "RedisOp.h"
#include "tostring.h"
#include "BaseConnectPool.h"
#include "RedisConnectPool.h"
#include "RedisHandle.h"
#include <algorithm>
#include "StringUtility.h"

std::string RedisOp::m_strMyGns;
std::string RedisOp::m_strGpsboxRedisGns;
std::string RedisOp::m_strAlarmRedisGns;
std::string RedisOp::m_strStatusRedisGns_R;
std::string RedisOp::m_strStatusRedisGns_W;
int RedisOp::m_dwMaxRedisConnections = 150;


char RedisOp::m_szErrorMsg[1024];

//////////////////////////////////////////////////////////////////////////
// TODO: WARNING: redisclient经过修改，跟public下的这个文件已经不同了，考虑改个名字，以避免include冲突
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// TODO: 实现分片接口
int GetEidShard(uint64_t uddwEid)
{
    return 0;
}

int GetEidGrey(uint64_t uddwEid)
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////
void* RedisOp::GetRedisClient(const std::string &strRedisObj, int shard_id, int grey_id)
{
    std::string strRedisGns;
    if (strRedisObj.compare("gpsbox") == 0)
    {
        strRedisGns = m_strGpsboxRedisGns;
    }
    else if (strRedisObj.compare("alarm") == 0)
    {
        strRedisGns = m_strAlarmRedisGns;
    }
    else if (strRedisObj.compare("status_r") == 0)
    {
        strRedisGns = m_strStatusRedisGns_R;
    }
    else if (strRedisObj.compare("status_w") == 0)
    {
        strRedisGns = m_strStatusRedisGns_W;
    }
    else
    {
        return NULL;
    }
    
    RedisConnectPool *pool = SingleRedisPoolMgr::instance().getRedisConnectPool(m_strMyGns, strRedisGns, shard_id, grey_id, m_dwMaxRedisConnections);
    CRedisClient *client   = pool? pool->grab() : NULL;
    return client;
}

