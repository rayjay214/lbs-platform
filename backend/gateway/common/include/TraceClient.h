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
    /*�ⲿ����ҲҪ�ã����繹������ַ����ȣ����Ҫ�Ƶ�public*/
    virtual bool check(const char *checkif); // �ж��Ƿ��ϴ���־�ĺ���, ������, �Ż��ϴ�; ʹ�����ڼ̳��������ظú���
    virtual ~TraceClient() {}

    // ������TRACE_LOG��TRACE_LOG_CHECK�꣬�������Դ�ļ��кţ��˴�Ϊ���ݾɴ��룬�ݲ�ɾ����
    int DLog(const char *fmt, ...); // ֱ���ϴ���־
    int Log(const char *checkif, const char *fmt, ...); // ���������ϴ���־; ����Ϊ�ַ���checkif ; check(checkif) Ϊ��, �Ż��ϴ�

    // ������TRACE_LOG��TRACE_LOG_CHECK�꣬�����Լ�����Դ��λ�õ��鷳
    int DLogExtra(const char * file_name, int line, const char * func_name, const char *fmt, ...); // ֱ���ϴ���־������д��Դ��λ�ù����Բο�
    int LogExtra(const char * file_name, int line, const char * func_name, const char *checkif, const char *fmt, ...);// ���������ϴ���־; ����Ϊ�ַ���checkif ; check(checkif) Ϊ��, �Ż��ϴ���ͬʱ����д��Դ��λ�ù����Բο�
    
    int DLogExtra2(const char * file_name, int line, const char * func_name, const std::string & trace_session,const char *fmt, ...); // ֱ���ϴ���־������д��Դ��λ�ù����Բο�
    int LogExtra2( const char * file_name, int line, const char * func_name, const char *checkif,const std::string & trace_session, const char *fmt, ...);

    // gns_id, ip, port ��������ʹ���ߵ���Ϣ; conf_update_intvl �����ø��µ�Ƶ��(��)
    int Init(const std::string& gns_id, const std::string& ip, int port, unsigned conf_update_intvl = 2, Ice::CommunicatorPtr ic = 0);

protected:
    virtual void setConfig(const cfg& c); // �����·�����; ʹ�����ڼ̳��������ظú���; ���ð�����������( 2 ������, 1 ���ַ�����) ʹ���߿������ɾ���ʹ����Щ����; ��������ֵ, Լ��-1��ʾ�ò���Ϊδ����

    void SetTracing(bool btrace = true)
    {
        bIsTracing = btrace;
    }

    bool IsTracing()
    {
        return bIsTracing;
    }
    
    int UploadLog(std::string & session_id, const char * file_name, int line, const char * func_name, const char *fmt, va_list va); // ʵ��ִ��д����־�Ĳ���

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

    //���������˸���ʱ���Զ�ֹͣ���͸�����Ϣ
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
