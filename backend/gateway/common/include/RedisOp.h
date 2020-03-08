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
    // Function: 设置本模块的GNS名，建议在程序初始化时就调用。
    static void SetMyGnsName(const std::string & strMyGns)
    {
        m_strMyGns = strMyGns;
    }

    // Function: 设置爱车安Redis模块的GNS名，建议在程序初始化时就调用。
    static void SetGpsboxRedisGnsName(const std::string & strRedisGns)
    {
        m_strGpsboxRedisGns = strRedisGns;
    }
    // Function: 设置爱车安Redis模块的GNS名，建议在程序初始化时就调用。
    static void SetAlarmRedisGnsName(const std::string & strRedisGns)
    {
        m_strAlarmRedisGns = strRedisGns;
    }
	// Function: 状态相关只读Redis模块的GNS名，建议在程序初始化时就调用。
    static void SetStatusRedisGnsNameR(const std::string & strRedisGns)
    {
        m_strStatusRedisGns_R = strRedisGns;
    }
	// Function: 状态相关读写Redis模块的GNS名，建议在程序初始化时就调用。
    static void SetStatusRedisGnsNameW(const std::string & strRedisGns)
    {
        m_strStatusRedisGns_W = strRedisGns;
    }
    static void SetMaxRedisConnections(const int & max_conn)
    {
        m_dwMaxRedisConnections = max_conn;
    }

    // 获取出错信息
    static std::string GetErrorMsg()
    {
        return std::string(m_szErrorMsg);
    }

	//获取redis cli句柄 "gpsbox" "alarm" "status_r" "status_w"
	static void* GetRedisClient(const std::string &strRedisObj, int shard_id=0, int grey_id=0);

protected:
    static std::string m_strMyGns;
    static std::string m_strGpsboxRedisGns; // 本GNS名只指向gpsbox数据库对应的t_user/t_enterprise等数据Redis，如果用到其他的Redis，请添加一个变量名。
    static std::string m_strAlarmRedisGns;
	static std::string m_strStatusRedisGns_R;  // 只读状态redis，无对应数据表
	static std::string m_strStatusRedisGns_W;  // 读写状态redis, 无对应数据表
    static int m_dwMaxRedisConnections;

    static char m_szErrorMsg[1024];
private:
};

#endif
