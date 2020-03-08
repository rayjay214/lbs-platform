#ifndef _GPS_FILTER_BASE_H_
#define _GPS_FILTER_BASE_H_

#include <string>
#include <vector>
#include <cstring>
#include <stdint.h>
#include <Ice/Ice.h>

#define USE_LITTLE_ENDIAN

// ʵʱGPS���ݸ�ʽ����GMC��GPSVO_V1�ṹ��ͬ
typedef struct _GPS_STU
{
    int     iHead;          // ͷ��λ����־���������ֶ��Ƿ���Ч������GMC��

    ////////////////////////////////////////////////////////////////////////

    uint64_t uid;           // �豸ID
    uint64_t dateTime;      // ��λʱ��
    uint64_t sysTime;       // ϵͳʱ��
    uint64_t heartTime;     // ����ʱ��
    int32_t longitude;      // ����
    int32_t latitude;       // γ��
    int32_t speed;          // �ٶ�
    int32_t route;          // ����
    int32_t seq;            // ���
    uint64_t sid;           //
    char endPoint[32];      // 
    char status[16];        //
}__attribute__((packed)) GPS_STU;

// ��ʷGPS��Ϣ����GFS�л�ȡ�����ݸ�ʽ��ͬ
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


// ��־����ģ��ӿ��ֻ࣬���̳��á�
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

    // �������ܣ�   ��ʼ����ģ��
    // ����ֵ��     ����ʼ���ɹ�������0�����򷵻�-1
    virtual int Init()
    {
        return 0;
    }

    // �������ܣ�   ����ʼ����ģ��
    // ����ֵ��     ������ʼ���ɹ�������0�����򷵻�-1
    virtual int Uninit()
    {
        return 0;
    }

    // �������ܣ�   ��ȡģ�����ƣ����ⲿģ�����
    // ����ֵ��     ģ�������ַ���ָ��
    virtual const char * GetModuleName()
    {
        return m_szModuleName;
    }

    // �������ܣ�   ��ȡ������Ϣ�����ⲿģ�����
    // ����ֵ��     ������Ϣ�ַ���ָ��
    virtual const char * GetLastError()
    {
        return m_szErrorMsg;
    }

    // �������ܣ�   Ԥ����һ�����ݣ�������GPS����Ҳ���������ñ������Ϣ�����ɶ�̬�����strSource��strDestination��Ϣ���б����δ���
    // ���������strSource       �������ݵ���Դ
    // ���������strDestination  �������ݵ�Ŀ�ĵ�
    // ����/���������   data        ��Ԥ���������
    // ����ֵ��     ��Ԥ����ɹ�������0�����򷵻�-1
    virtual int PredealData(const std::string & strSource, const std::string & strDestination, std::vector<std::string > & data)
    {
        return 0;
    }

    // �������ܣ�   ����֪ͨ�²���
    // ����/���������   duration   �ϴε���notify�����ε��õ�ʱ���������룩
    // ����ֵ��     ��OK����0�����򷵻�-1
    virtual int Notify(long duration)
    {
        return 0;
    }

    // �������ܣ�   ����һ��GPS���ݣ�������GPS����Ҳ���������ñ������Ϣ�����ɶ�̬�����strSource��strDestination��Ϣ���б����δ���
    // ���������strSource       �������ݵ���Դ
    // ���������strDestination  �������ݵ�Ŀ�ĵ�
    // ���������   data        �����������
    // ����ֵ��     �������ɹ�������0������������Ϊ��ʧ�ܣ�����ԭ�򱣴���ErrorMsg��Ա������
    virtual int ProcessData(const std::string & strSource, const std::string & strDestination, const std::vector<std::string > & data) = 0;

protected:
    ::Ice::CommunicatorPtr m_pIC;
    ::Ice::ObjectAdapterPtr m_pAdapter;

    char m_szErrorMsg[1024];        // ���������Ϣ

    char m_szModuleName[128];       // ģ������

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


