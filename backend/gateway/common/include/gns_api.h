/*
	GNS API Library Code
*/


#ifndef GNS_API_H
#define GNS_API_H

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>
#include "ms_interface.h"

namespace GNS_API
{

// ��ȡ������ʱ�Ƿ�������IDC
enum SPECIFY_IDC_TYPE
{
    NO_SPECIFY_IDC = 0,         // ������IDC
    SPECIFY_SAME_IDC = 1,       // ֻ��ȡͬIDC�ķ���
	SPECIFY_SPECIAL_IDC = 2,	// ֻ��ȡָ��IDC�ķ���
    SPECIFY_DIFFERENT_IDC = -1  // ֻ��ȡ��ͬIDC�ķ���
};

// ��������Ϣ�ṹ
struct CALLER
{
	std::string caller_id;      // �����ߵ�GNS����
    std::string caller_ip;      // ����IP
    int caller_port;            // �����˿�
    std::string caller_idc;     // ����IDC���ƣ���ʽһ��Ϊ����·ǰ׺ + ����ǰ׺ + IDC����ǰ׺ + ���ܱ�ʶ��
    int shard_id;               // ��ƬID
    int grey_ver;               // �ҶȰ汾
    CALLER():caller_port(0), shard_id(0), grey_ver(0)
    {
    }; 
} ;

// ����������Ϣ�ṹ
struct CALLEE
{
	std::string callee_id;      // �������ߵ�GNS����
	std::string ip;             // ����IP
	int port;                   // �����˿�
    std::string callee_idc;     // ����IDC���ƣ���ʽһ��Ϊ����·ǰ׺ + ����ǰ׺ + IDC����ǰ׺ + ���ܱ�ʶ��
    int priority;               // ���ȼ�����û�������������£���ֵ��ߵ�һ�������߷���ᱻѡ�С���ֵ>0��ʾ����������ã�<=0��ʾ������
    int shard_id;               // ��ƬID����ʹ�÷�Ƭ�ķ���Ĭ��Ϊ0������һƬ��
	int callee_ver;             // �ҶȰ汾�ţ���ʹ�ûҶȵķ���Ĭ��Ϊ0������һ���汾��
    int specify_idc;            // ָ��IDC��0=������IDC��Ĭ�ϣ���1=ֻ��ȡͬIDC�ķ���2=ֻ��ȡָ��IDC�ķ���-1=ֻ��ȡ��ͬIDC�ķ���
    int choose_strategy;		//0�����ѡ��(Ĭ��)��1Ϊ��ѯ���������޶���
    int specify_master;         // 0�ǲ�����Ŀ����master����slave��Ĭ�ϣ���1��ָ����ȡmaster��-1��ָ����ȡslave
	
	CALLEE()
	{
		port = 0;
		priority = 0;
        shard_id = 0;
		callee_ver = 0;
        specify_idc = (int)NO_SPECIFY_IDC;
		choose_strategy = 0;
        specify_master = 0;
	}; 

    bool operator==(const CALLEE & other) const
    {
        return ((callee_id == other.callee_id)
            && (ip == other.ip)
            && (port == other.port)
            && (shard_id == other.shard_id)
            && (callee_ver == other.callee_ver));
    }

    bool operator!=(const CALLEE & other) const
    {
        return ((callee_id != other.callee_id)
            || (ip != other.ip)
            || (port != other.port)
            || (shard_id != other.shard_id)
            || (callee_ver != other.callee_ver));
    }

