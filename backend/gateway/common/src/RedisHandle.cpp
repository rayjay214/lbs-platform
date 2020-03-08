#include <stdio.h>
#include "gns_api_raii.h"
#include "RedisHandle.h"
#include "RedisConnectPool.h"

// TODO: 提交redis请求时，如果catch到key_error和value_error，则不再重试。所有接口都要改

// 上报GSC用的方法ID
const int kMethodIDExpire = 0;
const int kMethodIDExists = 1;
const int kMethodIDTtl = 2;

const int kMethodIDGet = 101;
const int kMethodIDMGet = 102;
const int kMethodIDSetNx = 103;
const int kMethodIDSetEx = 104;
const int kMethodIDSet = 105;
const int kMethodIDDel = 106;
const int kMethodIDMSet = 107;
const int kMethodIDMDel = 108;
const int kMethodIDIncr = 109;
const int kMethodIDIncrBy = 110;
const int kMethodIDDecr = 111;

const int kMethodIDHGet = 200;
const int kMethodIDHMGet = 201;
const int kMethodIDHSet = 202;
const int kMethodIDHMSet = 203;
const int kMethodIDHDel = 204;
const int kMethodIDHIncrBy = 205;

const int kMethodIDLpush = 300;
const int kMethodIDLrem = 301;
const int kMethodIDLlen = 302;
const int kMethodIDLrange = 303;
const int kMethodIDLtrim = 304;
const int kMethodIDLpop = 305;

const int kMethodIDSMembers = 400;
const int kMethodIDSISMEMBER = 401;
const int kMethodIDSAdd = 402;
const int kMethodIDSRem = 403;

const int kMethodIDZCount = 500;
const int kMethodIDZScore = 501;
const int kMethodIDZCard = 502;
const int kMethodIDZRank = 503;
const int kMethodIDZRevRank = 504;
const int kMethodIDZRange = 505;
const int kMethodIDZRangeByScore = 506;
const int kMethodIDZRevRange = 507;
const int kMethodIDZRevRangeByScore = 508;
const int kMethodIDZAdd = 509;
const int kMethodIDZIncrBy = 510;
const int kMethodIDZRem = 511;
const int kMethodIDZRemLastRecord = 512;
const int kMethodIDZRemByScore = 513;
const int kMethodIDZRemByRank = 514;    


CRedisClient::CRedisClient(RedisConnectPool* pool):m_redisPool(pool)
{
    m_predisClient = NULL;
    m_tLast = 0;
}

CRedisClient::~CRedisClient()
{
    if(m_predisClient)
    {
        delete m_predisClient;
        m_predisClient = NULL;
    }
}

void CRedisClient::checkIdle()
{
    time_t t = time(NULL);
    if(t - m_tLast > 120 || m_predisClient == NULL)
    {
        if(ReConnectRedis())
            m_tLast = t;
    }
    else
        m_tLast = t;

}

bool CRedisClient::ReConnectRedis()
{
    m_redisPool->GetGnsInfo(m_oCaller.caller_id, m_oCallee.callee_id);

    if(m_predisClient)
    {
        delete m_predisClient;
		m_predisClient = NULL;
    }
	
    std::string ip;
	int port;

	if( m_redisPool == NULL || !m_redisPool->QryGNS(ip,port))
	{
	    return false;
	}

    try
    {
        m_predisClient = new redis::client(ip, port);
        m_oCallee.ip = ip;
        m_oCallee.port = port;

        if(m_predisClient)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch(redis::redis_error & e)
    {
        if(m_predisClient)
        {
            delete m_predisClient;
        }
        m_predisClient = NULL;
        //printf( "Connect to redis %s fail! error:%s", m_host.c_str(), e.what());
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////

bool CRedisClient::Get(const std::string & key, std::string & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDGet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            value = m_predisClient->get(key);
            if(value == redis::client::missing_value())
            {
                value.clear();
            }
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return false;
}


bool CRedisClient::Set(const std::string & key, const std::string & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSet);

    //counter用于在redis失败后，重建连接重试一次
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->set(key, value);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}


bool CRedisClient::Expire(const std::string & key, unsigned int secs)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSet);

    //counter用于在redis失败后，重建连接重试一次
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->expire(key, secs);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}


