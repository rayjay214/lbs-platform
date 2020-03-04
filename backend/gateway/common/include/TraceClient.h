#ifndef TRACE_CLIENT
#define TRACE_CLIENT

#include <LogProxy.h>
#include <IceUtil/IceUtil.h>
#include <gns_api_ice.h>
#include <string>
#include <set>
#include <time.h>
#include <stdarg.h>

namespace Log
{

enum
{
    LOG_BUFSIZE = 2048,
    LOG_BATCH_SEND_SIZE = 256
};

class Worker : public IceUtil::Thread
{
private:
    msgseq2 logs_m;
    IceUtil::Monitor<IceUtil::Mutex> monitor_m;
    void *tc_m;
    unsigned conf_update_intvl_m;
    time_t last_update_time_m;
    bool done_m;
	long m_cfgid;
	LogProxyPrx m_prx, m_prx_sendLog;
	bool m_bres;
	msgseq2 m_logs2sendBuffer;
	IceUtil::Monitor<IceUtil::Mutex> m_timed_monitor;

public:
	Worker(void *tc, unsigned conf_update_intvl) : 
		tc_m(tc), conf_update_intvl_m(conf_update_intvl), last_update_time_m(0), done_m(false), m_prx(NULL), m_prx_sendLog(NULL), m_bres(true){}
	
    void run();
    void destroy();
    int enqueue(const msg2& m);
    void cfg2_to_cfg(cfg2& c2, cfg& c, std::string & sessionid);
	void handle_ice_response(::Ice::Long cfgid, const ::Log::cfgseq2& cfgs);
	void set_bool_response(const bool b);	
};
typedef IceUtil::Handle<Worker> WorkerPtr;

class TraceClient
{
public:
    /*外部可能也要用，比如构造输出字符串等，因而要移到public*/
    virtual bool check(const char *checkif); // 判断是否上传日志的函数, 返回真, 才会上传; 使用者在继承类中重载该函数
    virtual ~TraceClient() {}

    // 建议用TRACE_LOG和TRACE_LOG_CHECK宏，可以输出源文件行号，此处为兼容旧代码，暂不删除。
    int DLog(const char *fmt, ...); // 直接上传日志
    int Log(const char *checkif, const char *fmt, ...); // 根据条件上传日志; 条件为字符串checkif ; check(checkif) 为真, 才会上传

    // 建议用TRACE_LOG和TRACE_LOG_CHECK宏，以免自己填入源码位置的麻烦
    int DLogExtra(const char * file_name, int line, const char * func_name, const char *fmt, ...); // 直接上传日志，允许写入源码位置供调试参考
    int LogExtra(const char * file_name, int line, const char * func_name, const char *checkif, const char *fmt, ...);// 根据条件上传日志; 条件为字符串checkif ; check(checkif) 为真, 才会上传。同时允许写入源码位置供调试参考
    
    int DLogExtra2(const char * file_name, int line, const char * func_name, const std::string & trace_session,const char *fmt, ...); // 直接上传日志，允许写入源码位置供调试参考
    int LogExtra2( const char * file_name, int line, const char * func_name, const char *checkif,const std::string & trace_session, const char *fmt, ...);

    // gns_id, ip, port 都是描述使用者的信息; conf_update_intvl 是配置更新的频率(秒)
    int Init(const std::string& gns_id, const std::string& ip, int port, unsigned conf_update_intvl = 2, Ice::CommunicatorPtr ic = 0);

protected:
    virtual void setConfig(const cfg& c); // 配置下发处理; 使用者在继承类中重载该函数; 配置包含三个参数( 2 个整形, 1 个字符串型) 使用者可以自由决定使用哪些参数; 对于整型值, 约定-1表示该参数为未设置

    void SetTracing(bool btrace = true)
    {
        bIsTracing = btrace;
    }

    bool IsTracing()
    {
        return bIsTracing;
    }
    
    int UploadLog(std::string & session_id, const char * file_name, int line, const char * func_name, const char *fmt, va_list va); // 实际执行写入日志的操作

private:
    std::string gns_id_m, ip_m;
    int port_m;
    IceUtil::RWRecMutex conf_mtx_m;
    WorkerPtr worker_m;

    friend class Worker;
	msgseq2 m_logs2sendBuffer;	
	LogProxyPrx m_prx_sendLog;
protected:
    std::string m_session_id;

    //用于在无人跟踪时，自动停止发送跟踪消息
    bool bIsTracing;
};

}

#define TRACE_LOG(handle,fmt,...) handle->DLogExtra(__FILE__,__LINE__,__FUNCTION__,fmt,##__VA_ARGS__)
#define TRACE_LOG_CHECK(handle,checkif,fmt,...) handle->LogExtra(__FILE__,__LINE__,__FUNCTION__,checkif,fmt,##__VA_ARGS__)
#define TRACE_LOG_IF(handle, b, fmt,...) {if (b) handle->DLogExtra(__FILE__,__LINE__,__FUNCTION__,fmt,##__VA_ARGS__); }

#define SESSION_TRACE_LOG(handle,fmt,...) handle->DLogExtra(__FILE__,__LINE__,__FUNCTION__,fmt,##__VA_ARGS__)
#define SESSION_TRACE_LOG_CHECK(handle,checkif,fmt,...) handle->LogExtra(__FILE__,__LINE__,__FUNCTION__,checkif,fmt,##__VA_ARGS__)
#define SESSION_TRACE_LOG_IF(handle,b,fmt,...) {if (b) handle->DLogExtra(__FILE__,__LINE__,__FUNCTION__,fmt,##__VA_ARGS__); }

#define TRACE_LOG_CHECK2(handle,checkif,session,fmt,...) handle->LogExtra2(__FILE__,__LINE__,__FUNCTION__,checkif,session,fmt,##__VA_ARGS__)
#define TRACE_LOG_IF2(handle,b,session,fmt,...) {if (b) handle->DLogExtra2(__FILE__,__LINE__,__FUNCTION__,session,fmt,##__VA_ARGS__); }

#endif
