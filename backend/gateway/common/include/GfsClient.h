// 封装GFS的Get操作
#ifndef __GM_COMMON_GFS_CLIENT_H__
#define __GM_COMMON_GFS_CLIENT_H__

#include <string>
#include "Gfs.h"
#include "gns_api_ice.h"


namespace Goome
{
    class CGfsClient
    {
    public:
        CGfsClient();
        virtual ~CGfsClient();

        // 初始化对象
        // Input:   oCaller         调用者的GNS信息
        // Input:   oCallee         被调者（GFS）的GNS信息
        // Input:   strGfsObjName   GFS的Ice Object名称，默认为GfsProxy
        // Return:  初始化是否成功。
        bool Init(GNS_API::CALLER oCaller, GNS_API::CALLEE oCallee, std::string strGfsObjName = "GfsProxy");

        // 从GFS中获取设备某段时间内的数据。
        // Input:   uddwUid         设备ID
        // Input:   dwBeginTime     起始Unix时间
        // Input:   dwEndTime       结束Unix时间
        // Output:  oResp           获取到的数据
        // Return:  获取数据是否成功。若失败，错误消息可用GetErrMsg()获得
        // 注：失败时可能也已获取了其中一部分数据
        bool GetGpsRecords(uint64_t uddwUid, time_t dwBeginTime, time_t dwEndTime, ::Gfs::Response & oResp);

        
        // 获取操作错误消息
        // Return:  错误消息字符串
        std::string GetErrMsg()
        {
            return m_strErrMsg;
        }

    protected:
        // 连接GFS
        // Return:  连接GFS是否成功
        bool ConnectGfs();

    protected:
        bool m_bIsInited;   // 本对象是否已被初始化
        GNS_API::GNS_API_ICE m_oGnsApiIce;
        Gfs::GfsProxyPrx m_gfsPrx;

        // GNS用信息
        GNS_API::CALLER m_oCaller;
        GNS_API::CALLEE m_oCallee;
        std::string m_strGfsObjName;

        std::string m_strErrMsg;
    };
}



#endif  // __GM_COMMON_GFS_CLIENT_H__

