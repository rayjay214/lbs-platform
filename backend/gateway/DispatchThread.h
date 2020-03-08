#ifndef _DISPATCH_THREAD_H_
#define _DISPATCH_THREAD_H_

#include <vector>
#include <Ice/Ice.h> 
#include <IceUtil/Thread.h>
#include <IceUtil/Monitor.h>
#include <IceUtil/IceUtil.h>
#include "global.h"
#include "WriteCassandra.h"
#include "kafkaOp.h"
#include <librdkafka/rdkafka.h>

class ReceiveDataList
{
public:
    bool AddData(SVec &str);
	bool AddData(const std::string &strData);

    inline bool Swap(SVec &table)
    {
        IceUtil::RecMutex::Lock l(m_oMutex);
        table.swap(m_RecvBuf);
        return true;
    }

    uint64_t GetBuffTotal()
    {
        IceUtil::RecMutex::Lock l(m_oMutex);
        return m_RecvBuf.size();
    }

private:
    SVec m_RecvBuf;
    IceUtil::RecMutex m_oMutex;
};

class CDispatchThread:  public IceUtil::Thread
{
public:
    CDispatchThread();
    virtual ~CDispatchThread();
    virtual void run();
    void stop();
    bool RecBuffSwap();
	bool PushToKafka(SVec &data);
	
private:
    bool m_bStop;
    WriteCassandra * _cassandra;
	ProducerKafka* _kafkaGpsProducer;
	ProducerKafka* _kafkaCmdProducer;
	ProducerKafka* _kafkaIccidProducer;
	ProducerKafka* _kafkaAlarmProducer;
};

typedef IceUtil::Handle<CDispatchThread> CDispatchThreadPtr;

#endif

