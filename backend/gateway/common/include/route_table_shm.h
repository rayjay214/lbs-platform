#ifndef __SHARE_MEMORY_H__
#define __SHARE_MEMORY_H__

#include <sys/shm.h>
#include <string>
#include <cstring>

// 从共享内存中加载的句柄

class CRouteEntry
{
	public:
	// 调用者信息
	char m_caller_id[50]; 	// 50 调用者服务ID

	// 被调用者信息
	char m_callee_id[50]; 	// 50 被调用者服务ID
	char m_callee_ip[50]; 	// 16 被调用者IP
	int m_callee_port;		// 4 被调用者端口，-1为无效值
	// 调用统计数据
	int m_health; 			// 4 健康度数值，-1为无效值
	int m_Priority;			// 4 callee的IP、PORT优先级，-1为无效值
	int m_health_local;		// 4 本地健康度数值，-1为无效值

	//函数功能：CRouteEntry 的带参数构造函数
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

	//函数功能：CRouteEntry 的不带参数构造函数
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
	
	//函数功能："<=" 本结构重载函数
	//函数特点：每个字段都参数比较，并且优先顺序为：
	//m_caller_id, m_callee_id, m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator<=(const CRouteEntry& other) const
	{
		return(complare(other)<= 0);
	};

	//函数功能："<" 本结构重载函数
	//函数特点：每个字段都参数比较，并且优先顺序为：
	//m_caller_id, m_callee_id, m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator<(const CRouteEntry& other) const
	{
		return(complare(other)< 0);
	};

	//函数功能：">" 本结构重载函数
	//函数特点：每个字段都参数比较，并且优先顺序为：
	//m_caller_id, m_callee_id, m_Priority,m_Priority, m_callee_ip, m_callee_port, m_health, m_health_local
	bool operator>(const CRouteEntry& other) const
	{
		return(complare(other)> 0);
	};

	//函数功能："=="本结构重载函数
	//函数特点：两个结构要相等，必须有下面四个字段都相等
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
		//用于共享内存当前所使用部分的操作指针与数值变量
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
		// 共享内存中存在数据的部分标识变量，0 代表前半部分有数据，1 代表后半部分没有数据
		// 2(默认值) 代表两个两个部分都没有数据
		int *m_pindex;
		//共享内存的总长度和起始指针
		int m_real_length;
		void *m_pshare_cache;

		//用于共享内存前半区的指针与数值变量
		int *m_pfirst_length;
		int *m_pfirst_max_entry;
		CRouteEntry *m_pfirst_route;
		int m_first_size;

		//用于共享内存后半区的指针与数值变量
		int *m_psecond_length;
		int *m_psecond_max_entry;
		CRouteEntry *m_psecond_route;
		int m_second_size;

	public:
		// 构造函数和析构函数
		CRouteCache();
		~CRouteCache();
		
		// 函数功能：使当前操作指针指向需要进行数据更新的共享内存的部分
		// 输出参数：共享内存操作结构指针
		void get_expire_cache(CMyShare& oMyShare);
		// 函数功能：使m_pindex指针所指向的int变量所标识的当前存在数据的共享内存部分
		void reset_index();
		// 函数功能：使当前操作指针指向存在数据的共享内存的部分
		// 返回值：成功返回0，否则返回-1
		int get_data_cache(CMyShare& oMyShare);
		// 函数功能：清空共享内存空间
		// 返回值：成功清空则返回0，否则返回-1
		int Clear(CMyShare& oMyShare);
		// 函数功能：获取共享当前可用路由数目
		// 返回值：返回共享当前可用路由数目
		int GetCount(CMyShare& oMyShare);
		// 函数功能：初始化分配共享内存空间
		// 返回值：成功初始化返回0，否则返回-1
		int Init(int key, int max_entry);
		// 函数功能：回收分配共享内存空间
		// 返回值：成功回收返回0，否则返回-1
		int Destroy();
		// 函数功能：在共享内存中插入路由项
		// 输入参数：插入路由项结构体参数
		// 返回值：成功插入返回0，有重复项返回-2，空间不足返回-3，插入数据无效-4，否则返回-1
		int Insert(CMyShare& oMyShare,const CRouteEntry& route_entry );
		// 函数功能：在共享内存中获取路由项
		// 输入参数：查找项结构体参数
		// 输出参数：匹配路由项目起始指针，和匹配路由项目数目
		// 返回值：成功找到返回0，否则返回-1
		int Inquire(CMyShare& oMyShare,const CRouteEntry& route_entry, const CRouteEntry** route_entry_f, int& match_lenth);
		// 函数功能：在共享内存中更新路由项
		// 输入参数：更新路由项结构体参数
		// 返回值：成功更新返回0，没有找到对应项返回-2，否则返回-1
		int Update(CMyShare& oMyShare,const CRouteEntry& route_entry);
		// 函数功能：在共享内存中删除路由项
		// 输入参数：删除路由项结构体参数
		// 返回值：成功删除返回0，没有找到对应项返回-2，否则返回-1
		int Delete(CMyShare& oMyShare,const CRouteEntry& route_entry);
		int printall(CMyShare& oMyShare);

	private:
		//函数功能：使操作当前的操作指针指向共享区的第一个部分
		void get_first_cache(CMyShare& oMyShare);
		//函数功能：使操作当前的操作指针指向共享区的第二个部分
		void get_second_cache(CMyShare& oMyShare);
		// 函数功能：将共享内存中某个起始的路由项后移指定距离
		// 输入参数：起始路由项目下标，移动距离
		// 返回值：成功移动返回0，否则返回-1
		int MoveBackword(CMyShare& oMyShare,int start, int step);
		// 函数功能：将共享内存中某个起始的路由项前移指定距离
		// 输入参数：起始路由项目下标，移动距离
		// 返回值：成功移动返回0，否则返回-1
		int MoveForword(CMyShare& oMyShare,int start, int step);
		// 函数功能：在共享内存中查找是否有路由项
		// 输入参数：查找项结构体参数
		// 输出参数：在路由缓存中首个匹配的下标
		// 返回值：成功找到返回0，否则返回-1
		int Binary_search(CMyShare& oMyShare,const CRouteEntry & route_entry, int & start);
		// 函数功能：在共享内存中查找是否有路由项
		// 输入参数：查找项结构体参数
		// 输出参数：在路由缓存中首个和最后一个匹配的下标
		// 返回值：成功找到返回0，否则返回-1
		int Equal_range(CMyShare& oMyShare,const CRouteEntry & route_entry, int & start, int & end);

};

#endif