bool CRedisClient::MGet(const std::vector<std::string> & keys, std::vector<std::string> & out)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDMGet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->mget(keys,out);
            for (std::vector<std::string>::iterator it = out.begin(); it != out.end(); ++it)
            {
                if (*it == redis::client::missing_value())
                {
                    it->clear();
                }
            }
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Mset(const std::vector<std::string> &key, const std::vector<std::string> &value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDMSet);

    //counter用于在redis失败后，重建连接重试一次
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->mset(key, value);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;

}

bool CRedisClient::SetNx(const std::string& key, const std::string& value, bool & is_exist)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSetNx);

    //counter用于在redis失败后，重建连接重试一次
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            int set_result = m_predisClient->setnx(key, value);
            is_exist = (set_result == 0);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::SetEx(const std::string& key, const std::string& value, unsigned int secs)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSetEx);

    //counter用于在redis失败后，重建连接重试一次
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->setex(key, value, secs);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Del(const std::string & key )
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDDel);
    int counter = 0;

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            ret = m_predisClient->del(key);
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::MDel(const std::vector<std::string> & keys)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDMDel);
    int counter = 0;

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            ret = m_predisClient->del(keys.begin(), keys.end());
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
		catch(boost::exception &e)
		{
		    oReporter.set_return_code(1);
		    return false;
		}

        return ret;
    }

    return ret;
}

bool CRedisClient::Incr(const std::string &key,int &val)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDIncr);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            val = m_predisClient->incr(key);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Incrby(const std::string &key,long &increment, int &result)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDIncrBy);

    while(counter < 2)
    {
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            result = m_predisClient->incrby(key, increment);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Decr(const std::string &key,int &val)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDDecr);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            val = m_predisClient->decr(key);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}



//////////////////////////////////////////////////////////////////////////

bool CRedisClient::SAdd(const std::string & key, const std::string & element)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSAdd);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->sadd(key, element);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

long CRedisClient::SMembers(const std::string & key, std::set<std::string> &out )
{
    long ret = -1;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSMembers);

    int counter = 0;

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return ret;
        }

        try
        {
            ++counter;
            ret = m_predisClient->smembers(key, out);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                ret= -1;
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::SRem(const std::string & key, const std::string & element )
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSRem);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->srem(key, element);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool CRedisClient::HSet(const std::string & key, const std::string & field, const std::string & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHSet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->hset(key, field, value);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::HMSet(const std::string & key, const std::vector<std::string> &fields, const std::vector<std::string> & values)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHMSet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->hmset(key, fields, values);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }
    return true;
}

bool CRedisClient::HGet(const std::string & key, const std::string & fieid, std::string & value )
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHGet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            value = m_predisClient->hget(key, fieid);
            if (value == redis::client::missing_value())
            {
                value.clear();
            }
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return false;
}

bool CRedisClient::HMGet(const std::string & key, const std::vector<std::string> &fields,std::vector<std::string> &out)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHMGet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->hmget(key, fields, out);
            for (std::vector<std::string>::iterator it = out.begin(); it != out.end(); ++it)
            {
                if (*it == redis::client::missing_value())
                {
                    it->clear();
                }
            }
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }
    return true;
}

bool CRedisClient::HGetAll(const std::string & key, std::vector<std::pair<std::string, std::string> > &out)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHMGet);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->hgetall(key, out);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }
    return true;
}

bool CRedisClient::HDel(const std::string & key, const std::string & field)
{
    std::vector<std::string> fields;
    fields.push_back(field);
    return HDel(key, fields);
}

bool CRedisClient::HDel(const std::string & key, const std::vector<std::string> &fields)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHDel);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->hdel(key, fields);   // if field not exist, this call return false. we just ignore it.
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return false;
}

/*返回值为计数器更新后的值*/
long CRedisClient::HIncrby(const std::string & key, const std::string & fieid, long increment)
{
    long cnt = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDHIncrBy);

    int counter = 0;

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return cnt;
        }

        try
        {
            ++counter;
            cnt = m_predisClient->hincrby(key, fieid, increment);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return cnt;
            }
            else
            {
                continue;
            }
        }
        return cnt;
    }

    return cnt;
}


//////////////////////////////////////////////////////////////////////////

