
#include "tostring.h"
#include "StringUtility.h"
#include "TimeTransformer.h"
#include "dev.pb.h"
#include "DispatchThread.h"

ReceiveDataList g_oReceiveList;
IceUtil::Monitor<IceUtil::Mutex> g_oReceiveListMutex;

bool ReceiveDataList::AddData(SVec &data) 
{
    {
        IceUtil::RecMutex::Lock lock(m_oMutex);
        if( m_RecvBuf.size() > g_conf.bufferSize )
        {
            MYLOG_WARN(g_logger, "SWAP FULL! RecvBuf size: %lu, buffer Size: %u", m_RecvBuf.size(), g_conf.bufferSize);
            return false;
        }

        if( !data.empty() )
        {
            m_RecvBuf.insert(m_RecvBuf.end(), data.begin(), data.end());
        }
    }

    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(g_oReceiveListMutex);
    g_oReceiveListMutex.notify();

    return true;
}

bool ReceiveDataList::AddData(const std::string &strData) 
{
    {
        IceUtil::RecMutex::Lock lock(m_oMutex);
        if( m_RecvBuf.size() > g_conf.bufferSize )
        {
            MYLOG_WARN(g_logger, "SWAP FULL! RecvBuf size: %lu, buffer Size: %u", m_RecvBuf.size(), g_conf.bufferSize);
            return false;
        }

        if( !strData.empty())
        {
            m_RecvBuf.push_back(strData);
        }
    }

    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(g_oReceiveListMutex);
    g_oReceiveListMutex.notify();

    return true;
}

CDispatchThread::CDispatchThread()
{
    m_bStop = false;
    _cassandra = NULL;
}

CDispatchThread::~CDispatchThread()
{
    stop();
}

void CDispatchThread::stop()
{
    MYLOG_INFO(g_logger,"stopping CReceiveThread.");
    m_bStop = true;
    _kafkaGpsProducer->destroy();
    _kafkaCmdProducer->destroy();
    _kafkaIccidProducer->destroy();
    _kafkaAlarmProducer->destroy();
}