    bool operator<(const CALLEE & other) const
    {
        if (callee_id == other.callee_id)
        {
            if (ip == other.ip)
            {
                return (port < other.port);
            }
            else
            {
                return (ip < other.ip);
            }
        }
        else
        {
            return (callee_id < other.callee_id);
        }
    }
};

// �ϱ�GNS���ý���Ľṹ
struct REPORT
{
	struct timeval beg_time;        // ���ÿ�ʼʱ��
	struct timeval end_time;        // ���ý���ʱ��
	int return_code;                // ���÷����롣0��ʾ������������ֵ��ʾ������[-100, -1]ΪԤ���Ĵ���ţ��ⲿ������������صĴ����ʾ��
	int method_id;                  // ���õķ���ID�����ڱ������ṩ�ĸ����ӿڣ�������������ʹ�ò�ͬ��ID����֮���Ը��ٴ���
};


// �������ܣ�   ���ݵ����ߺͱ������ߵ�GNS���ƣ��ҵ�һ�����ʵı����߷���
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callee  �������ߵ���Ϣ�����б������callee.callee_id��
//                      ����벻��Ҫ��ȡĳ�ض���Ƭ�ķ�����callee.shard_id��-1
//                      �������Ҫ��ȡĳ�ض��ҶȰ汾�ķ�����callee.callee_ver��-1
//                      ���ֻ���ȡMasterʵ������������callee.specify_master=1����ʱshard_id��callee_ver������-1.
// ���������   callee  ѡ�е�ĳ���������߷����ȫ����Ϣ
// ����ֵ��     ����0��ʾѡ·�ɹ���������ʾʧ�ܡ�
int GNS_API_get(const CALLER& caller, CALLEE& callee);


// �������ܣ�   ���ݵ����ߺͱ������ߵ�GNS���ƣ��ҵ�һ�����ʵı����߷��񣬲�����������������һ����֮ǰ�������ǿ��õ�
//              �ú����ᾡ����һ�����ȼ�>0�ı����ߣ�����Ҳ������򷵻�ʧ�ܡ�
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callee  �������ߵ���Ϣ�����б������callee.callee_id��
//                      ����벻��Ҫ��ȡĳ�ض���Ƭ�ķ�����callee.shard_id��-1
//                      �������Ҫ��ȡĳ�ض��ҶȰ汾�ķ�����callee.callee_ver��-1
// ���������   callee  ѡ�е�ĳ���������߷����ȫ����Ϣ
// ����ֵ��     ����0��ʾѡ·�ɹ���������ʾʧ�ܡ�
int GNS_API_get_first_available(const CALLER& caller, CALLEE& callee);


// �������ܣ�   ���ݵ����ߺͱ������ߵ�GNS���ƣ��ҵ��������������б����߷��񣬲���callee������һ�����ʵı����߷���
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callee  �������ߵ���Ϣ�����б������callee.callee_id��
//                      ����벻��Ҫ��ȡĳ�ض���Ƭ�ķ�����callee.shard_id��-1
//                      �������Ҫ��ȡĳ�ض��ҶȰ汾�ķ�����callee.callee_ver��-1
//                      ������ȡ������Ƭ���Ҷȵ�Masterʵ������������callee.specify_master=1����ʱcallees�����õ�������������������Master��
// ���������   callee  ѡ�е�ĳ���������߷����ȫ����Ϣ
// ���������   callees �������������б������߷������Ϣ
// ����ֵ��     ����0��ʾִ�гɹ���������ʾʧ�ܡ�
int GNS_API_gets(const CALLER& caller, CALLEE& callee,std::vector<CALLEE>& callees);


// �������ܣ�   ���ݵ����ߵ�GNS���ƣ��ҵ��ܱ��õ����ߵ��õ����з����ȫ����Ϣ��
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callees �ܱ��õ����ߵ��õ����з������Ϣ
// ����ֵ��     ����0��ʾִ�гɹ���������ʾʧ�ܡ�
int GNS_API_GetAllCallee(const CALLER& caller, std::map<std::string, std::vector<CALLEE> > & callees);


// �������ܣ�   ���ݵ����ߵ�GNS���ƺͶ˿ںţ���ȡ�õ����ߵ���Ϣ����IDC���ƣ���Ƭ/�ҶȺŵ�
// ���������   gns_name �����ߵ�GNS��
// ���������   port     �����ߵĶ˿ںţ���õ����߿��ܻ�ռ�ö���˿ڣ��������-1����ʾ��ָ���˿ںţ�ȡ��һ����¼����Ϣ��
// ���������   caller   �õ����߽ṹ�е�������Ա
// ����ֵ��     ����0��ʾִ�гɹ���������ʾʧ�ܡ�
int GNS_API_GetCallerInfo(const std::string & gns_name, int port, CALLER & caller);

// �������ܣ�   ��ȡ���������Ƿ���Ҫ�������еı����߷��񡱵�������Ϣ���Ըý������ǿ��Ҫ�󣬽����ο�
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callee  �������ߵ���Ϣ�����б������callee.callee_id��
// ����ֵ��     ����1��ʾ��������ø�callee�����з��񡱣�0��ʾ��������ø�callee������һ�����񡱣�������ʾִ��ʧ�ܡ�
int GNS_API_IfCallAll(const CALLER & caller, const CALLEE & callee);

// �������ܣ�   ��ȡ��һ�η��ʹ����������Ϣ
// ����ֵ��     ��������
std::string GNS_API_GetErrorMsg();

// �������ܣ�   ���Ա�����־��ʽ���ϱ����ý����GNS����GSC����
// ���������   caller  �����ߵ���Ϣ������ֻ��Ҫ����caller.caller_id����
// ���������   callee  �������ߵ���Ϣ
// ���������   report  ���ý��
// ����ֵ��     ����0��ʾִ�гɹ���������ʾʧ�ܡ�
int GNS_API_report(const CALLER& caller, const CALLEE& callee, const REPORT& report);

// �������1������ͬһ��IDC������-1��ʾ����ͬ��IDC
int IdcRelation(const std::string & caller_idc, const std::string & callee_idc);

// ����������Ϣ�仯ʱ�Ļص�����
void SetMsCallback(MsApiCallback f);
}

#endif

