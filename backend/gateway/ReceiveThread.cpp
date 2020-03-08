#include "tostring.h"
#include "StringUtility.h"
#include "TimeTransformer.h"
#include "dev.pb.h"
#include "common.h"
#include "t808Parser.h"
#include "GWReaderThread.h"
#include "ReceiveThread.h"

static int is_printable (const char *buf, size_t size)
{
	size_t i;
	for (i = 0 ; i < size ; i++)
	{
		if (!isprint((int)buf[i]))
		{
			return 0;
		}
	}
	return 1;
}


CReceiveThread::CReceiveThread()
{
    m_bStop = false;
    _rk = NULL;
}

CReceiveThread::~CReceiveThread()
{
    stop();
}

void CReceiveThread::stop()
{
    MYLOG_INFO(g_logger,"stopping CReceiveThread.");
    m_bStop = true;

    destroy();
}

void CReceiveThread::run()
{
    MYLOG_INFO(g_logger,"Start Receive Thread!");

    int timeout_ms = 100;
    while (!m_bStop)
    {
        if (!init_kafka())
        {
            sleep(1);
            continue;
        }

        rd_kafka_message_t *rkm = NULL;
		rkm = rd_kafka_consumer_poll(_rk, timeout_ms);
		if (!rkm)
		{
			continue;
		}

		/* consumer_poll() will return either a proper message
		 * or a consumer error (rkm->err is set). */
		if (rkm->err)
		{
			/* Consumer errors are generally to be considered
			 * informational as the consumer will automatically
			 * try to recover from all types of errors. */
			MYLOG_WARN(g_logger, "Consumer error: %s", rd_kafka_message_errstr(rkm));
			rd_kafka_message_destroy(rkm);
			continue;
		}

        consumer(rkm);
        rd_kafka_message_destroy(rkm);
    }

    MYLOG_INFO(g_logger, "exiting CReceiveThread.");
    return;
}

bool CReceiveThread::consumer(rd_kafka_message_t *rkm)
{
    std::string strTopicName(rd_kafka_topic_name(rkm->rkt));
    /* Proper message. */
	MYLOG_DEBUG(g_logger, "Message on %s [%d] at offset %lu:",
	   strTopicName.c_str(), rkm->partition, rkm->offset);

	/* Print the message key. */
	if (rkm->key)
	{
        if (is_printable((const char *)rkm->key, rkm->key_len))
        {
		    MYLOG_DEBUG(g_logger, " Key: %.*s", (int)rkm->key_len, (const char *)rkm->key);
        }
        else
        {
            MYLOG_DEBUG(g_logger, " Key: (%d bytes)", (int)rkm->key_len);
        }
	}

	/* Print the message value/payload. */
	if (rkm->payload)
	{
        if (is_printable((const char *)rkm->payload, rkm->len))
        {
		    MYLOG_DEBUG(g_logger, " Value: %.*s", (int)rkm->len, (const char *)rkm->payload);
        }
        else
        {
            MYLOG_DEBUG(g_logger, " Value: (%d bytes)", (int)rkm->len);
        }

        std::string strPayload((const char *)rkm->payload, rkm->len);
        if (0 == strTopicName.compare("cmd.req"))
        {
            consumeCmdReqMsg(strPayload);
        }
        else if (0 == strTopicName.compare("gw.gps"))
        {
            consumeGpsMsg(strPayload);
        }
        else if (0 == strTopicName.compare("gw.cmdrsp"))
        {
            consumeCmdRspMsg(strPayload);
        }
        else
        {
            ;
        }
	}

    return true;
}

