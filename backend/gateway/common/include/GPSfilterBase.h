#ifndef _GPS_FILTER_BASE_H_
#define _GPS_FILTER_BASE_H_

#include <string>
#include <vector>
#include <cstring>
#include <stdint.h>
#include <Ice/Ice.h>

#define USE_LITTLE_ENDIAN

// 实时GPS数据格式，与GMC的GPSVO_V1结构相同
typedef struct _GPS_STU
{
    int     iHead;          // 头部位，标志接下来的字段是否有效（参照GMC）

    ////////////////////////////////////////////////////////////////////////

    uint64_t uid;           // 设备ID
    uint64_t dateTime;      // 定位时间
    uint64_t sysTime;       // 系统时间
    uint64_t heartTime;     // 心跳时间
    int32_t longitude;      // 经度
    int32_t latitude;       // 纬度
    int32_t speed;          // 速度
    int32_t route;          // 航向
    int32_t seq;            // 序号
    uint64_t sid;           //
    char endPoint[32];      // 
    char status[16];        //
}__attribute__((packed)) GPS_STU;

// 历史GPS信息，与GFS中获取的数据格式相同
struct GPS_FROM_GFS
{
    uint64_t uid;
    uint64_t reportTime;
    uint64_t sysTime;
    uint32_t longitude;
    uint32_t latitude;
    uint32_t speed;
    uint32_t direction;
    uint32_t sequenceNo;
    uint8_t protocolNo[4];
    uint8_t status[16];
}__attribute__((packed));


// 日志分析模块接口类，只供继承用。
class GpsFilterBase {
public:
    GpsFilterBase()
    {
        memset(m_szErrorMsg, 0, sizeof(m_szErrorMsg));
        memset(m_szModuleName, 0, sizeof(m_szModuleName));
        m_ListenPort = 0;
    }

    void SetListenPort(unsigned short listenport)
    {
        m_ListenPort = listenport;
    }
    
    unsigned short GetListenPort()
    {
        return m_ListenPort;
    }

    void SetICECommunicator(::Ice::CommunicatorPtr ic, ::Ice::ObjectAdapterPtr adapter)
    {
        m_pIC = ic;
        m_pAdapter = adapter;
    }

    virtual ~GpsFilterBase() {}

    // 函数功能：   初始化本模块
    // 返回值：     若初始化成功，返回0，否则返回-1
    virtual int Init()
    {
        return 0;
    }

    // 函数功能：   反初始化本模块
    // 返回值：     若反初始化成功，返回0，否则返回-1
    virtual int Uninit()
    {
        return 0;
    }

    // 函数功能：   获取模块名称，供外部模块输出
    // 返回值：     模块名称字符串指针
    virtual const char * GetModuleName()
    {
        return m_szModuleName;
    }

    // 函数功能：   获取错误消息，供外部模块输出
    // 返回值：     错误消息字符串指针
    virtual const char * GetLastError()
    {
        return m_szErrorMsg;
    }

    // 函数功能：   预处理一批数据（可以是GPS数据也可以是配置变更等信息）。由动态库根据strSource或strDestination信息自行辨别如何处理
    // 输入参数：strSource       这批数据的来源
    // 输入参数：strDestination  这批数据的目的地
    // 输入/输出参数：   data        待预处理的数据
    // 返回值：     若预处理成功，返回0，否则返回-1
    virtual int PredealData(const std::string & strSource, const std::string & strDestination, std::vector<std::string > & data)
    {
        return 0;
    }

    // 函数功能：   定期通知下层插件
    // 输入/输出参数：   duration   上次调用notify到本次调用的时间间隔（毫秒）
    // 返回值：     若OK返回0，否则返回-1
    virtual int Notify(long duration)
    {
        return 0;
    }

    // 函数功能：   处理一批GPS数据（可以是GPS数据也可以是配置变更等信息）。由动态库根据strSource或strDestination信息自行辨别如何处理
    // 输入参数：strSource       这批数据的来源
    // 输入参数：strDestination  这批数据的目的地
    // 输入参数：   data        待处理的数据
    // 返回值：     若操作成功，返回0，返回其他认为是失败，错误原因保存在ErrorMsg成员变量中
    virtual int ProcessData(const std::string & strSource, const std::string & strDestination, const std::vector<std::string > & data) = 0;

protected:
    ::Ice::CommunicatorPtr m_pIC;
    ::Ice::ObjectAdapterPtr m_pAdapter;

    char m_szErrorMsg[1024];        // 保存错误消息

    char m_szModuleName[128];       // 模块名称

    unsigned short m_ListenPort;
};


// the types of the class factories
#ifdef __cplusplus
extern "C" {
#endif
    typedef GpsFilterBase* create_t();
    typedef void destroy_t(GpsFilterBase*);

#ifdef __cplusplus
}
#endif



#endif  // _GPS_FILTER_BASE_H_


