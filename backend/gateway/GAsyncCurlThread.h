#ifndef _GASYNCCURLTHREAD_H_
#define _GASYNCCURLTHREAD_H_

#include <Ice/Ice.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/IceUtil.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include "dev.pb.h"
#include "global.h"

#define HTTP_CGI_ADAPTER_URL "http://cloudapi.gpsoo.net/iceadapter/lbs?sign="

typedef size_t (*callback)(void*, size_t, size_t, void*);


typedef struct tagJob
{
	std::string type;
    YunJi::LbsMsg *pLbsMsg;
}Job;

class GAsyncCurlThread : public IceUtil::Thread
{
public:
    
    GAsyncCurlThread();
    ~GAsyncCurlThread();
    virtual void run();
    void stop();
    void init();
    int AddCurlHandle(CURL* curl);
    int curl_multi_select(CURLM * curl_m);
    void async_send();
    int SendHttpReq(const char* url, callback cb, void *user_ptr, std::string& params);
	static size_t CommonCurlWriteCallback(void *buffer, size_t size, size_t nmemb, void *user_ptr);
	static int AddJob2Queue(Job job);
	int AddJob2MultiHandleStack();

private:
    bool    m_stop;
    int     m_taskCount;
    CURLM*  m_multi_handle;
	static std::vector<Job> m_jobs;
	static IceUtil::RecMutex m_job_mutex;
};

typedef IceUtil::Handle<GAsyncCurlThread> GAsyncCurlThreadPtr;

#endif


