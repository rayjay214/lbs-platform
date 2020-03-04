/* 
	common library for service connection failover
		Tang Yaobin @ 2012-07-06
*/


#ifndef SERVICE_CONNECTION_FAILOVER_H
#define SERVICE_CONNECTION_FAILOVER_H

#include <Ice/Ice.h>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

#include <string>
#include <vector>

typedef boost::shared_mutex RWLOCK;
typedef boost::unique_lock< boost::shared_mutex > WRITELOCK;
typedef boost::shared_lock< boost::shared_mutex >  READLOCK;

typedef struct _FAILOVER_INIT_OPTION
{
	int argc;
	char **argv;
	int timeout_ms;
	int monitor_check_intvl_s;
} FAILOVER_INIT_OPTION;

typedef struct _FAILOVER_ENDPOINTS
{
	std::string service_id;
	std::vector<std::string> endpoint_list;
} FAILOVER_ENDPOINTS;

typedef struct _FAILOVER_CONNECTION
{
	int default_idx, best_idx;
	std::vector<Ice::ObjectPrx> connection;
} FAILOVER_CONNECTION;

typedef struct _FAILOVER_CONNECTION_POOL
{
	RWLOCK poolMutex;
	std::vector<FAILOVER_CONNECTION> pool;	
	int size;
} FAILOVER_CONNECTION_POOL;

extern FAILOVER_CONNECTION_POOL failover_pool;

int init_failover(FAILOVER_INIT_OPTION& init);

int create_connection(const FAILOVER_ENDPOINTS& endpoints, int& connection_id, std::string& err);

template <class ConnectionType>
int get_connection(const int& connection_id, ConnectionType& default_handle, ConnectionType& best_handle, std::string& err)
{
	READLOCK lock(failover_pool.poolMutex);

	if(connection_id < 0 || connection_id >= failover_pool.size)
	{
		err = "Invalid connection_id";
		return -1;
	}

	try
	{
		int default_idx = failover_pool.pool[connection_id].default_idx;
		int best_idx = failover_pool.pool[connection_id].best_idx;
		Ice::ObjectPrx default_prx = failover_pool.pool[connection_id].connection[default_idx];
		Ice::ObjectPrx best_prx = failover_pool.pool[connection_id].connection[best_idx];
		default_handle = ConnectionType::uncheckedCast(default_prx);
		best_handle = ConnectionType::uncheckedCast(best_prx);
	}
	catch(const Ice::Exception& ex)
	{
		err = ex.what();
		return -1;
	}

	return 0;
}

#endif
