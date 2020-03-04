/*
	GNS API Library Code
*/


#ifndef GNS_API_H
#define GNS_API_H

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>
#include "ms_interface.h"

namespace GNS_API
{

// 获取被调者时是否需区分IDC
enum SPECIFY_IDC_TYPE
{
    NO_SPECIFY_IDC = 0,         // 不区分IDC
    SPECIFY_SAME_IDC = 1,       // 只获取同IDC的服务
	SPECIFY_SPECIAL_IDC = 2,	// 只获取指定IDC的服务
    SPECIFY_DIFFERENT_IDC = -1  // 只获取不同IDC的服务
};

// 调用者信息结构
struct CALLER
{
	std::string caller_id;      // 调用者的GNS名称
    std::string caller_ip;      // 所在IP
    int caller_port;            // 监听端口
    std::string caller_idc;     // 所在IDC名称，格式一般为“线路前缀 + 城市前缀 + IDC机房前缀 + 机架标识”
    int shard_id;               // 分片ID
    int grey_ver;               // 灰度版本
    CALLER():caller_port(0), shard_id(0), grey_ver(0)
    {
    }; 
} ;

// 被调用者信息结构
struct CALLEE
{
	std::string callee_id;      // 被调用者的GNS名称
	std::string ip;             // 所在IP
	int port;                   // 监听端口
    std::string callee_idc;     // 所在IDC名称，格式一般为“线路前缀 + 城市前缀 + IDC机房前缀 + 机架标识”
    int priority;               // 优先级，在没发生错误的情况下，该值最高的一个被调者服务会被选中。该值>0表示被调服务可用，<=0表示不可用
    int shard_id;               // 分片ID，不使用分片的服务都默认为0（即第一片）
	int callee_ver;             // 灰度版本号，不使用灰度的服务都默认为0（即第一个版本）
    int specify_idc;            // 指定IDC。0=不区分IDC（默认）。1=只获取同IDC的服务，2=只获取指定IDC的服务，-1=只获取不同IDC的服务
    int choose_strategy;		//0是随机选择(默认)，1为轮询，其他暂无定义
    int specify_master;         // 0是不区分目标是master还是slave（默认），1是指定获取master，-1是指定获取slave
	
	CALLEE()
	{
		port = 0;
		priority = 0;
        shard_id = 0;
		callee_ver = 0;
        specify_idc = (int)NO_SPECIFY_IDC;
		choose_strategy = 0;
        specify_master = 0;
	}; 

    bool operator==(const CALLEE & other) const
    {
        return ((callee_id == other.callee_id)
            && (ip == other.ip)
            && (port == other.port)
            && (shard_id == other.shard_id)
            && (callee_ver == other.callee_ver));
    }

    bool operator!=(const CALLEE & other) const
    {
        return ((callee_id != other.callee_id)
            || (ip != other.ip)
            || (port != other.port)
            || (shard_id != other.shard_id)
            || (callee_ver != other.callee_ver));
    }

