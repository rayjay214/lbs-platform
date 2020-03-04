#ifndef _REDIS_CONNECT_POOL_HEAD_H_
#define _REDIS_CONNECT_POOL_HEAD_H_

#include "BaseConnectPool.h"
#include "RedisHandle.h"
#include <IceUtil/IceUtil.h>
#include <map>
#include "singleton.h"
#include "tostring.h"


class RedisConnectPool : public BaseConnectPool<CRedisClient>
{
public:

	RedisConnectPool(const std::string &caller,const std::string &callee,int shardID = 0,int ver = 0,int max_conn = 150):BaseConnectPool<CRedisClient>(max_conn)
	{
	    m_ver     = ver;
		m_shardID = shardID;
	    m_caller = caller;
		m_callee = callee;
		
	}

	~RedisConnectPool()
	{
	    for(std::vector<pointer>::iterator it = m_conns.begin(); it != m_conns.end(); ++it)
	    {
	        destroy(*it);
	    }

		m_conns.clear();
	}
	
	bool QryGNS(std::string &ip,int &port);
    void GetGnsInfo(std::string & strCaller, std::string & strCallee)
    {
        strCaller = m_caller;
        strCallee = m_callee;
    }

protected:
	pointer create();

	void destroy(pointer);

private:
    int m_ver;
	int m_shardID;
    std::string m_caller;
	std::string m_callee;
};


class AutoRelease
{
public:
	AutoRelease(CRedisClient *client = NULL):m_client(client){}

    void setClient(CRedisClient *client)
    {
        if(!m_client)
			m_client = client;
    }

	void Release()
	{
	    if(m_client && m_client->m_redisPool)
		{
			m_client->m_redisPool->Release(m_client);
			m_client = NULL;
		}
	}
	
	~AutoRelease()
	{
		Release();
	}

private:
	CRedisClient *m_client;
};


class CManagerPool
{
public:
    CManagerPool(){}
	
	~CManagerPool(){}

	RedisConnectPool *getRedisConnectPool(const std::string &caller,const std::string &callee,int shardID = 0,int ver = 0, int max_conn = 150)
	{
	    std::string key;
	    std::map<std::string,RedisConnectPool*>::iterator it;
		
		key = caller + ":" + callee + ToString(shardID) + ":" + ToString(ver);
        if((it = m_redisPool_map.find(key)) != m_redisPool_map.end())
        {
            return it->second;
        }

		IceUtil::Mutex::Lock _lock(m_mutex);
		if((it = m_redisPool_map.find(key)) != m_redisPool_map.end())  // double check
        {
            return it->second;
        }

		RedisConnectPool *temp = new RedisConnectPool(caller,callee,shardID,ver,max_conn);
		if(temp == NULL)
			return NULL;

		m_redisPool_map[key] = temp;
		return temp;
	}
	
private:
	IceUtil::Mutex  m_mutex;
	std::map<std::string, RedisConnectPool*>  m_redisPool_map;
};


typedef CBOOST::singleton_default<CManagerPool>  SingleRedisPoolMgr;

//using   SingleRedisPoolMgr::instance().getRedisConnectPool();

#endif


