#include <openssl/md5.h>
#include "lbs.pb.h"
#include "tostring.h"
#include "common.h"
#include "DispatchThread.h"
#include "GAsyncCurlThread.h"


std::vector<Job> GAsyncCurlThread::m_jobs;
IceUtil::RecMutex GAsyncCurlThread::m_job_mutex;

extern ReceiveDataList g_oReceiveList;

int MakeMD5(const std::string& rawData, std::string& md5Data)
{
    uint32_t len = rawData.length();
    char tmp[3] = {0};
    char buf[33]= {0};
    uint8_t md[16] = {0};

    // malloc memory
    uint8_t* data = new uint8_t[len];
    memcpy(data, rawData.c_str(), len); 
    if (NULL == MD5(data, rawData.length(), md))
    {   
        // free memory
        delete[] data;
        md5Data = "set md5 fail";
        return -1;
    }   

    // free memory
    delete[] data;
    for (int i = 0; i < 16; i++)
    {   
        sprintf(tmp, "%2.2x", md[i]);
        strcat(buf,tmp);
    }   

    md5Data = buf;
    return 0;
}

size_t CurlLbsCallback(void *buffer, size_t size, size_t nmemb, void *user_ptr)
{
    YunJi::LbsMsg *pLbsMsg = (YunJi::LbsMsg*)user_ptr;
    std::string strRsp;
    strRsp.append((char*)buffer, size*nmemb);
    
    lbs::LbsRec oRes;
    oRes.ParseFromString(strRsp);
	MYLOG_DEBUG(g_logger,"Lbs query result: ID=%s, Location=%lf,%lf, Acc=%lf", 
        oRes.bsid().c_str(), oRes.lat(), oRes.lon(), oRes.accuracy());

    long lng = long(oRes.lon()*1000000);
    long lat = long(oRes.lat()*1000000);

    //发送数据到kafka
	YunJi::GpsMsg oPbGps;
    oPbGps.set_id(pLbsMsg->id());
    oPbGps.set_imei(pLbsMsg->imei());
    oPbGps.set_seq(pLbsMsg->seq());
    oPbGps.set_systime(pLbsMsg->systime());
    oPbGps.set_lat(lat);
    oPbGps.set_lng(lng);
    oPbGps.set_speed(-1);
    oPbGps.set_route(0);
    oPbGps.set_datetime(pLbsMsg->datetime());

    YunJi::UpDevMsg oPbMsg;
    oPbMsg.set_msgtype(YunJi::kGpsMsg);
    *(oPbMsg.mutable_gps()) = oPbGps;

    std::string strPbRes;
    oPbMsg.SerializeToString(&strPbRes);
    g_oReceiveList.AddData(strPbRes);

    //更新redis
    SSMap mpParam;
    mpParam["latitude"] = ToString(oPbGps.lat());
    mpParam["longitude"] = ToString(oPbGps.lng());
    mpParam["speed"] = ToString(oPbGps.speed());
    mpParam["course"] = ToString(oPbGps.route());
    mpParam["sys_time"] = ToString(oPbGps.systime());
    mpParam["gps_time"] = ToString(oPbGps.datetime());
    saveRedisDevRunInfo(oPbGps.id(), mpParam);

    delete(pLbsMsg);
    return size * nmemb;
}


GAsyncCurlThread::GAsyncCurlThread()
{
    m_stop = false;
}

GAsyncCurlThread::~GAsyncCurlThread()
{
    stop();
}

void GAsyncCurlThread::stop()
{
	MYLOG_INFO(g_logger,"async curl thread stopped");
    m_stop = true;
}

