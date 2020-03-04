#ifndef _DEV_TYPE_SHM_BASE_H_
#define _DEV_TYPE_SHM_BASE_H_
#include <sys/shm.h>
#include <cstddef>
#include <time.h>

//用户信息结构项目
namespace DevTypeShm
{

class CDevType{
	
	public:
		char m_DevType[12];
};

class DevTypeShmBase
{
	private:
		//共享内存的起始指针和总长度
		void *m_pShareCache;
		size_t *m_ipTotallSize;

		size_t *m_ipMaxEntryNum;	//设备类型最大记录条数
		size_t *m_ipEntryNum;		//设备类型当前记录条数
		size_t *m_ipVecSize;		//共享内存中整个设备类型数组存储空间的大小
		int *m_ipTimeStamp;			//最近更新的时间戳
		CDevType *m_szDevTypeCache;	//设备类型内存区指针

        char m_szErrorMsg[1024];    // 错误消息
	public:
		//构造函数和析构函数
	    DevTypeShmBase();
	    virtual ~DevTypeShmBase();
		
		const char * GetError()
        {
            return m_szErrorMsg;
        }
	protected:
		/////////////////////////////////////////////////////////////////////////
	    // Function:   _Init
	    // Description: 初始化分配共享内存空间
	    // Input:      创建或打开的共享内存的key值，最大可插入设备类型项目数
	    // Return:    成功初始化返回0，否则返回-1
	    //////////////////////////////////////////////////////////////////////////
		int _Init(int lkey, int max_entry = 0);

		// 函数功能：清空共享内存空间
		/////////////////////////////////////////////////////////////////////////
	    // Function:	_InsertDevType
	    // Description:从数据库中导入设备类型，写入共享内存中
	    // Input:		dwMaxSize       设备ID的上限
	    // Return:    成功插入返回0，否则返回-1
	    //////////////////////////////////////////////////////////////////////////
	    int _InsertDevType(int iDevID, const std::string & szDevType);

		/////////////////////////////////////////////////////////////////////////
	    // Function:	_GetDevType
	    // Description: 根据设备ID，获取其设备类型		
	    // Input:		dwDevID		设备ID
	    // Output:	strDevType	设备类型
	    // Return:	若成功获取，返回0，否则返回-1
	    //////////////////////////////////////////////////////////////////////////
	    int _GetDevType(size_t dwDevID, std::string& strDevType);

		//函数功能:清空共享内存中的设备类型记录的数组空间
		void _Clear();

		// 函数功能：获取共享当前可用设备类型记录数目
		// 返回值： 当返回-1时，表示获取失败；返回>=0时，表示共享内存中当前可用设备类型记录数目
		int _GetCount();

		// 函数功能：打印共享内存的所有有效记录
		void _Printall();

		// 函数功能：设置共享内存中的时间字段为指定的时间值
		void _SetTIme(int time);

		// 函数功能：获取共享内存中的时间字段为指定的时间值
		// 返回值：当返回-1时，表示获取失败；返回>=0时，表示共享内存中当前时间字段
		int _GetTIme();
};

}    // namespace

#endif // _DEV_TYPE_SHM_BASE_H_
