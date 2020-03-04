#ifndef TRACE_CLIENT
#define TRACE_CLIENT

#include <LogProxy.h>
#include <IceUtil/IceUtil.h>
#include <gns_api_ice.h>
#include <string>
#include <set>
#include <time.h>
#include <stdarg.h>
#include <map>

using namespace std;

namespace Log
{

class TraceClientNew;

class CConfigInfo
{
public:
    CConfigInfo();
    ~CConfigInfo();

public:
    cfg2 cfg;
    long ctime;
};

class CConfigInfos
{
public:
    CConfigInfos();
    ~CConfigInfos();

public:
    void Set(const cfg2& c);
    void Get(vector<CConfigInfo>& cfg);
    void Del(const string& sessionid);

private:
    IceUtil::RWRecMutex m_rw_mutex;
    vector<CConfigInfo> m_cfgs;
};

class WorkerThd : public IceUtil::Thread
{
public:
	WorkerThd(TraceClientNew *tc);
    virtual ~WorkerThd();

public:
    void run();
    void destroy();
    int enqueue(const msg2& m);
    void dequeue(map<string, msgseq2>& logs);
	void handle_ice_response(::Ice::Long cfgid, const ::Log::cfgseq2& cfgs);
	void set_bool_response(const bool b);
    void connect2proxy();

protected:
    void write_log(const std::string& sessionid, msgseq2& logs);

private:
    IceUtil::Monitor<IceUtil::Mutex> m_timed_monitor;
    map<string, msgseq2> m_logs;
    TraceClientNew* m_tc;
    bool m_stop;
	long m_cfgid;
	LogProxyPrx m_prx;
	bool m_bres;
};
typedef IceUtil::Handle<WorkerThd> WorkerThdPtr;


class TraceClientNew
{
public:
    virtual ~TraceClientNew();
    virtual bool check(const cfg2& c, const char *checkif);
    
    int Init(const std::string& gns_id, const std::string& ip, int port, Ice::CommunicatorPtr ic = 0);
    void TLog(const char* filename, int line, const char* funcname, const char* sessionid, const char* fmt, ...);
    void TLogIf(const char* filename, int line, const char* funcname, const char* checkif, const char* fmt, ...);
    void DelConfig(const string& sessionid);

    virtual void setConfig(const cfg2& c) = 0;

protected:
    void SaveConfig(const cfg2& c);
    int UploadLog(const std::string& session_id, const char* file_name, int line, const char* func_name, const char* fmt, va_list va);

public:
    string m_gns_id;
    string m_ip;
    int m_port;
    
protected:
    WorkerThdPtr m_worker;
    CConfigInfos m_cfginfos;
};

}

#define TRACE_LOG_NEW(handle, sessionid, fmt, ...) handle->TLog(__FILE__, __LINE__, __FUNCTION__, sessionid, fmt, ##__VA_ARGS__);
#define TRACE_LOG_NEW_IF(handle, checkif, fmt, ...) handle->TLogIf(__FILE__, __LINE__, __FUNCTION__, checkif, fmt, ##__VA_ARGS__);

#endif
