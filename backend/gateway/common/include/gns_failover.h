/* 
	common library for socket connection failover
		SongQuan Zhou @ 2012-08-14
*/


#ifndef GNS_CONNECTION_FAILOVER_H
#define GNS_CONNECTION_FAILOVER_H

#include <sys/time.h>
#include <string>
#include <vector>
#include <map>

namespace GNS_FAILOVER
{

// ���ڴ��ݾ��е������������ķ������� IP ��ַ�Ͷ˿ں�
typedef struct _CALLEE
{

	std::string srv_id;	// ���� ID���ɵ����߸����ʼ��������ķ���
	std::string ip;		// ���ص������������ķ������� IP ��ַ
	int port;			// ���ص������������ķ������Ķ˿ں�

    bool operator==(const _CALLEE& other) const
    {
        return ((srv_id == other.srv_id) && (ip == other.ip) && (port == other.port));
    }

    bool operator!=(const _CALLEE& other) const
    {
        return ((srv_id != other.srv_id) || (ip != other.ip) || (port != other.port));
    }

    bool operator<(const _CALLEE& other) const
    {
        return ((srv_id < other.srv_id) || (ip < other.ip) || (port < other.port));
    }

    bool operator<=(const _CALLEE& other) const
    {
        return ((srv_id <= other.srv_id) || (ip <= other.ip) || (port <= other.port));
    }
} CALLEE;

// ����������ָ�����óɹ���ʧ��ʧ��
typedef struct _REPORT
{

	struct timeval beg_time;	// �������ڵ��ú������ӡ���ȡ��д������֮ǰ�����Ƚ����ֶγ�ʼ��Ϊ��ʱ��ʱ��
	struct timeval end_time;	// ���ļ��ڲ�ʹ�ã����ڼ�¼�ϱ���ʵʱʱ��
	int return_code;			// �����߸��ݵ��ý�����г�ʼ����0 ��ʾ�ɹ���-1 ��ʾʧ��

} REPORT;

// ����:���������ڳ�ʼ�� failover ��
// �������:caller_id �ɵ�����ָ���䱾��ķ��� id
// ����ֵ��0 ��ʾ��ʼ���ɹ���-1 ��ʾ��ʼ��ʧ��
int GNS_client_init( const char* caller_id);

// ����:���������ڻ�ȡ�������������ķ����� IP �Ͷ˿ں�
// �������:callee �� srv_id �ֶΣ������߸���ָ��������ķ���
// �������:callee �� ip �� port �ֶΣ����ڷ��ؾ��е������������ķ������� IP ��ַ�Ͷ˿ں�
// ����ֵ��0 ��ʾ�ɹ���ȡIP��ַ�Ͷ˿ڣ�-1 ��ʾʧ��
int GNS_client_getnodeinfo(CALLEE& callee);

// ����:���������ڻ㱨�����ӡ���д�ĳɰ����
// �������:������ͨ�� callee ָ����㱨�Ķ�������������ӵ��ĸ�����report ����ָ���ɹ���ʧ�ܣ��Լ���Ӧ�ĵ��õ���ʼʱ���
// ����:0 ��ʾ�㱨�ɹ���-1 ��ʾ�㱨ʧ��
int GNS_client_report(const CALLEE& callee, REPORT& report);

}

#endif