bool CRedisClient::Lpush(const std::string & key, const std::string & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDLpush);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->lpush(key, value);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Llen(const std::string & key, int & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDLlen);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            value = m_predisClient->llen(key);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Lrange(const std::string & key, int start, int end, std::vector<std::string>& out)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDLrange);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->lrange(key, start, end, out);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Ltrim(const std::string & key, int start, int end)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDLtrim);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->ltrim(key, start, end);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}

bool CRedisClient::Lpop(const std::string & key, std::string & value)
{
    int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDLpop);

    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            value = m_predisClient->lpop(key);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return false;
            }
            else
            {
                continue;
            }
        }
        return true;
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////


bool CRedisClient::ZAdd(const std::string &key,double score,const std::string &member)
{
	int counter = 0;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZAdd);

	 while(counter < 2)
	 {
		 //ReConnectRedis();
		 checkIdle();
		 if (NULL == m_predisClient)
		 {
             oReporter.set_return_code(1);
			 return false;
		 }
	
		 try
		 {
			 ++counter;
			 m_predisClient->zadd(key,score,member);
		 }
		 catch (redis::redis_error & e)
		 {
			 delete m_predisClient;
			 m_predisClient = NULL;
			 if(counter > 1)
			 {
                 oReporter.set_return_code(1);
				 return false;
			 }
			 else
			 {
				 continue;
			 }
		 }
		 return true;
	 }
	
	 return true;
}


bool CRedisClient::ZScore(const std::string &key,const std::string &member,double &score)
{
	 int counter = 0;
     GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZScore);

	 while(counter < 2)
	 {
		 //ReConnectRedis();
		 checkIdle();
		 if (NULL == m_predisClient)
		 {
             oReporter.set_return_code(1);
			 return false;
		 }
	
		 try
		 {
			 ++counter;
			 score = m_predisClient->zscore(key,member);
		 }
		 catch (redis::redis_error & e)
		 {
			 delete m_predisClient;
			 m_predisClient = NULL;
			 if(counter > 1)
			 {
                 oReporter.set_return_code(1);
				 return false;
			 }
			 else
			 {
				 continue;
			 }
		 }
		 catch(boost::exception &e)
		 {
		     score = -1;
		 }
		 
		 return true;
	 }
	
	 return true;
}

bool CRedisClient::ZIncrby(const std::string &key,const std::string &member, double increment, double &score)
{
	 int counter = 0;
     GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZIncrBy);

	 while(counter < 2)
	 {
		 //ReConnectRedis();
		 checkIdle();
		 if (NULL == m_predisClient)
		 {
             oReporter.set_return_code(1);
			 return false;
		 }
	
		 try
		 {
			 ++counter;
			 score = m_predisClient->zincrby(key, member, increment);
		 }
		 catch (redis::redis_error & e)
		 {
			 delete m_predisClient;
			 m_predisClient = NULL;
			 if(counter > 1)
			 {
                 oReporter.set_return_code(1);
				 return false;
			 }
			 else
			 {
				 continue;
			 }
		 }
		 catch(boost::exception &e)
		 {
		     score = -1;
		 }
		 
		 return true;
	 }
	
	 return true;
}


bool CRedisClient::ZRange(std::string key, int start, int end, std::vector<std::string> & out)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRange);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrange(key,start,end,out);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}


bool CRedisClient::ZRangeByScore(const std::string & key, double min, double max, std::vector<std::string> & out, int offset, int max_count, int range_modification)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRangeByScore);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrangebyscore(key,min,max,out,offset,max_count,range_modification);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::ZRangeByScore(const std::string & key, double min, double max, std::vector< std::pair<std::string, double> > & out, int offset, int max_count, int range_modification)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRangeByScore);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrangebyscore(key,min,max,out,offset,max_count,range_modification);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::ZRevRangeByScore(const std::string & key, double max, double min, std::vector<std::string> & out, int offset, int max_count, int range_modification)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRevRangeByScore);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrevrangebyscore(key,max,min,out,offset,max_count,range_modification);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::ZRevRangeByScore(const std::string & key, double max, double min, std::vector< std::pair<std::string, double> > & out, int offset, int max_count, int range_modification)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRevRangeByScore);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrevrangebyscore(key,max,min,out,offset,max_count,range_modification);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            else
            {
                continue;
            }
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::ZCard(std::string key, int& num)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZCard);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            num = m_predisClient->zcard(key);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
        return ret;
    }

    return ret;
}

