// Tang Yaobin @ 2012-09-04

#ifndef GNS_API_ICE_H
#define GNS_API_ICE_H

#include <Ice/Ice.h>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#include <string>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <map>
#include <errno.h>
#include "gns_api.h"

namespace GNS_API
{

class GNS_API_ICE
{
typedef boost::shared_mutex RWLOCK;
typedef boost::unique_lock< boost::shared_mutex > WRITELOCK;
typedef boost::shared_lock< boost::shared_mutex > READLOCK;

private:
	
std::map<std::string, Ice::ObjectPrx> proxypool;
RWLOCK poolmutex;
Ice::CommunicatorPtr gns_ic;
bool use_external_ic;
unsigned int m_timeout;
std::string err_msg;

public:
GNS_API_ICE(unsigned int proxy_timeout_ms = 5000):use_external_ic(false), m_timeout(proxy_timeout_ms)
{
	int argc = 2;
	char *argv[2] = {(char *)"--Ice.ThreadPool.Client.Size=2", (char *)"--Ice.ThreadPool.Client.SizeMax=8"};

	try
	{
		gns_ic = Ice::initialize(argc, argv);
	}
	catch(const Ice::Exception& ex)
	{
		err_msg = ex.what() + std::string(" ") + std::string(strerror(errno));
		gns_ic = NULL;
	}
}

GNS_API_ICE(int argc, char *argv[], unsigned int proxy_timeout_ms = 5000):use_external_ic(false), m_timeout(proxy_timeout_ms)
{	
	try
	{
		gns_ic = Ice::initialize(argc, argv);
	}
	catch(const Ice::Exception& ex)
	{
		err_msg = ex.what() + std::string(" ") + std::string(strerror(errno));
		gns_ic = NULL;
	}
}

GNS_API_ICE(Ice::CommunicatorPtr ic, unsigned int proxy_timeout_ms = 5000):gns_ic(ic), use_external_ic(true), m_timeout(proxy_timeout_ms)
{
}

~GNS_API_ICE()
{
	if(false == use_external_ic)
	{
		try
		{
			if(gns_ic)
				gns_ic->destroy();
		}	
		catch(const Ice::Exception& ex)
		{
		}
	}
	else
	{
		gns_ic = NULL;
	}
}

std::string GetErrMsg() const
{
    return err_msg;
}

bool IsValid()
{
	if(!gns_ic)
		return false;

	return true;
}


template <class ProxyType>
int GNS_API_getproxyByIpPort(const CALLER& caller, const CALLEE& callee, const std::string& callee_objectid, 
							ProxyType& proxy, int timeout = 0)
{
    timeout = (0 == timeout) ? m_timeout : timeout;
    if (!gns_ic)
    {
        err_msg = "Ice Communicator invalid!";
        return -3;
    }

	char key[1024];
	snprintf(key, sizeof(key) - 1, "%s%s%s%s%d%d", caller.caller_id.c_str(), callee.callee_id.c_str(), callee_objectid.c_str(), callee.ip.c_str(), callee.port, timeout);

	{
		READLOCK lock(poolmutex);

		std::map<std::string, Ice::ObjectPrx>::iterator it = proxypool.find(key);
		if(it != proxypool.end())
		{
			proxy = ProxyType::uncheckedCast(it->second);

			return 0;
		}
		
	}

	REPORT report;
	gettimeofday(&report.beg_time, NULL);
	report.method_id = 0;

	char proxystr[1024];
	try
	{
		snprintf(proxystr, sizeof(proxystr) - 1, "%s:tcp -h %s -p %d -t %u", callee_objectid.c_str(), callee.ip.c_str(), callee.port, timeout);
		proxy = ProxyType::checkedCast(gns_ic->stringToProxy(proxystr));
		if(!proxy)
		{
            err_msg = "ICE checkedCast fail!";
			return -1;
		}
	}
	catch(const Ice::Exception& ex)
	{
		gettimeofday(&report.end_time, NULL);
		report.return_code = -1;
		GNS_API_report(caller, callee, report);
        err_msg = ex.what();
		return -1;
	}

	proxy->m_caller = caller;
	proxy->m_callee = callee;

	{
		WRITELOCK lock(poolmutex);

		proxypool[key] = proxy;
	}

	return 0;
}

// Function: Get GNS handle by Caller&Callee name.
// Input: callerid          Caller's GNS name
// Input: calleeid          Callee's GNS name
// Input: callee_objectid   Callee's Ice ObjectID
// Output: proxy            Ice handle
// Input: callee_ver        callee grey version.
// Input: timeout           Ice connect&invoke timeout in milliseconds, if it's set to 0, then use the default value(m_timeout)
// Return: 0 if success. other values means error, see GetErrMsg()
template <class ProxyType>
int GNS_API_getproxy(const std::string& callerid, const std::string& calleeid, const std::string& callee_objectid, ProxyType& proxy, int callee_ver = 0, int timeout = 0)
{
	CALLER caller;
	CALLEE callee;
	caller.caller_id = callerid;
	callee.callee_id = calleeid;
	callee.callee_ver = callee_ver;
    timeout = (0 == timeout) ? m_timeout : timeout;

    if(!gns_ic)
    {
        err_msg = "Ice Communicator invalid!";
        return -3;
    }

	int rc = GNS_API_get(caller, callee);
	if (rc != 0)
	{
        char szTmp[128] = {0};
        snprintf(szTmp, sizeof(szTmp), "GNS config error! ret=%d.", rc);
        err_msg = szTmp;
		return -2;
	}
    return GNS_API_getproxyByIpPort(caller, callee, callee_objectid, proxy, timeout);
}

// Function: Get GNS handle by Caller&Callee info struct.
// Input: callerid          Caller's GNS info
// Input: calleeid          Callee's GNS info
// Input: callee_objectid   Callee's Ice ObjectID
// Output: proxy            Ice handle
// Input: timeout           Ice connect&invoke timeout in milliseconds, if it's set to 0, then use the default value(m_timeout)
// Return: 0 if success. other values means error, see GetErrMsg()
template <class ProxyType>
int GNS_API_getproxy(CALLER& caller, CALLEE& callee, const std::string& callee_objectid, ProxyType& proxy, int timeout = 0)
{
    timeout = (0 == timeout) ? m_timeout : timeout;
    if(!gns_ic)
    {
        err_msg = "Ice Communicator invalid!";
        return -3;
    }

	int rc = GNS_API_get(caller, callee);
	if (rc != 0)
	{
        char szTmp[128] = {0};
        snprintf(szTmp, sizeof(szTmp), "GNS config error! ret=%d.", rc);
        err_msg = szTmp;
		return -2;
	}
    return GNS_API_getproxyByIpPort(caller, callee, callee_objectid, proxy, timeout);
}

template <class ProxyType>
int GNS_API_getProxys(const std::string& callerid, const std::string& calleeid, const std::string& callee_objectid, const int & oShardID, 
					std::vector<ProxyType> &proxyVec, bool partAllow = false, int callee_ver = 0, int timeout = 0)
{
    timeout = (0 == timeout) ? m_timeout : timeout;
    
    CALLER oCaller;
    CALLEE oCallee;
    oCaller.caller_id = callerid;
    oCallee.callee_id = calleeid;
    oCallee.callee_ver = callee_ver;
    oCallee.shard_id = oShardID;
    
    std::vector<CALLEE> CalleeVec;
    int ret = GNS_API_gets(oCaller, oCallee, CalleeVec);
    if (ret || CalleeVec.empty())
    {
        err_msg = "GNS_API_gets fail";
        return -4;
    }
    for (size_t i = 0; i < CalleeVec.size(); ++i)
    {
        ProxyType proxy;
        ret = GNS_API_getproxyByIpPort(oCaller, CalleeVec[i], callee_objectid, proxy, timeout);
        if (ret)
        {
        	if (false == partAllow)
            {
	            err_msg = "GNS_API_getproxyByIpPort fail";
	            return -5;
            }
            continue;
        }
        proxyVec.push_back(proxy);
    }
	return 0;
}

};

}

#endif
