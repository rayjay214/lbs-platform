// ��װGFS��Get����
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

        // ��ʼ������
        // Input:   oCaller         �����ߵ�GNS��Ϣ
        // Input:   oCallee         �����ߣ�GFS����GNS��Ϣ
        // Input:   strGfsObjName   GFS��Ice Object���ƣ�Ĭ��ΪGfsProxy
        // Return:  ��ʼ���Ƿ�ɹ���
        bool Init(GNS_API::CALLER oCaller, GNS_API::CALLEE oCallee, std::string strGfsObjName = "GfsProxy");

        // ��GFS�л�ȡ�豸ĳ��ʱ���ڵ����ݡ�
        // Input:   uddwUid         �豸ID
        // Input:   dwBeginTime     ��ʼUnixʱ��
        // Input:   dwEndTime       ����Unixʱ��
        // Output:  oResp           ��ȡ��������
        // Return:  ��ȡ�����Ƿ�ɹ�����ʧ�ܣ�������Ϣ����GetErrMsg()���
        // ע��ʧ��ʱ����Ҳ�ѻ�ȡ������һ��������
        bool GetGpsRecords(uint64_t uddwUid, time_t dwBeginTime, time_t dwEndTime, ::Gfs::Response & oResp);

        
        // ��ȡ����������Ϣ
        // Return:  ������Ϣ�ַ���
        std::string GetErrMsg()
        {
            return m_strErrMsg;
        }

    protected:
        // ����GFS
        // Return:  ����GFS�Ƿ�ɹ�
        bool ConnectGfs();

    protected:
        bool m_bIsInited;   // �������Ƿ��ѱ���ʼ��
        GNS_API::GNS_API_ICE m_oGnsApiIce;
        Gfs::GfsProxyPrx m_gfsPrx;

        // GNS����Ϣ
        GNS_API::CALLER m_oCaller;
        GNS_API::CALLEE m_oCallee;
        std::string m_strGfsObjName;

        std::string m_strErrMsg;
    };
}



#endif  // __GM_COMMON_GFS_CLIENT_H__