bool CReceiveThread::init_kafka()
{
    if (_rk)
    {
        return true;
    }

    if (g_conf.kafkaConsumerTopics.empty())
    {
        //没有需要消费的topic
        return false;
    }

    std::vector<std::string> vecTopics;
    Goome::SplitString(g_conf.kafkaConsumerTopics, ",", vecTopics);

    //rd_kafka_t *rk;          /* Consumer instance handle */
	rd_kafka_conf_t *conf;   /* Temporary configuration object */
	rd_kafka_resp_err_t err; /* librdkafka API error code */
	char errstr[512];        /* librdkafka API error reporting buffer */
	//const char *brokers;     /* Argument: broker list */
	//const char *groupid;     /* Argument: Consumer group id */
	//char **topics;           /* Argument: list of topics to subscribe to */
	//int topic_cnt;           /* Number of topics to subscribe to */
	rd_kafka_topic_partition_list_t *subscription; /* Subscribed topics */
	//int i;

	/*
	 * Create Kafka client configuration place-holder
	 */
	conf = rd_kafka_conf_new();

	/* Set bootstrap broker(s) as a comma-separated list of
	 * host or host:port (default port 9092).
	 * librdkafka will use the bootstrap brokers to acquire the full
	 * set of brokers from the cluster. */
	if (rd_kafka_conf_set(conf, "bootstrap.servers", g_conf.kafkaBroker.data(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
	{
		MYLOG_WARN(g_logger, "rd_kafka_conf_set failed! errstr: %s", errstr);
		rd_kafka_conf_destroy(conf);
		return false;
	}

	/* Set the consumer group id.
	 * All consumers sharing the same group id will join the same
	 * group, and the subscribed topic' partitions will be assigned
	 * according to the partition.assignment.strategy
	 * (consumer config property) to the consumers in the group. */
	if (rd_kafka_conf_set(conf, "group.id", g_conf.kafkaGroupId.data(), errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
	{
		MYLOG_WARN(g_logger, "rd_kafka_conf_set failed! errstr: %s", errstr);
		rd_kafka_conf_destroy(conf);
		return false;
	}

	/* If there is no previously committed offset for a partition
	 * the auto.offset.reset strategy will be used to decide where
	 * in the partition to start fetching messages.
	 * By setting this to earliest the consumer will read all messages
	 * in the partition if there was no previously committed offset. */
	if (rd_kafka_conf_set(conf, "auto.offset.reset", "earliest", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
	{
		MYLOG_WARN(g_logger, "rd_kafka_conf_set failed! errstr: %s", errstr);
		rd_kafka_conf_destroy(conf);
		return false;
	}

	/*
	 * Create consumer instance.
	 *
	 * NOTE: rd_kafka_new() takes ownership of the conf object
	 *       and the application must not reference it again after
	 *       this call.
	 */
	_rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
	if (!_rk)
	{
		MYLOG_WARN(g_logger, "Failed to create new consumer: %s", errstr);
		return false;
	}

	conf = NULL; /* Configuration object is now owned, and freed,
				  * by the rd_kafka_t instance. */


	/* Redirect all messages from per-partition queues to
	 * the main queue so that messages can be consumed with one
	 * call from all assigned partitions.
	 *
	 * The alternative is to poll the main queue (for events)
	 * and each partition queue separately, which requires setting
	 * up a rebalance callback and keeping track of the assignment:
	 * but that is more complex and typically not recommended. */
	rd_kafka_poll_set_consumer(_rk);


	/* Convert the list of topics to a format suitable for librdkafka */
    int topic_cnt = vecTopics.size();
	subscription = rd_kafka_topic_partition_list_new(topic_cnt);
	for (int i = 0 ; i < topic_cnt ; i++)
	{
		rd_kafka_topic_partition_list_add(subscription,
										  vecTopics[i].data(),
										  /* the partition is ignored
										   * by subscribe() */
										  RD_KAFKA_PARTITION_UA);
	}

	/* Subscribe to the list of topics */
	err = rd_kafka_subscribe(_rk, subscription);
	if (err)
	{
		MYLOG_WARN(g_logger, "Failed to subscribe to %d topics: %s",
				subscription->cnt, rd_kafka_err2str(err));
		rd_kafka_topic_partition_list_destroy(subscription);
		rd_kafka_destroy(_rk);
        _rk = NULL;
		return false;
	}

	MYLOG_INFO(g_logger, "Subscribed to %d topics(%s), "
			"waiting for rebalance and messages...\n",
			subscription->cnt, g_conf.kafkaConsumerTopics.c_str());

	rd_kafka_topic_partition_list_destroy(subscription);
    return true;
}

void CReceiveThread::destroy()
{
	/* Close the consumer: commit final offsets and leave the group. */
	MYLOG_INFO(g_logger, "Closing consumer");
	rd_kafka_consumer_close(_rk);

	/* Destroy the consumer */
	rd_kafka_destroy(_rk);
}

bool CReceiveThread::consumeCmdReqMsg(const std::string &msg)
{
    if (msg.empty())
    {
        return true;
    }

    YunJi::DownDevMsg oPbMsg;
    if (!oPbMsg.ParseFromString(msg))
    {
        MYLOG_WARN(g_logger, "ParseFromString failed!");
        return false;
    }

    MYLOG_DEBUG(g_logger, "command req DebugString :%s", oPbMsg.DebugString().c_str());
    const YunJi::CommandReq &oPbCmdReq = oPbMsg.cmdreq();
    std::string strCmdMsg = T808Parser::CommandReqMsg(oPbCmdReq.imei(), oPbCmdReq.content());
    DebugBinaryStr(strCmdMsg.data(), strCmdMsg.length());

    GWReader::SendMsgToClient(oPbCmdReq.id(), strCmdMsg);
    return true;
}

bool CReceiveThread::consumeGpsMsg(const std::string &msg)
{
    if (msg.empty())
    {
        return true;
    }

    YunJi::UpDevMsg oPbMsg;
    if (!oPbMsg.ParseFromString(msg))
    {
        MYLOG_WARN(g_logger, "ParseFromString failed!");
        return false;
    }

    MYLOG_DEBUG(g_logger, "Gps DebugString :%s", oPbMsg.DebugString().c_str());
    return true;
}

bool CReceiveThread::consumeCmdRspMsg(const std::string &msg)
{
    if (msg.empty())
    {
        return true;
    }

    YunJi::UpDevMsg oPbMsg;
    if (!oPbMsg.ParseFromString(msg))
    {
        MYLOG_WARN(g_logger, "ParseFromString failed!");
        return false;
    }

    MYLOG_DEBUG(g_logger, "Command Rsp DebugString :%s", oPbMsg.DebugString().c_str());
    return true;
}




