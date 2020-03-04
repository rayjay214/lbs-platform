
#ifndef GOOME_MS_OBSERVER_H_
#define GOOME_MS_OBSERVER_H_

#include "zookeeper.h"
#include "zookeeper.jute.h"
#include "zkbase.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <set>

struct ZNodeInfo 
{
    std::string strNodeName;
    std::string strNodeData;
};

// 回调函数形式
typedef void (*MsApiCallback)(void * pUserParam, const std::string & strPath, const std::vector<ZNodeInfo> & oNodes);

class ZkObserver : public ZkBaseApi
{
public:
    ZkObserver();

    ~ZkObserver();

    virtual int Init(const char * pHost);

    virtual int ReconnectDeal();

    virtual int MonitorService(const std::string & strZkPath, std::vector<ZNodeInfo> & oInfo);

    virtual void SetCallback(MsApiCallback fUserCallBack, void * pUserParam);
protected:
    virtual int WatchNode(const std::string & strZkPath, std::vector<ZNodeInfo> & oInfo);

public:
    MsApiCallback m_fCallback;
    void * m_pUserParam;
    std::set<std::string> m_oZkPaths;
};

class MsObserverApi
{
public:
    MsObserverApi();

    ~MsObserverApi();

    virtual int Init(const char * pZkHost, const char * pPath, std::vector<ZNodeInfo> & oInfo);

    virtual void SetCallback(MsApiCallback fUserCallBack, void * pUserParam);

    virtual std::string GetErrorMsg();
protected:
    ZkObserver m_oZkMgr;
};


#endif /*GOOME_MS_OBSERVER_H_*/
