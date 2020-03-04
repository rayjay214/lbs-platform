#ifndef __SHARE_MEMORY_H__
#define __SHARE_MEMORY_H__

#include <sys/shm.h>
#include <string>
#include <cstring>

// �ӹ����ڴ��м��صľ��

class CRouteEntry
{
	public:
	// ��������Ϣ
	char m_caller_id[50]; 	// 50 �����߷���ID

	// ����������Ϣ
	char m_callee_id[50]; 	// 50 �������߷���ID
	char m_callee_ip[50]; 	// 16 ��������IP
	int m_callee_port;		// 4 �������߶˿ڣ�-1Ϊ��Чֵ
	// ����ͳ������
	int m_health; 			// 4 ��������ֵ��-1Ϊ��Чֵ
	int m_Priority;			// 4 callee��IP��PORT���ȼ���-1Ϊ��Чֵ
	int m_health_local;		// 4 ���ؽ�������ֵ��-1Ϊ��Чֵ

	//�������ܣ�CRouteEntry �Ĵ��������캯��
	CRouteEntry(const std::string &caller_id, const std::string &callee_id, int Priority = -1, std::string callee_ip= "", int callee_port = -1, int health = -1,int health_local = -1 )
	{
		if( !caller_id.empty())
		{
			strncpy(m_caller_id, caller_id.c_str(), sizeof(m_caller_id));
		}
		else
		{
			m_caller_id[0] ='\0';
		}
		if(!callee_id.empty())
		{
			strncpy(m_callee_id, callee_id.c_str(), sizeof(m_callee_id));
		}
		else
		{
			m_callee_id[0] ='\0';
		}
		m_Priority = Priority;
		if( !callee_ip.empty())
		{
			strncpy(m_callee_ip, callee_ip.c_str(), sizeof(m_callee_ip));
		}
		else
		{
			m_callee_ip[0] ='\0';
		}
		m_callee_port = callee_port;
		m_health = health;
		m_health_local = health_local;
	};
	CRouteEntry(const char *caller_id = NULL, const char *callee_id = NULL, int Priority = -1, const char *callee_ip = NULL, int callee_port = -1, int health = -1,int health_local = -1 )
	{
		if( NULL != caller_id)
		{
			strncpy(m_caller_id, caller_id, sizeof(m_caller_id));
		}
		else
		{
			m_caller_id[0] ='\0';
		}
		if( NULL != callee_id)
		{
			strncpy(m_callee_id, callee_id, sizeof(m_callee_id));
		}
		else
		{
			m_callee_id[0] ='\0';
		}
		m_Priority = Priority;
		if( NULL != callee_ip)
		{
			strncpy(m_callee_ip, callee_ip, sizeof(m_callee_ip));
		}
		else
		{
			m_callee_ip[0] ='\0';
		}
		m_callee_port = callee_port;
		m_health = health;
		m_health_local = health_local;
	};

	//�������ܣ�CRouteEntry �Ĳ����������캯��
	CRouteEntry()
	{
		m_caller_id[0] = '\0';
		m_callee_id[0] = '\0';
		m_callee_ip[0] = '\0';
		m_callee_port = -1;
		m_health = -1;
		m_Priority = -1;
		m_health_local = -1;
	};
	
	//�������ܣ�"<=" ���ṹ���غ���
	//�����ص㣺ÿ���ֶζ������Ƚϣ���������˳��Ϊ��
	//m_caller_id, m_callee_id, m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator<=(const CRouteEntry& other) const
	{
		return(complare(other)<= 0);
	};

	//�������ܣ�"<" ���ṹ���غ���
	//�����ص㣺ÿ���ֶζ������Ƚϣ���������˳��Ϊ��
	//m_caller_id, m_callee_id, m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator<(const CRouteEntry& other) const
	{
		return(complare(other)< 0);
	};

	//�������ܣ�">" ���ṹ���غ���
	//�����ص㣺ÿ���ֶζ������Ƚϣ���������˳��Ϊ��
	//m_caller_id, m_callee_id, m_Priority,m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator>(const CRouteEntry& other) const
	{
		return(complare(other)> 0);
	};

