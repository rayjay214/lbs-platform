#ifndef _GNS_ICE_HANDLE_H_
#define _GNS_ICE_HANDLE_H_

#include "gns_api_ice.h"
#include "tostring.h"
#include "errorcode.h"
#include <list>
#include <map>

extern GNS_API::GNS_API_ICE* g_pGnsApi;

int InitGnsApi();

int InitGnsApi(int argc, char *argv[], unsigned int proxy_timeout_ms = 5000);

int InitGnsApi(Ice::CommunicatorPtr ic, unsigned int proxy_timeout_ms = 5000);

int GetCabinetIDs(const std::string & strCaller, const std::string & strCallee, std::set<int> & setCabinets);

template<typename T>
static int Connect2Proxy(GNS_API::CALLER & oCaller, GNS_API::CALLEE & oCallee, const std::string & strIceObjID, T & prx, std::string & strErrorMsg, int dwTimeout = 5000)
{
    if (NULL == g_pGnsApi)
    {
        g_pGnsApi = new GNS_API::GNS_API_ICE();
        if (NULL == g_pGnsApi)
        {
            strErrorMsg = "Gns Api not init yet";
            return -1;
        }
    }

    strErrorMsg.clear();
    int ret = g_pGnsApi->GNS_API_getproxy(oCaller, oCallee, strIceObjID, prx, dwTimeout);

    if(0 != ret || prx == NULL)
    {
        strErrorMsg = g_pGnsApi->GetErrMsg();
        return ret;
    }

    return 0;
}

template<typename T>
static int Connect2Proxys(const std::string & strCaller, const std::string & strCallee, const std::string & strIceObjID, std::vector<T> & prxVec, std::string & strErrorMsg, int shardID = 0, int ver = 0, int dwTimeout = 5000)
{
    if (NULL == g_pGnsApi)
    {
        g_pGnsApi = new GNS_API::GNS_API_ICE();
        if (NULL == g_pGnsApi)
        {
            strErrorMsg = "Gns Api not init yet";
            return -1;
        }
    }
    strErrorMsg.clear();
    int ret = g_pGnsApi->GNS_API_getProxys(strCaller, strCallee, strIceObjID, shardID, prxVec);
    if(0 != ret || prxVec.empty())
    {
        strErrorMsg = g_pGnsApi->GetErrMsg();
        return -2;
    }

    return 0;
}

template<typename T>
static int Connect2Proxy(const std::string & strCaller, const std::string & strCallee, const std::string & strIceObjID, T & prx, std::string & strErrorMsg, int shardID = 0, int ver = 0, int dwTimeout = 5000)
{
    GNS_API::CALLER caller;
    GNS_API::CALLEE callee;

    caller.caller_id = strCaller;
    callee.callee_id = strCallee;
    callee.shard_id   = shardID;
    callee.callee_ver = ver;

    return Connect2Proxy(caller, callee, strIceObjID, prx, strErrorMsg, dwTimeout);
}

