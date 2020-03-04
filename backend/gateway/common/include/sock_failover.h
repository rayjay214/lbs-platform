/* 
	common library for socket connection failover
		Tang Yaobin @ 2012-07-24
*/


#ifndef SOCKET_CONNECTION_FAILOVER_H
#define SOCKET_CONNECTION_FAILOVER_H

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>

namespace SOCK_FAILOVER
{

typedef struct _CALLEE
{
	std::string srv_id;
	std::string ip;
	int port;
} CALLEE;

typedef struct _REPORT
{
	struct timeval beg_time;
	struct timeval end_time;
	bool ok;
	int return_code;
} REPORT;

int init(const char* caller_id);
int get(CALLEE& callee);
int report(const CALLEE& callee, const REPORT& report);

}

#endif
