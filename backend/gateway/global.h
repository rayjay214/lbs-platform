
/*
 * global.h
 *
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>
#include <inttypes.h>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <math.h>
#include "tbb/concurrent_hash_map.h"
#include "Log.h"

#define hton16(A) ((((uint16_t)(A) & 0xff00) >> 8) | \
                    (((uint16_t)(A) & 0x00ff) << 8))
#define hton32(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
                    (((uint32_t)(A) & 0x00ff0000) >> 8)  | \
                    (((uint32_t)(A) & 0x0000ff00) << 8)  | \
                    (((uint32_t)(A) & 0x000000ff) << 24))
#define hton64(A) (((((uint64_t)A)<<56) & 0xFF00000000000000ULL)  | \
                    ((((uint64_t)A)<<40) & 0x00FF000000000000ULL)  | \
                    ((((uint64_t)A)<<24) & 0x0000FF0000000000ULL)  | \
                    ((((uint64_t)A)<< 8) & 0x000000FF00000000ULL)  | \
                    ((((uint64_t)A)>> 8) & 0x00000000FF000000ULL)  | \
                    ((((uint64_t)A)>>24) & 0x0000000000FF0000ULL)  | \
                    ((((uint64_t)A)>>40) & 0x000000000000FF00ULL)  | \
                    ((((uint64_t)A)>>56) & 0x00000000000000FFULL))

#define ntoh16  hton16
#define ntoh32  hton32
#define ntoh64  hton64

#define SETBIT(x,y) ((x)|=(1<<(y)))
#define CLRBIT(x,y) ((x)&=~(1<<(y)))
#define GETBIT(x,y) ((x)&(1<<(y)))

#define CONF_FILE_PATH          "../etc/gpp.conf"
#define LOG4CXX_CFGFILE         "../etc/log4cxx.cfg"

#define MAX_CONN_COUNT  100000
#define GPP_IMEI_LEN 8
#define IMEI_STR_LEN 15
#define CONN_BUFFER_INIT_TAIL 8
#define CONN_BUFFER_LENGTH  (512)

typedef std::vector<std::string> SVec;
typedef std::map<std::string, std::string> SSMap;
typedef std::map<std::string, int>     SIMap;
typedef std::map<std::string, SVec>    SSVecMap;
typedef std::map<std::string, SSMap>   SSSMap;
typedef std::map<std::string, SIMap>   SSIMap;
typedef std::map<std::string, SSIMap>  SSSIMap;

enum error_code_e
{
    ERR_OK = 0,
    ERR_BUFFER_EMPTY,
    ERR_TABLE_NOT_FOUND,
    ERR_WRITE_FILE_FAILED,
    ERR_CONNECT_DB,
    ERR_FIELD_UNKNOW,
    ERR_TABLE_NOT_EXIST,
    ERR_CONNECT_LOST,
    ERR_NOT_PROCESSED,
    ERR_LOCK_WAIT_TIMEOUT,
    TRANSFORM0X7E_ERROR,
    PACKET_INCOMPLETE,
};

enum E_PROTOCOL_NO
{
	PROTOCOL_INVALID = 0,
	PROTOCOL_T808 = 1
};


typedef struct _CONN_BUFFER
{
    uint8_t  buffer[CONN_BUFFER_LENGTH];
    uint32_t tail;
    uint64_t id;
    time_t access_time;
    uint32_t ip;
    int readerId;
    uint8_t used;
}CONN_BUFFER;

typedef struct _ID_INFO
{
	int cfd;
} __attribute__((packed)) ID_INFO;

template<typename K>
struct HashCompare
{
	static size_t hash(const K &key)
	{
		return key%100000000;
	}
	static bool equal(const K &key1, const K &key2)
	{
		return ( key1 == key2 );
	}
};

typedef tbb::concurrent_hash_map<uint64_t, ID_INFO,HashCompare<uint64_t> > ID_TAB;


//配置
typedef struct _conf
{
	std::string caller;
	std::string redis_callee;
	std::string cassandra_hosts;

	uint32_t    wakeupInterval;
	uint32_t    bufferSize;

	int reader_thread_num;
	std::string udp_listen_port;
	std::vector<std::string> tcp_listen_ports;

	std::string kafkaGpsTopic;
	std::string kafkaCmdTopic;
	std::string kafkaIccidTopic;
	std::string kafkaAlarmTopic;
	std::string kafkaConsumerTopics;
    std::string kafkaBroker;
	std::string kafkaGroupId;
}G_CONF;

extern G_CONF               g_conf;
extern log4cxx::LoggerPtr g_logger;

#endif /* GLOBAL_H_ */

