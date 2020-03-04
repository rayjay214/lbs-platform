
#ifndef GOOME_MS_INTERFACE_H_
#define GOOME_MS_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <map>
#include <set>
#include "ms_observer.h"

namespace Goome
{
    // 当多个服务拥有同样的GNS时，使用该结构的“分片号+灰度号”属性归类。
    struct GnsSvrAttribute
    {
        int dwShardID;
        int dwGreyID;
        GnsSvrAttribute():dwShardID(0), dwGreyID()
        {
        }
        bool operator<(const GnsSvrAttribute& other) const
        {
            return (dwShardID == other.dwShardID) ? (dwGreyID < other.dwGreyID) : (dwShardID < other.dwShardID);
        }
        bool operator==(const GnsSvrAttribute& other) const
        {
            return ((dwShardID == other.dwShardID) && (dwGreyID == other.dwGreyID));
        }
    };

    // 服务实例部署信息
    struct GnsSvrLocation
    {
        std::string strIp;
        int dwPort;
        GnsSvrLocation():dwPort(0)
        {
        }
        bool operator<(const GnsSvrLocation& other) const
        {
            return (strIp == other.strIp) ? (dwPort < other.dwPort) : (strIp < other.strIp);
        }
        bool operator==(const GnsSvrLocation& other) const
        {
            return ((strIp == other.strIp) && (dwPort == other.dwPort));
        }
    };

    typedef std::map<GnsSvrAttribute, std::set<GnsSvrLocation> > GnsAttr2Location;

    struct GnsSvrMsInfo 
    {
        MsObserverApi * pMsApi;
        GnsAttr2Location oAttr2Location;   // 根据GNS配置的分片和灰度找到其master服务的ip、port
    };
}

// 主从管理器
class MsMgr
{
public:
    MsMgr();
    ~MsMgr();

    // Function: 获取某一服务的Master节点信息
    // Input:   strZkPath   目标服务在Zookeeper上注册主从信息时使用的路径
    // Input:   strZkHost   目标服务注册主从信息时使用的Zookeeper（可以是host列表，格式如"127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"）
    // Output:  oAttr2Location     符合条件的Master的部署位置
    // Return: 操作是否成功。
    bool GetMaster(const std::string & strZkPath, const std::string & strZkHost, Goome::GnsAttr2Location & oAttr2Location);

    // Function: 获取某一服务指定分片、灰度的Master信息（IP+端口）
    // Input:   strZkPath   目标服务在Zookeeper上注册主从信息时使用的路径
    // Input:   dwShard     指定目标服务的分片
    // Input:   dwGrey      指定目标服务的灰度版本
    // Input:   strZkHost   目标服务注册主从信息时使用的Zookeeper（可以是host列表，格式如"127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"）
    // Output:  oSvrLoc     符合条件的Master的部署位置
    bool GetMaster(const std::string & strZkPath, int dwShard, int dwGrey, const std::string & strZkHost, std::set<Goome::GnsSvrLocation> & oSvrLoc);

    // Function: 更新某一Zookeeper Node路径上主从服务的信息，主要供Zookeeper回调函数使用
    // Input:   strPath             目标Zookeeper Node路径
    // Input:   oAttr2Location      主从服务的信息
    void SetLocation(const std::string & strPath, const Goome::GnsAttr2Location & oAttr2Location);

    // Function: 设置主从信息变化时的回调函数
    void SetCallback(MsApiCallback f);

    // Function: 获得错误信息
    // Return:   错误信息字符串
    std::string GetError()
    {
        return m_strErr;
    }
protected:
    // Function: 在Zookeeper上获取并监听指定路径的信息
    // Input:   strZkHost   目标Zookeeper的主机信息
    // Input:   strZkPath   目标路径
    // Return: 操作是否成功。
    bool Monitor(const std::string & strZkHost, const std::string & strZkPath);

    friend void MsMgrWatcher(void * pUserParam, const std::string & strZkPath, const std::vector<ZNodeInfo> & oNodes);

protected:
    MsApiCallback m_fCallback;      // 用户自定义回调函数

    std::string m_strErr;

    std::map<std::string, Goome::GnsSvrMsInfo> m_oSvr2ZkApi;    // “Zookeeper路径 -> 其主从信息”的本地缓存
    // 注：由于此处Key是Zookeeper的Znode路径。如果日后系统中部署了多套互相隔离的Zookeeper，此时某Caller无法去多套ZK上取同一路径的主从数据。

    pthread_mutex_t m_oLock;    // TODO: 将来改造成读写锁以提高性能
private:
};

#endif // GOOME_MS_INTERFACE_H_