	//�������ܣ�"=="���ṹ���غ���
	//�����ص㣺�����ṹҪ��ȣ������������ĸ��ֶζ����
	//m_caller_id, m_callee_id,m_Priority, m_callee_ip, m_callee_port 
	bool operator==(const CRouteEntry& other) const
	{
		return(complare(other)== 0);
	};

	int complare(const CRouteEntry& snd) const
	{
		int rc = 0;
		rc = strncmp(m_caller_id, snd.m_caller_id, sizeof(m_caller_id));
		if (rc != 0)
		{
			return rc;
		}
		
		if(isOK(m_callee_id) && isOK(snd.m_callee_id))
		{
			rc =  strncmp(m_callee_id, snd.m_callee_id, sizeof(m_callee_id));
			if (rc != 0)
			{
				return rc;
			}
		}
		else
		{
			return rc;
		}

		if(isOK(m_Priority) && isOK(snd.m_Priority))
		{
			rc = intcmp(m_Priority, snd.m_Priority);
			if (rc != 0)
			{
				return rc;
			}
		}
		else
		{
			return rc;
		}
		
		if(isOK(m_callee_ip) && isOK(snd.m_callee_ip))
		{
			rc =  strncmp(m_callee_ip, snd.m_callee_ip, sizeof(m_callee_ip));
			if (rc != 0)
			{
				return rc;
			}
		}
		else
		{
			return rc;
		}
		
		if(isOK(m_callee_port) && isOK(snd.m_callee_port))
		{
			return intcmp(m_callee_port, snd.m_callee_port);
		}
		else
		{
			return rc;
		}
	};

	bool isOK(const char* s_Vector) const
	{
		 return('\0' != s_Vector[0]);
	};

	CRouteEntry& operator=(const CRouteEntry& other)
	{
		if( '\0' != other.m_caller_id[0])
		{
			strncpy(m_caller_id, other.m_caller_id, sizeof(m_caller_id));
		}

		if( '\0' != other.m_callee_id[0])
		{
			strncpy(m_callee_id, other.m_callee_id, sizeof(m_callee_id));
		}

		if( '\0' != other.m_callee_ip[0])
		{
			strncpy(m_callee_ip, other.m_callee_ip, sizeof(m_callee_ip));
		}
		
		m_callee_port = (-1 != other.m_callee_port) ? other.m_callee_port : m_callee_port;
		m_health = (-1 != other.m_health) ? other.m_health : m_health;
		m_Priority = (-1 != other.m_Priority) ? other.m_Priority : m_Priority;
		m_health_local = (-1 != other.m_health_local) ? other.m_health_local : m_health_local;
		return *this;
	};

	bool isOK(const int i_N) const
	{
		return( -1 != i_N);
	};
	private:
	int intcmp(int fst, int snd) const
	{
		return (fst - snd);
	}
};
class CMyShare
{
public:
		//���ڹ����ڴ浱ǰ��ʹ�ò��ֵĲ���ָ������ֵ����
		int *plength;
		int max_entry; 
		CRouteEntry *proute;

    CMyShare():plength(0),max_entry(0),proute(NULL)
    {
    };
};

class CRouteCache
{
	private:
		// �����ڴ��д������ݵĲ��ֱ�ʶ������0 ����ǰ�벿�������ݣ�1 �����벿��û������
		// 2(Ĭ��ֵ) ���������������ֶ�û������
		int *m_pindex;
		//�����ڴ���ܳ��Ⱥ���ʼָ��
		int m_real_length;
		void *m_pshare_cache;

		//���ڹ����ڴ�ǰ������ָ������ֵ����
		int *m_pfirst_length;
		int *m_pfirst_max_entry;
		CRouteEntry *m_pfirst_route;
		int m_first_size;

		//���ڹ����ڴ�������ָ������ֵ����
		int *m_psecond_length;
		int *m_psecond_max_entry;
		CRouteEntry *m_psecond_route;
		int m_second_size;

	public:
		// ���캯������������
		CRouteCache();
		~CRouteCache();
		
