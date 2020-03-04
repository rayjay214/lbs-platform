#include "GnsIceHandle.h"

GNS_API::GNS_API_ICE* g_pGnsApi = NULL;


int InitGnsApi()
{
    if (NULL != g_pGnsApi)
    {
        return 0;
    }

    g_pGnsApi = new GNS_API::GNS_API_ICE();
    return (NULL != g_pGnsApi && g_pGnsApi->IsValid()) ? 0 : -1;
}

int InitGnsApi(int argc, char *argv[], unsigned int proxy_timeout_ms)
{
    if (NULL != g_pGnsApi)
    {
        return 0;
    }

    g_pGnsApi = new GNS_API::GNS_API_ICE(argc, argv, proxy_timeout_ms);
    return (NULL != g_pGnsApi && g_pGnsApi->IsValid()) ? 0 : -1;
}

int InitGnsApi(Ice::CommunicatorPtr ic, unsigned int proxy_timeout_ms)
{
    if (NULL != g_pGnsApi)
    {
        return 0;
    }

    g_pGnsApi = new GNS_API::GNS_API_ICE(ic, proxy_timeout_ms);
    return (NULL != g_pGnsApi && g_pGnsApi->IsValid()) ? 0 : -1;
}

int GetCabinetIDs(const std::string & strCaller, const std::string & strCallee, std::set<int> & setCabinets)
{
    GNS_API::CALLER oCaller;
    GNS_API::CALLEE oCallee;
    std::vector<GNS_API::CALLEE> vecCallees;
    oCaller.caller_id = strCaller;
    oCallee.callee_id = strCallee;
    oCallee.shard_id = -1;

    int dwRet = GNS_API::GNS_API_gets(oCaller, oCallee, vecCallees);
    if (0 != dwRet)
    {
        return dwRet;
    }
    for (std::vector<GNS_API::CALLEE>::iterator it = vecCallees.begin(); it != vecCallees.end(); ++it)
    {
        setCabinets.insert(it->shard_id);
    }
    return 0;
}