void GAsyncCurlThread::init()
{
	m_taskCount = 0;
    m_multi_handle = curl_multi_init();
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void GAsyncCurlThread::async_send()
{
	int msgs_left;
    CURLMsg* msg;
    int running_handles;
    int res;

    if(0 == m_taskCount)
    {
        usleep(100);
		AddJob2MultiHandleStack();
        return;
    }

    do
    {
        res = curl_multi_perform(m_multi_handle, &running_handles);
    } while(res == CURLM_CALL_MULTI_PERFORM);

    if (CURLM_OK != res)
    {
        MYLOG_WARN(g_logger, "curl_multi_perform error, ret is %d", res);
		exit(1);
    }

    while (running_handles >= 0)
    {
		AddJob2MultiHandleStack();
	
        int rc = curl_multi_select(m_multi_handle);
        if (-1 == rc)
        {
            MYLOG_WARN(g_logger, "select error happend");
        }
        do
        {
            res = curl_multi_perform(m_multi_handle, &running_handles);
        } while(CURLM_CALL_MULTI_PERFORM == res);

        if (CURLM_OK != res)
        {
            MYLOG_WARN(g_logger, "curl_multi_perform error, ret is %d", res);
			exit(1);
        }

        while((msg = curl_multi_info_read(m_multi_handle, &msgs_left)))
        {
            if (CURLMSG_DONE == msg->msg)
            {
                CURL* curl = msg->easy_handle;
                m_taskCount--;
				MYLOG_DEBUG(g_logger, "task count %d, msg_left %d", m_taskCount, msgs_left);
                curl_multi_remove_handle(m_multi_handle, curl);
                curl_easy_cleanup(curl);
            }
			else
			{
                MYLOG_WARN(g_logger, "error msg status %d", msg->msg);;
			}		
        }
    }
}

void GAsyncCurlThread::run()
{
	MYLOG_INFO(g_logger,"async curl thread start");
    while (!m_stop)
    {
        async_send();
    }

	/*check whether there are running tasks*/
	if(0 != m_taskCount)
		async_send();
}

int GAsyncCurlThread::AddCurlHandle(CURL* curl)
{
	int ret;

    ret = curl_multi_add_handle(m_multi_handle, curl);
	if(ret == CURLM_OK)
	{
		m_taskCount++;
		MYLOG_DEBUG(g_logger, "add task, count %d", m_taskCount);
		if(m_taskCount > 100)
		{
			MYLOG_WARN(g_logger, "async curl task overload, task count %d", m_taskCount);	
		}
	}
	else
	{
		MYLOG_ERROR(g_logger, "add async curl task failed, ret is %d", ret);	
	}
    return ret;
}

int GAsyncCurlThread::curl_multi_select(CURLM * curl_m)
{
    struct timeval timeout_tv;
    fd_set  fd_read;
    fd_set  fd_write;
    fd_set  fd_except;
    int     max_fd = -1;
    FD_ZERO(&fd_read);
    FD_ZERO(&fd_write);
    FD_ZERO(&fd_except);

    timeout_tv.tv_sec = 1;
    timeout_tv.tv_usec = 0;


    curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);

    int ret_code = select(max_fd + 1, &fd_read, &fd_write, NULL, &timeout_tv);
    switch(ret_code)
    {
    //error
    case -1:
        return ret_code;
    case 0:
        return 0;
    default:
        return 1;
    }
    return 0;
}

int GAsyncCurlThread::SendHttpReq(const char* url, callback cb, void *user_ptr, std::string& params)
{
    int ret;
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, user_ptr);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);  //disable chunked

    if(!params.empty())
    {
        char local_buffer[1024]={0};
        memcpy(local_buffer, params.c_str(), params.size());
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, params.size());
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, local_buffer);
    }

    ret = AddCurlHandle(curl);
	if(CURLM_OK != ret)
	{
        MYLOG_ERROR(g_logger, "add async curl task failed");
	}

	MYLOG_DEBUG(g_logger, "add async curl task success");
	
    return ret;
}

int GAsyncCurlThread::AddJob2Queue(Job job)
{
	IceUtil::RecMutex::Lock lock(m_job_mutex);
	m_jobs.push_back(job);
	return 0;
}

int GAsyncCurlThread::AddJob2MultiHandleStack()
{
	std::vector<Job> jobs;
	{
		IceUtil::RecMutex::Lock lock(m_job_mutex);
		if(m_jobs.empty())
		{
			return 0;
		}
		jobs.swap(m_jobs);
	}

    std::string strHost;
	for(auto it = jobs.begin(); it != jobs.end(); it++)
	{
		Job job = *it;
        if (job.type == "lbs")
        {
            lbs::LbsList oLbsList;
            if (job.pLbsMsg == NULL)
            {
                continue;
            }

            strHost = HTTP_CGI_ADAPTER_URL;
            std::string strLbsPrefix = job.pLbsMsg->mcc() + job.pLbsMsg->mnc();
            std::string strLbsId;
        	for(int idx=0; idx < job.pLbsMsg->lbsinfo_size(); ++idx)
        	{
               const YunJi::LbsMsg_LbsInfo &opbLbsArea = job.pLbsMsg->lbsinfo(idx);
               strLbsId = strLbsPrefix + opbLbsArea.lac() + opbLbsArea.ci();
        	   lbs::LbsInfo *pLbsInfo = oLbsList.add_info();
        	   pLbsInfo->set_id(strLbsId);
        	   pLbsInfo->set_strength(opbLbsArea.rssi());
        	}
        	oLbsList.set_imei(job.pLbsMsg->imei());
        	oLbsList.set_method(job.type);

        	std::string strReq;
        	oLbsList.SerializeToString(&strReq);

        	std::string strSign;
        	MakeMD5(strReq, strSign);

            strHost += strSign;
            SendHttpReq(strHost.data(), CurlLbsCallback, (void *)job.pLbsMsg, strReq);
        }
	}
	
	return 0;
}