    bool operator<(const CALLEE & other) const
    {
        if (callee_id == other.callee_id)
        {
            if (ip == other.ip)
            {
                return (port < other.port);
            }
            else
            {
                return (ip < other.ip);
            }
        }
        else
        {
            return (callee_id < other.callee_id);
        }
    }
};

// 上报GNS调用结果的结构
struct REPORT
{
	struct timeval beg_time;        // 调用开始时间
	struct timeval end_time;        // 调用结束时间
	int return_code;                // 调用返回码。0表示调用正常，负值表示出错。而[-100, -1]为预留的错误号，这部分用于网络相关的错误表示。
	int method_id;                  // 调用的方法ID。对于被调者提供的各个接口（方法），建议使用不同的ID区分之，以跟踪错误。
};


// 函数功能：   根据调用者和被调用者的GNS名称，找到一个合适的被调者服务
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输入参数：   callee  被调用者的信息，其中必填的是callee.callee_id，
//                      如果想不需要获取某特定分片的服务，则callee.shard_id填-1
//                      如果不需要获取某特定灰度版本的服务，则callee.callee_ver填-1
//                      如果只想获取Master实例，则需设置callee.specify_master=1，此时shard_id和callee_ver不能是-1.
// 输出参数：   callee  选中的某个被调用者服务的全部信息
// 返回值：     返回0表示选路成功，其他表示失败。
int GNS_API_get(const CALLER& caller, CALLEE& callee);


// 函数功能：   根据调用者和被调用者的GNS名称，找到一个合适的被调者服务，并且这个服务在最近的一分钟之前至少仍是可用的
//              该函数会尽力找一个优先级>0的被调者，如果找不到，则返回失败。
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输入参数：   callee  被调用者的信息，其中必填的是callee.callee_id，
//                      如果想不需要获取某特定分片的服务，则callee.shard_id填-1
//                      如果不需要获取某特定灰度版本的服务，则callee.callee_ver填-1
// 输出参数：   callee  选中的某个被调用者服务的全部信息
// 返回值：     返回0表示选路成功，其他表示失败。
int GNS_API_get_first_available(const CALLER& caller, CALLEE& callee);


// 函数功能：   根据调用者和被调用者的GNS名称，找到符合条件的所有被调者服务，并在callee中填入一个合适的被调者服务。
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输入参数：   callee  被调用者的信息，其中必填的是callee.callee_id，
//                      如果想不需要获取某特定分片的服务，则callee.shard_id填-1
//                      如果不需要获取某特定灰度版本的服务，则callee.callee_ver填-1
//                      如果想获取各个分片、灰度的Master实例，则需设置callee.specify_master=1，此时callees可以拿到符合搜索条件的所有Master。
// 输出参数：   callee  选中的某个被调用者服务的全部信息
// 输出参数：   callees 符合条件的所有被调用者服务的信息
// 返回值：     返回0表示执行成功，其他表示失败。
int GNS_API_gets(const CALLER& caller, CALLEE& callee,std::vector<CALLEE>& callees);


// 函数功能：   根据调用者的GNS名称，找到能被该调用者调用的所有服务的全部信息。
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输出参数：   callees 能被该调用者调用的所有服务的信息
// 返回值：     返回0表示执行成功，其他表示失败。
int GNS_API_GetAllCallee(const CALLER& caller, std::map<std::string, std::vector<CALLEE> > & callees);


// 函数功能：   根据调用者的GNS名称和端口号，获取该调用者的信息，如IDC名称，分片/灰度号等
// 输入参数：   gns_name 调用者的GNS名
// 输入参数：   port     调用者的端口号（因该调用者可能会占用多个端口）。如果填-1，表示不指定端口号，取第一条记录的信息。
// 输出参数：   caller   该调用者结构中的其他成员
// 返回值：     返回0表示执行成功，其他表示失败。
int GNS_API_GetCallerInfo(const std::string & gns_name, int port, CALLER & caller);

// 函数功能：   获取“调用者是否需要调用所有的被调者服务”的配置信息。对该结果不做强制要求，仅供参考
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输入参数：   callee  被调用者的信息，其中必填的是callee.callee_id，
// 返回值：     返回1表示“建议调用该callee的所有服务”；0表示“建议调用该callee的其中一个服务”；其他表示执行失败。
int GNS_API_IfCallAll(const CALLER & caller, const CALLEE & callee);

// 函数功能：   获取上一次发送错误的描述信息
// 返回值：     错误描述
std::string GNS_API_GetErrorMsg();

// 函数功能：   （以本地日志方式）上报调用结果给GNS服务及GSC服务
// 输入参数：   caller  调用者的信息，其中只需要填入caller.caller_id即可
// 输入参数：   callee  被调用者的信息
// 输入参数：   report  调用结果
// 返回值：     返回0表示执行成功，其他表示失败。
int GNS_API_report(const CALLER& caller, const CALLEE& callee, const REPORT& report);

// 如果返回1，则在同一个IDC，返回-1表示不在同个IDC
int IdcRelation(const std::string & caller_idc, const std::string & callee_idc);

// 设置主从信息变化时的回调函数
void SetMsCallback(MsApiCallback f);
}

#endif