// 获取某Callee所有指定分片集合的ICE句柄
// Input:   strCaller       Caller的GNS名
// Input:   strCallee       Callee的GNS名
// Input:   oInputShardIDs  需获取句柄的分片ID集合，如为空，表示获取所有分片
// Input:   strIceObjID     Callee使用的Ice对象名
// Output:  oShard2Prx      分片ID->ICE句柄的查找表，保存已经成功获取到的ICE连接句柄
// Output:  oErrMsgs        分片ID->错误消息的查找表，保存获取失败的分片及原因。
// Input:   ver             指定的灰度版本号。
// Return: 如果指定分片集合的句柄都获取成功，返回0； 如果部分句柄获取成功，返回1； 如果全部获取失败，返回2
template<typename T>
static int Connect2ProxyByShardIDs(const std::string & strCaller,
    const std::string & strCallee,
    const std::set<int> & oInputShardIDs,
    const std::string & strIceObjID,
    std::map<int, T> & oShard2Prx,
    std::map<int, std::string> & oErrMsgs,
    int ver = 0,
    int dwIceTimeout = 5000)
{
    std::set<int> oSpecifyShards;
    if (oInputShardIDs.empty())
    {
        // 如果指定的分片集合是空的，则认为要获取所有分片ID
        GNS_API::CALLER caller;
        GNS_API::CALLEE callee;

        caller.caller_id = strCaller;
        callee.callee_id = strCallee;
        callee.shard_id   = -1;
        callee.callee_ver = ver;

        std::vector<GNS_API::CALLEE> vecCallees;
        if (0 != GNS_API::GNS_API_gets(caller, callee, vecCallees))
        {
            for (std::set<int>::const_iterator it = oInputShardIDs.begin(); it != oInputShardIDs.end(); ++it)
            {
                // 对于传入空分片ID集合的情况，此处就没法根据分片ID设置对应的error msg了
                oErrMsgs.insert(std::make_pair(*it, "Invalid GNS config!"));
            }
            return (int)ERR_ALL_GNS_ICE_FAIL;
        }

        for (std::vector<GNS_API::CALLEE>::iterator it = vecCallees.begin(); it != vecCallees.end(); ++it)
        {
            oSpecifyShards.insert(it->shard_id);
        }
    }
    else
    {
        oSpecifyShards = oInputShardIDs;
    }

    // 获取各分片的句柄
    for (std::set<int>::iterator it = oSpecifyShards.begin(); it != oSpecifyShards.end(); ++it)
    {
        int dwShardID = *it;
        T prx;
        std::string strErrorMsg;
        int dwRet = Connect2Proxy(strCaller, strCallee, strIceObjID, prx, strErrorMsg, dwShardID, ver, dwIceTimeout);
        // TODO: 可以在这里重试N次，N=满足条件的Callee实例个数
        if (0 == dwRet)
        {
            oShard2Prx.insert(std::make_pair(dwShardID, prx));
            std::map<int, std::string>::iterator itErrMsg = oErrMsgs.find(dwShardID);
            if (itErrMsg != oErrMsgs.end())
            {
                oErrMsgs.erase(itErrMsg);
            }
        }
        else
        {
            oErrMsgs.insert(std::make_pair(dwShardID, strErrorMsg));
        }
    }

    // 检查有哪些分片的获取失败了
    if (oShard2Prx.size() == oSpecifyShards.size())
    {
        // 全部获取成功
        return 0;
    }
    else if (oShard2Prx.empty())
    {
        // 全部获取失败
        return (int)ERR_ALL_GNS_ICE_FAIL;
    }
    else
    {
        // 部分获取成功
        return (int)ERR_PART_GNS_ICE_FAIL;
    }

}

/*
template<typename T>
static error_code_e ConnectProxy(const std::string &endpoint, T &prx)
{
    Ice::ObjectPrx base;
    error_code_e retCode = ERR_OK;

    try
    {
        base  = g_ic->stringToProxy(endpoint);
        prx   = T::checkedCast(base);
        if(!prx)  throw("Invalidate proxy!");
    }
    catch(const Ice::Exception &ex)
    {
        retCode = ERR_SYSTEM_ERROR;
        MYLOG_ERROR(g_logger,"Connect Clinet failed,exception:%s,proxy:%s",ex.what(),endpoint.c_str());
    }
    catch(const char *msg)
    {
        retCode = ERR_SYSTEM_ERROR;
        MYLOG_ERROR(g_logger,"Connect Clinet failed,exception:%s,proxy:%s",msg,endpoint.c_str());
    }
    catch(...)
    {
        retCode = ERR_SYSTEM_ERROR;
        MYLOG_ERROR(g_logger,"Connect Clinet failed,proxy:%s",endpoint.c_str());
    }

    return retCode;
}



template<typename T>
static error_code_e ConnectProxy(const std::string &ip,int port,const std::string &objID, T &prx)
{

    std::string str;
    str = objID + ":tcp -h " + ip + " -p ";
    str += ToString(port);

    return ConnectProxy<T>(str,prx);
}
*/

#endif



