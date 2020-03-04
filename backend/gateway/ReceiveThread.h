#ifndef _RECEIVETHREAD_H_
#define _RECEIVETHREAD_H_

#include <vector>
#include <Ice/Ice.h> 
#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/IceUtil.h>
#include <librdkafka/rdkafka.h>
//#include <librdkafka/rdkafkacpp.h>
#include "global.h"


class CReceiveThread:  public IceUtil::Thread
{
public:
    CReceiveThread();
    ~CReceiveThread();
    virtual void run();
    void stop();

private:
    bool init_kafka();
    void destroy();
    bool consumer(rd_kafka_message_t *rkm);
	bool consumeCmdReqMsg(const std::string &msg);
	bool consumeGpsMsg(const std::string &msg);
	bool consumeCmdRspMsg(const std::string &msg);

private:
    bool m_bStop;
	rd_kafka_t *_rk;
};

typedef IceUtil::Handle<CReceiveThread> CReceiveThreadPtr;
#endif