void CDispatchThread::run()
{
    _cassandra = new WriteCassandra;
    if (_cassandra != NULL)
    {
        if (!_cassandra->init())
        {
            MYLOG_ERROR(g_logger,"Cassandra instance init failed.");
        }
        else
        {
            MYLOG_INFO(g_logger, "Connect to Cassandra successfully.");
        }
    }
    else
    {
        MYLOG_ERROR(g_logger,"Create cassandra instance failed.");
    }

    _kafkaGpsProducer = new ProducerKafka;
    if (0 == _kafkaGpsProducer->init_kafka(0, g_conf.kafkaBroker.data(), g_conf.kafkaGpsTopic.data()))
    {
        MYLOG_INFO(g_logger, "ProducerKafka init success. kafkaBroker=%s, kafkaGpsTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaGpsTopic.c_str());
    }
    else
    {
        MYLOG_ERROR(g_logger, "producer init failedkafkaBroker=%s, kafkaGpsTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaGpsTopic.c_str());
    }

    _kafkaCmdProducer = new ProducerKafka;
    if (0 == _kafkaCmdProducer->init_kafka(0, g_conf.kafkaBroker.data(), g_conf.kafkaCmdTopic.data()))
    {
        MYLOG_INFO(g_logger, "ProducerKafka init success. kafkaBroker=%s, kafkaCmdTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaCmdTopic.c_str());
    }
    else
    {
        MYLOG_ERROR(g_logger, "producer init failedkafkaBroker=%s, kafkaCmdTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaCmdTopic.c_str());
    }

    _kafkaIccidProducer = new ProducerKafka;
    if (0 == _kafkaIccidProducer->init_kafka(0, g_conf.kafkaBroker.data(), g_conf.kafkaIccidTopic.data()))
    {
        MYLOG_INFO(g_logger, "ProducerKafka init success. kafkaBroker=%s, kafkaIccidTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaIccidTopic.c_str());
    }
    else
    {
        MYLOG_ERROR(g_logger, "producer init failedkafkaBroker=%s, kafkaIccidTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaIccidTopic.c_str());
    }

    _kafkaAlarmProducer = new ProducerKafka;
    if (0 == _kafkaAlarmProducer->init_kafka(0, g_conf.kafkaBroker.data(), g_conf.kafkaAlarmTopic.data()))
    {
        MYLOG_INFO(g_logger, "ProducerKafka init success. kafkaBroker=%s, kafkaAlarmTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaAlarmTopic.c_str());
    }
    else
    {
        MYLOG_ERROR(g_logger, "producer init failedkafkaBroker=%s, kafkaAlarmTopic=%s", 
            g_conf.kafkaBroker.c_str(), g_conf.kafkaAlarmTopic.c_str());
    }

    MYLOG_INFO(g_logger,"Start Dispatch Thread!");
    while (!m_bStop)
    {
        try
        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(g_oReceiveListMutex);
            g_oReceiveListMutex.timedWait(IceUtil::Time::milliSeconds(g_conf.wakeupInterval));
        }
        catch (Ice::Exception &ex)
        {
            MYLOG_ERROR(g_logger, "g_oReceiveListMutex.wait() Unexpected return:%s", ex.what());
            return;
        }

        RecBuffSwap();
    }

    // send cache data to gmq
    RecBuffSwap();
    MYLOG_INFO(g_logger, "exiting CReceiveThread.");
    return;
}

bool CDispatchThread::RecBuffSwap()
{
    SVec data;
    g_oReceiveList.Swap(data);

    uint64_t t1,t2;

    // write cassandra
    if (_cassandra != NULL)
    {
        t1 = Goome::GetTimeMillisecond();
        _cassandra->process_records(data);
        t2 = Goome::GetTimeMillisecond();

        if(data.size() > 1000)
        {
            MYLOG_WARN(g_logger, "Write Cassandra Record size=%lu cost[%lu]ms", data.size(), t2-t1);
        }
    }

    //发送到kafka队列
    PushToKafka(data);
    return true;
}

bool CDispatchThread::PushToKafka(SVec &data)
{
    if (data.empty())
    {
        return true;
    }
    
    for (size_t idx=0; idx < data.size(); ++idx)
    {
        const std::string &msg = data[idx];
        YunJi::UpDevMsg oPbMsg;
        if (!oPbMsg.ParseFromString(msg))
        {
            MYLOG_WARN(g_logger, "ParseFromString failed!");
            continue;
        }

        switch(oPbMsg.msgtype())
        {
            case YunJi::kGpsMsg:
            {
                if (0 != _kafkaGpsProducer->push_data_to_kafka(msg.data(), msg.size()))
                {
                    MYLOG_WARN(g_logger, "kafka push Gps data failed %s", oPbMsg.DebugString().c_str());
                }
                else
                {
                    MYLOG_DEBUG(g_logger, "kafka push Gps data success %s", oPbMsg.DebugString().c_str());
                }
                break;
            }
            case YunJi::kCommandRsp:
            {
                if (0 != _kafkaCmdProducer->push_data_to_kafka(msg.data(), msg.size()))
                {
                    MYLOG_WARN(g_logger, "kafka push CmddRsp data failed %s", oPbMsg.DebugString().c_str());
                }
                break;
            }
            case YunJi::kIccidMsg:
            {
                if (0 != _kafkaIccidProducer->push_data_to_kafka(msg.data(), msg.size()))
                {
                    MYLOG_WARN(g_logger, "kafka push Iccid data failed %s", oPbMsg.DebugString().c_str());
                }
                break;
            }
             case YunJi::kAlarmMsg:
            {
                if (0 != _kafkaAlarmProducer->push_data_to_kafka(msg.data(), msg.size()))
                {
                    MYLOG_WARN(g_logger, "kafka push Alarm data failed %s", oPbMsg.DebugString().c_str());
                }
                else
                {
                    MYLOG_DEBUG(g_logger, "kafka push Alarm data success %s", oPbMsg.DebugString().c_str());
                }
                break;
            }
            default:
                MYLOG_WARN(g_logger, "not need push to kafka. msg:%s", oPbMsg.DebugString().c_str());
                break;
        }
    }

    return true;
}



