#include "RedisConnectPool.h"
#include "gns_api.h"


RedisConnectPool::pointer RedisConnectPool::create()
{
    CRedisClient *client = new CRedisClient(this);
    if(client)
    {
        client->ReConnectRedis();
    }
	return client;
}

void RedisConnectPool::destroy(RedisConnectPool::pointer p)
{
    if(p)
    {
        delete p;
    }
}

bool RedisConnectPool::QryGNS(std::string &ip,int &port)
{
	GNS_API::CALLER caller;
	GNS_API::CALLEE callee;

	if (m_caller.empty() || m_callee.empty())
	{
		return false;
	}

	caller.caller_id  = m_caller;
	callee.callee_id  = m_callee;
	callee.shard_id   = m_shardID;
	callee.callee_ver = m_ver;
	

	int rc = GNS_API::GNS_API_get(caller, callee);
	if(rc != 0)
		return false;

	ip	 = callee.ip;
	port = callee.port;

	return true;
}



