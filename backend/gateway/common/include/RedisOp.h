#ifndef __REDIS_COMMON_OP_H__
#define __REDIS_COMMON_OP_H__

#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>

#define EIGHT_HOUR_SECONDS		28800

class RedisOp
{
public:
    // Function: ���ñ�ģ���GNS���������ڳ����ʼ��ʱ�͵��á�
    static void SetMyGnsName(const std::string & strMyGns)
    {
        m_strMyGns = strMyGns;
    }

    // Function: ���ð�����Redisģ���GNS���������ڳ����ʼ��ʱ�͵��á�
    static void SetGpsboxRedisGnsName(const std::string & strRedisGns)
    {
        m_strGpsboxRedisGns = strRedisGns;
    }
    // Function: ���ð�����Redisģ���GNS���������ڳ����ʼ��ʱ�͵��á�
    static void SetAlarmRedisGnsName(const std::string & strRedisGns)
    {
        m_strAlarmRedisGns = strRedisGns;
    }
	// Function: ״̬���ֻ��Redisģ���GNS���������ڳ����ʼ��ʱ�͵��á�
    static void SetStatusRedisGnsNameR(const std::string & strRedisGns)
    {
        m_strStatusRedisGns_R = strRedisGns;
    }
	// Function: ״̬��ض�дRedisģ���GNS���������ڳ����ʼ��ʱ�͵��á�
    static void SetStatusRedisGnsNameW(const std::string & strRedisGns)
    {
        m_strStatusRedisGns_W = strRedisGns;
    }
    static void SetMaxRedisConnections(const int & max_conn)
    {
        m_dwMaxRedisConnections = max_conn;
    }

    // ��ȡ������Ϣ
    static std::string GetErrorMsg()
    {
        return std::string(m_szErrorMsg);
    }

	//��ȡredis cli��� "gpsbox" "alarm" "status_r" "status_w"
	static void* GetRedisClient(const std::string &strRedisObj, int shard_id=0, int grey_id=0);

protected:
    static std::string m_strMyGns;
    static std::string m_strGpsboxRedisGns; // ��GNS��ָֻ��gpsbox���ݿ��Ӧ��t_user/t_enterprise������Redis������õ�������Redis�������һ����������
    static std::string m_strAlarmRedisGns;
	static std::string m_strStatusRedisGns_R;  // ֻ��״̬redis���޶�Ӧ���ݱ�
	static std::string m_strStatusRedisGns_W;  // ��д״̬redis, �޶�Ӧ���ݱ�
    static int m_dwMaxRedisConnections;

    static char m_szErrorMsg[1024];
private:
};

#endif
