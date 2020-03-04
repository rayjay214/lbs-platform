
#ifndef GOOME_ZK_BASE_API_H_
#define GOOME_ZK_BASE_API_H_

#include "zookeeper.h"
#include "zookeeper.jute.h"
#include <string>
#include <stdio.h>


// 参考： Zookeeper开发常见问题 tech.uc.cn/?p=1189
// www.cnblogs.com/haippy/archive/2013/02/21/2919365.html


#define NODE_INDEX_SEPARATOR    ":"     // 临时节点中前缀与序号的分隔符

// 自定义错误类型，是ZOOKEEPER官方API里ZOO_ERRORS的补充
enum ZkApiError
{
    // 0到-199为zookeeper本身预留
    ZKAPI_ERR_BEGIN = -200,
    ZKAPI_ERR_UNINITED = -201,              // API未初始化
    ZKAPI_ERR_CONVERSATION_UNSETUP = -202,  // 会话还未建立
    ZKAPI_ERR_GNS_FAIL = -203,              // GNS API操作失败
    ZKAPI_ERR_OP_OVERTIME = -204,           // 操作超时
    ZKAPI_ERR_AUTH_PARAM = -205,            //错误的鉴权信息
    ZKAPI_ERR_NO_HOSTS = -206,              //未初始化Hosts信息
    ZKAPI_ERR_END = -300
};

// 封装Zookeeper官方提供的API，提供elector和observer两种使用者的公共操作。
class ZkBaseApi
{
public:
    ZkBaseApi();

    ~ZkBaseApi();

    // 设置Zookeeper的ip、port列表，在第一次InitZookeeper前，必须调用该函数
    virtual void SetHosts(const char * pHosts);

    // 初始化ZK连接句柄并连接目标主机
    virtual int InitZookeeper();

    virtual int CloseHandle();

    // 当Zookeeper session过期后，重新连上时要做的处理
    virtual int ReconnectDeal() = 0;

    // 设置ZK超时时间
    virtual void SetTimeout(int t);

    // 设置ZK日志级别
    virtual void SetLogLevel(int level);

    // 设置ZK日志流
    virtual void SetLogStream(FILE * fp);

    // 获取错误码
    virtual int GetErrorCode();

    // 根据错误类型获取错误描述
    static const char * GetErrorDesc(int e);
public:
    zhandle_t* m_pHandle;   // ZK连接句柄

    int m_dwErrCode;        // 错误码

protected:
    virtual int Uninit();
protected:
    std::string m_strHosts; // Zookeeper的ip、port列表
    int m_dwTimeout;    // 与ZK连接的超时时间，单位为ms。取值在4s-40s之间。
    int m_dwLogLevel;   // ZK日志级别
    FILE * m_pLogFile;  // ZK日志输出句柄，为NULL则表示使用标准出错。
};


namespace Goome
{
    // Function: 根据节点全路径，获取节点名
    // Input: buf     节点全路径
    // Output: node   节点名
    void GetZkNodeName(const char *buf, char *node);

    // Function: 根据指定的节点名及一个节点名集合，判断该节点是不是集合中拥有相同前缀的节点里序号最小的。（拥有最小的序号一般会认为是Master）
    // Input: strMyNodeName     待判断的节点名
    // Input: oNodeList         用于比较的节点名集合
    // Return:  如果该节点的序号在集合中是最小的，则返回true，否则返回false
    bool IsMinimalZkNode(const std::string & strMyNodeName, const struct String_vector & oNodeList);
}


#endif /*GOOME_ZK_BASE_API_H_*/