bool CRedisClient::ZCount(const std::string &key,double min,double max,int &num,int range_modification)
{
	bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZCount);
	int counter = 0;
	while(counter < 2)
	{
		//ReConnectRedis();
		checkIdle();
		if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
			return false;
        }

		try
		{
			++counter;
			num = m_predisClient->zcount(key,min,max,range_modification);
			ret = true;
		}
		catch (redis::redis_error & e)
		{
			ret = false;
			delete m_predisClient;
			m_predisClient = NULL;
			if(counter > 1)
            {
                oReporter.set_return_code(1);
				return ret;
            }

			continue;
		}
		return ret;
	}

	return ret;
}


bool CRedisClient::ZRem(const std::string &key,const std::string &member)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRem);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrem(key,member);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
        return ret;
    }

    return ret;

}


bool CRedisClient::ZRemRangeByRank(const std::string &key, int start, int end)
{
	bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRemByRank);
	int counter = 0;
	while(counter < 2)
	{
		//ReConnectRedis();
		checkIdle();
		if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
			return false;
        }

		try
		{
			++counter;
			m_predisClient->zremrangebyrank(key,start,end);
			ret = true;
		}
		catch (redis::redis_error & e)
		{
			ret = false;
			delete m_predisClient;
			m_predisClient = NULL;
			if(counter > 1)
            {
                oReporter.set_return_code(1);
				return ret;
            }
            continue;
		}
		return ret;
	}

	return ret;
}


bool CRedisClient::ZRemRangeByScore(const std::string &key,double min,double max)
{
	bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRemByScore);
	int counter = 0;
	while(counter < 2)
	{
		//ReConnectRedis();
		checkIdle();
		if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
			return false;
        }

		try
		{
			++counter;
			m_predisClient->zremrangebyscore(key,min,max);
			ret = true;
		}
		catch (redis::redis_error & e)
		{
			ret = false;
			delete m_predisClient;
			m_predisClient = NULL;
			if(counter > 1)
            {
                oReporter.set_return_code(1);
				return ret;
            }
            continue;
		}
		return ret;
	}

	return ret;
}




bool CRedisClient::SISMEMBER(std::string key,std::string value)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDSISMEMBER);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            ret = m_predisClient->sismember(key,value);
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
        return ret;
    }

    return ret;
}


bool CRedisClient::ZRank(const std::string &key,const std::string &member,int &rank)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRank);
    int counter = 0;

	rank = -1;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            // rank返回-1表示key或者member不存在
            rank = m_predisClient->zrank(key,member);
			ret = true;
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
		catch(boost::exception &e)
		{
		    rank = -1;
			ret = true;
		}
		
        return ret;
    }

    return ret;
}

bool CRedisClient::ZRevRank(const std::string &key,const std::string &member,long &rank)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRevRank);
    int counter = 0;

	rank = -1;
    while(counter < 2)
    {
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            // rank返回-1表示key或者member不存在
            rank = m_predisClient->zrevrank(key,member);
			ret = true;
        }
        catch (redis::redis_error & e)
        {
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
		catch(boost::exception &e)
		{
		    rank = -1;
			ret = true;
		}
		
        return ret;
    }

    return ret;
}

bool CRedisClient::ZRevRange(const std::string & key, long start, long end, std::vector< std::string > & out)
{
    bool ret = false;
    GNS_API::GNS_API_RAII oReporter(m_oCaller, m_oCallee, kMethodIDZRevRange);
    int counter = 0;
    while(counter < 2)
    {
        //ReConnectRedis();
        checkIdle();
        if (NULL == m_predisClient)
        {
            oReporter.set_return_code(1);
            return false;
        }

        try
        {
            ++counter;
            m_predisClient->zrevrange(key, start, end, out);
            ret = true;
        }
        catch (redis::redis_error & e)
        {
            ret = false;
            delete m_predisClient;
            m_predisClient = NULL;
            if(counter > 1)
            {
                oReporter.set_return_code(1);
                return ret;
            }
            continue;
        }
        return ret;
    }

    return ret;
}