		// �������ܣ�ʹ��ǰ����ָ��ָ����Ҫ�������ݸ��µĹ����ڴ�Ĳ���
		// ��������������ڴ�����ṹָ��
		void get_expire_cache(CMyShare& oMyShare);
		// �������ܣ�ʹm_pindexָ����ָ���int��������ʶ�ĵ�ǰ�������ݵĹ����ڴ沿��
		void reset_index();
		// �������ܣ�ʹ��ǰ����ָ��ָ��������ݵĹ����ڴ�Ĳ���
		// ����ֵ���ɹ�����0�����򷵻�-1
		int get_data_cache(CMyShare& oMyShare);
		// �������ܣ���չ����ڴ�ռ�
		// ����ֵ���ɹ�����򷵻�0�����򷵻�-1
		int Clear(CMyShare& oMyShare);
		// �������ܣ���ȡ����ǰ����·����Ŀ
		// ����ֵ�����ع���ǰ����·����Ŀ
		int GetCount(CMyShare& oMyShare);
		// �������ܣ���ʼ�����乲���ڴ�ռ�
		// ����ֵ���ɹ���ʼ������0�����򷵻�-1
		int Init(int key, int max_entry);
		// �������ܣ����շ��乲���ڴ�ռ�
		// ����ֵ���ɹ����շ���0�����򷵻�-1
		int Destroy();
		// �������ܣ��ڹ����ڴ��в���·����
		// �������������·����ṹ�����
		// ����ֵ���ɹ����뷵��0�����ظ����-2���ռ䲻�㷵��-3������������Ч-4�����򷵻�-1
		int Insert(CMyShare& oMyShare,const CRouteEntry& route_entry );
		// �������ܣ��ڹ����ڴ��л�ȡ·����
		// ���������������ṹ�����
		// ���������ƥ��·����Ŀ��ʼָ�룬��ƥ��·����Ŀ��Ŀ
		// ����ֵ���ɹ��ҵ�����0�����򷵻�-1
		int Inquire(CMyShare& oMyShare,const CRouteEntry& route_entry, const CRouteEntry** route_entry_f, int& match_lenth);
		// �������ܣ��ڹ����ڴ��и���·����
		// �������������·����ṹ�����
		// ����ֵ���ɹ����·���0��û���ҵ���Ӧ���-2�����򷵻�-1
		int Update(CMyShare& oMyShare,const CRouteEntry& route_entry);
		// �������ܣ��ڹ����ڴ���ɾ��·����
		// ���������ɾ��·����ṹ�����
		// ����ֵ���ɹ�ɾ������0��û���ҵ���Ӧ���-2�����򷵻�-1
		int Delete(CMyShare& oMyShare,const CRouteEntry& route_entry);
		int printall(CMyShare& oMyShare);

	private:
		//�������ܣ�ʹ������ǰ�Ĳ���ָ��ָ�������ĵ�һ������
		void get_first_cache(CMyShare& oMyShare);
		//�������ܣ�ʹ������ǰ�Ĳ���ָ��ָ�������ĵڶ�������
		void get_second_cache(CMyShare& oMyShare);
		// �������ܣ��������ڴ���ĳ����ʼ��·�������ָ������
		// �����������ʼ·����Ŀ�±꣬�ƶ�����
		// ����ֵ���ɹ��ƶ�����0�����򷵻�-1
		int MoveBackword(CMyShare& oMyShare,int start, int step);
		// �������ܣ��������ڴ���ĳ����ʼ��·����ǰ��ָ������
		// �����������ʼ·����Ŀ�±꣬�ƶ�����
		// ����ֵ���ɹ��ƶ�����0�����򷵻�-1
		int MoveForword(CMyShare& oMyShare,int start, int step);
		// �������ܣ��ڹ����ڴ��в����Ƿ���·����
		// ���������������ṹ�����
		// �����������·�ɻ������׸�ƥ����±�
		// ����ֵ���ɹ��ҵ�����0�����򷵻�-1
		int Binary_search(CMyShare& oMyShare,const CRouteEntry & route_entry, int & start);
		// �������ܣ��ڹ����ڴ��в����Ƿ���·����
		// ���������������ṹ�����
		// �����������·�ɻ������׸������һ��ƥ����±�
		// ����ֵ���ɹ��ҵ�����0�����򷵻�-1
		int Equal_range(CMyShare& oMyShare,const CRouteEntry & route_entry, int & start, int & end);

};

#endif




