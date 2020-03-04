/* 
	common library for socket connection failover
		SongQuan Zhou @ 2012-08-14
*/


#ifndef GNS_CONNECTION_FAILOVER_H
#define GNS_CONNECTION_FAILOVER_H

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>

namespace GNS_FAILOVER
{

// 用于传递具有调用者所需服务的服务器的 IP 地址和端口号
typedef struct _CALLEE
{

	std::string srv_id;	// 服务 ID，由调用者负责初始化其所需的服务
	std::string ip;		// 返回调用者所需服务的服务器的 IP 地址
	int port;			// 返回调用者所需服务的服务器的端口号

    bool operator==(const _CALLEE& other) const
    {
        return ((srv_id == other.srv_id) && (ip == other.ip) && (port == other.port));
    }

    bool operator!=(const _CALLEE& other) const
    {
        return ((srv_id != other.srv_id) || (ip != other.ip) || (port != other.port));
    }

    bool operator<(const _CALLEE& other) const
    {
        return ((srv_id < other.srv_id) || (ip < other.ip) || (port < other.port));
    }

    bool operator<=(const _CALLEE& other) const
    {
        return ((srv_id <= other.srv_id) || (ip <= other.ip) || (port <= other.port));
    }
} CALLEE;

// 调用者用于指明调用成功或失败失败
typedef struct _REPORT
{

	struct timeval beg_time;	// 调用者在调用函数连接、读取或写出数据之前必须先将本字段初始化为当时的时间
	struct timeval end_time;	// 库文件内部使用，用于记录上报的实时时间
	int return_code;			// 调用者根据调用结果进行初始化，0 表示成功，-1 表示失败

} REPORT;

// 功能:调用者用于初始化 failover 库
// 输入参数:caller_id 由调用者指明其本身的服务 id
// 返回值：0 表示初始化成功，-1 表示初始化失败
int GNS_client_init( const char* caller_id);

// 功能:调用者用于获取具有其所需服务的服务器 IP 和端口号
// 输入参数:callee 的 srv_id 字段，调用者负责指明其所需的服务
// 输出参数:callee 的 ip 和 port 字段，用于返回具有调用者所需服务的服务器的 IP 地址和端口号
// 返回值：0 表示成功获取IP地址和端口，-1 表示失败
int GNS_client_getnodeinfo(CALLEE& callee);

// 功能:调用者用于汇报其连接、读写的成败情况
// 输入参数:调用者通过 callee 指明其汇报的对象是其进行连接的哪个服务，report 用于指明成功或失败，以及对应的调用的起始时间点
// 返回:0 表示汇报成功，-1 表示汇报失败
int GNS_client_report(const CALLEE& callee, REPORT& report);

}

#endif









