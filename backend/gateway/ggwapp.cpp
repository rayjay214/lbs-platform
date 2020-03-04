#include <Ice/Service.h>
#include "global.h"
#include "inet_sockets.h"
#include "StringUtility.h"
#include "RedisOp.h"
#include "ggwapp.h"

G_CONF g_conf;
string g_cassandra_hosts = "127.0.0.1";
log4cxx::LoggerPtr g_logger = log4cxx::Logger::getRootLogger();

bool GGWApp::start(int argc, char* argv[], int& status)
{
    m_ic = communicator();
    Ice::PropertiesPtr props;
    props = m_ic->getProperties();

    g_logger = MYLOG_INIT("../etc/log4cxx.conf", "", (CLog::ELOG_LEVEL) CLog::EINFO);
    if (NULL == g_logger)
    {
        printf("MYLOG_INIT  error!");
        status = EXIT_FAILURE;
        return false;
    }

    // 读取配置文件
    g_conf.caller = props->getPropertyWithDefault("GnsCaller", "FOTA_GW");
    g_conf.redis_callee = props->getPropertyWithDefault("RedisGnsCallee", "FOTA_REDIS");
    std::string strTcpPorts = props->getPropertyWithDefault("GwTcpPorts", "8821|8841");
    Goome::SplitString(strTcpPorts, "|", g_conf.tcp_listen_ports);

    g_cassandra_hosts = props->getPropertyWithDefault("cassandra_hosts", "127.0.0.1");
    g_conf.wakeupInterval = props->getPropertyAsIntWithDefault("WakeupInterval", 50);
    g_conf.bufferSize = props->getPropertyAsIntWithDefault("RecvBufferSize", 1000);

    g_conf.kafkaBroker = props->getPropertyWithDefault("kafkaBroker", "127.0.0.1:9092");
    g_conf.kafkaGpsTopic = props->getPropertyWithDefault("kafkaGpsTopic", "gw.gps");
    g_conf.kafkaCmdTopic = props->getPropertyWithDefault("kafkaCmdTopic", "gw.cmdrsp");
    g_conf.kafkaIccidTopic = props->getPropertyWithDefault("kafkaIccidTopic", "gw.iccid");
    g_conf.kafkaAlarmTopic = props->getPropertyWithDefault("kafkaAlarmTopic", "gw.alarm");
    g_conf.kafkaConsumerTopics = props->getPropertyWithDefault("kafkaConsumerTopics", "gw.cmdrsp,gw.gps,cmd.req");
    g_conf.kafkaGroupId = props->getPropertyWithDefault("kafkaGroupId", "gw");

    // 设置GNS名称
    RedisOp::SetMyGnsName(g_conf.caller);
    RedisOp::SetStatusRedisGnsNameW(g_conf.redis_callee);

    MYLOG_DEBUG(g_logger, "RedisOp init. caller=%s, redis_callee=%s", 
        g_conf.caller.c_str(), g_conf.redis_callee.c_str());

    // 绑定TCP端口
    for (auto iter = g_conf.tcp_listen_ports.begin(); iter != g_conf.tcp_listen_ports.end(); ++iter)
    {
        //create listening port
        int listenfd = inetListen(iter->c_str(), 10000, NULL);
        if (listenfd == -1)
        {
            MYLOG_FATAL(g_logger, "Error inetListen listenfd: port=%d strerrno=%s", atoi(iter->c_str()), strerror(errno));
            return false;
        }
        if (set_nonblock(listenfd) == -1)
        {
            MYLOG_FATAL(g_logger, "Error set listenfd nonblock: port=%d strerrno=%s", atoi(iter->c_str()), strerror(errno));
            return false;
        }
        _listerfds.push_back(listenfd);
        MYLOG_INFO(g_logger, "start Listen on port=%s for tcp: fd = %d", iter->c_str(), listenfd);
    }

    _dispatchThread = new CDispatchThread();
    if (NULL == _dispatchThread)
    {
        MYLOG_ERROR(g_logger,"init CDispatchThread failed");
    }
    _dispatchThread->start();
    MYLOG_INFO(g_logger,"create CDispatchThread success.");

    _receiveThread = new CReceiveThread();
    if (NULL == _receiveThread)
    {
        MYLOG_ERROR(g_logger,"init CReceiveThread failed");
    }
    _receiveThread->start();
    MYLOG_INFO(g_logger,"create CReceiveThread success.");

    _asyncCurlThread = new GAsyncCurlThread();
    if (NULL == _asyncCurlThread)
    {
        MYLOG_ERROR(g_logger,"init GAsyncCurlThread failed");
    }
    _asyncCurlThread->init();
    _asyncCurlThread->start();
    MYLOG_INFO(g_logger,"create GAsyncCurlThread success.");

    // 启动读线程（监听端口）
    char readerNameBuf[64];
    std::string readerName;
    _readerThread = new GWReader(0, _listerfds);
    sprintf(readerNameBuf, "[Thread-GWReader %d]", 0);
    readerName = readerNameBuf;
    _readerThread->setThreadName(readerName);
    _readerThread->setExpir(7 * 60);
    _readerThread->start();
    MYLOG_INFO(g_logger, "Create Reader %d success.", 0);

    return true;
}


void GGWApp::interrupt()
{
    MYLOG_INFO(g_logger, "Stop readerThread");
    _readerThread->stop();

    MYLOG_INFO(g_logger, "Stop CDispatchThread");
    _dispatchThread->stop();

    MYLOG_INFO(g_logger, "Stop CReceiveThread");
    _receiveThread->stop();

    MYLOG_INFO(g_logger, "Stop GAsyncCurlThread");
    _asyncCurlThread->stop();

    while (_readerThread->isAlive())
    {
        MYLOG_ERROR(g_logger, "Wait for read data was flushed ...");
        sleep(1);
    }

    while (_dispatchThread->isAlive())
    {
        MYLOG_ERROR(g_logger, "Wait CDispatchThread was stop ...");
        sleep(1);
    }

    while (_receiveThread->isAlive())
    {
        MYLOG_ERROR(g_logger, "Wait CReceiveThread was stop ...");
        sleep(1);
    }
    
    while (_asyncCurlThread->isAlive())
    {
        MYLOG_ERROR(g_logger, "Wait GAsyncCurlThread was stop ...");
        sleep(1);
    }

    MYLOG_INFO(g_logger, "Shutdown communicator");
    Ice::Service::interrupt();
}
