#ifndef __RATE_LIMIT_H__
#define __RATE_LIMIT_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

#include <iterator>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <boost/config.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

enum
{
	CGI_BUSBO_MUTEX_IPLIMITE_KEY = 0x11235,
    CGI_BUSBO_MUTEX_METHODLIMITE_KEY = 0x11236,

	CGI_OPENBO_MUTEX_IPLIMITE_KEY = 0x12306,
    CGI_OPENBO_MUTEX_METHODLIMITE_KEY = 0x12307,
    
	CGI_RVSGEO_MUTEX_IPLIMITE_KEY = 0x20000,
    CGI_RVSGEO_MUTEX_METHODLIMITE_KEY = 0x20001,

    CGI_GOOCAR_MUTEX_IPLIMITE_KEY = 0x20002,
    CGI_GOOCAR_MUTEX_METHODLIMITE_KEY = 0x20003,
};

namespace MyProMutex
{
    inline int init(int ipc_key,pthread_mutex_t **mtx,bool bInitMutex = false)
    {
		int shmid = shmget(ipc_key, sizeof(pthread_mutex_t), IPC_CREAT | 0666);
		if(shmid < 0)
		{
		    return -1;
		}

		pthread_mutex_t* mutex = (pthread_mutex_t*)shmat(shmid, NULL, 0);
		if(mutex == (pthread_mutex_t*)-1)
		{
		    return -2;
		}

		*mtx = mutex;
		if(bInitMutex)
        {
		    pthread_mutexattr_t attr;
		    pthread_mutexattr_init(&attr);
		    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
		    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
		    pthread_mutexattr_setrobust_np(&attr,PTHREAD_MUTEX_ROBUST_NP);
		    return pthread_mutex_init(mutex, &attr);
        }

        return 0;
    }

    inline bool lock(pthread_mutex_t *mutex)
    {
        int ret = pthread_mutex_lock(mutex);
        //printf(">>>lock ret %d,%s\n",ret,strerror(ret));
        if(ret == 0)
        {
            return true;
        }
        else if(ret == EOWNERDEAD)
	    {
	        ret = pthread_mutex_consistent_np(mutex);
	        if(ret != 0)
	        {
	        	  //printf("consistent err %s\n", strerror(ret));
	            pthread_mutex_unlock( mutex);
	            return false;
	        }
	        else
	        {
	        	  pthread_mutex_unlock( mutex);
	        	  pthread_mutex_lock(mutex);
	        	  return true;
	        }
	    }

	    return false;
    }

    inline void unlock(pthread_mutex_t *mutex)
    {
        pthread_mutex_unlock( mutex);
    }

    class CAutoRelease
    {
    public:
        CAutoRelease(pthread_mutex_t *mutex = NULL) : mtx(mutex)
        {
            bret = lock(mutex);
        }

        ~CAutoRelease()
        {
	        if(bret)
	        {
	            unlock(mtx);
	        }
        }

    public:
        bool bret;
        pthread_mutex_t *mtx;
    };
};


namespace RateLimit
{

using boost::multi_index_container;
using namespace boost::multi_index;
namespace bip=boost::interprocess;

typedef bip::managed_mapped_file::allocator<char>::type	char_allocator;
typedef bip::basic_string<char, std::char_traits<char>, char_allocator> shm_string;

struct methodRate
{
    uint32_t id;
    uint32_t countSec;
    uint32_t countMin;
    uint32_t countHour;
    uint32_t countDay;
    time_t   tmSec;
    time_t   tmMin;
    time_t   tmHour;
    time_t   tmDay;

    methodRate()
    {
        id = countSec = countMin = countHour = countDay = tmSec = tmMin = tmHour = tmDay = 0;
    }
};

struct iprate
{
    uint32_t ip;
    uint32_t countSec;
    uint32_t countMin;
    uint32_t countHour;
    uint32_t countDay;
    time_t   tmSec;
    time_t   tmMin;
    time_t   tmHour;
    time_t   tmDay;

    iprate()
    {
        ip = countSec = countMin = countHour = countDay = tmSec = tmMin = tmHour = tmDay = 0;
    }
};

typedef multi_index_container<
methodRate,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(methodRate,uint32_t,id)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(methodRate,uint32_t,countSec)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(methodRate,uint32_t,countMin)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(methodRate,uint32_t,countHour)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(methodRate,uint32_t,countDay)>
>,
bip::allocator<methodRate,bip::managed_mapped_file::segment_manager>
> methodRate_con;


typedef multi_index_container<
iprate,indexed_by<
hashed_unique<BOOST_MULTI_INDEX_MEMBER(iprate,uint32_t,ip)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(iprate,uint32_t,countSec)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(iprate,uint32_t,countMin)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(iprate,uint32_t,countHour)>,
ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(iprate,uint32_t,countDay)>
>,
bip::allocator<iprate,bip::managed_mapped_file::segment_manager>
> iprate_con;


class CMethodLimit
{
public:
	typedef boost::multi_index::nth_index<methodRate_con,0>::type idIdx;
	typedef boost::multi_index::nth_index<methodRate_con,1>::type secCountIdx;
	typedef boost::multi_index::nth_index<methodRate_con,2>::type minCountIdx;
	typedef boost::multi_index::nth_index<methodRate_con,3>::type hourCountIdx;
	typedef boost::multi_index::nth_index<methodRate_con,4>::type dayCountIdx;

	enum ENUM_TYPE
	{
	    EID=0,ESEC_COUNT=1,EMIN_COUNT=2,EHOUR_COUNT=3,EDAY_COUNT=4
	};

public:
    CMethodLimit();
    ~CMethodLimit();
    int init(const char *shm_name, long count, int mutex_ipc_key,bool bInitMutex = false);
    int incCount(uint32_t ip,methodRate &data);

	bool lock();
    void unlock();
    int destory();
    int  destroyMutex();
    void check_shm(const char *shm_name, long count, int mutex_ipc_key,bool bInitMutex = false);

private:
    static methodRate_con *con;
    static pthread_mutex_t *mutex;
    static bip::managed_mapped_file *seg;
    static int maxCount;
    volatile bool tag;
};


class CIPLimit
{
public:
	typedef boost::multi_index::nth_index<iprate_con,0>::type ipIdx;
	typedef boost::multi_index::nth_index<iprate_con,1>::type secCountIdx;
	typedef boost::multi_index::nth_index<iprate_con,2>::type minCountIdx;
	typedef boost::multi_index::nth_index<iprate_con,3>::type hourCountIdx;
	typedef boost::multi_index::nth_index<iprate_con,4>::type dayCountIdx;

	enum ENUM_TYPE
	{
	    EIP=0,ESEC_COUNT=1,EMIN_COUNT=2,EHOUR_COUNT=3,EDAY_COUNT=4
	};

public:
    CIPLimit();
    ~CIPLimit();

    int init(const char *shm_name, long count, int mutex_ipc_key,bool bInitMutex = false);
    int incCount(uint32_t ip,iprate &data);
    int getDataGreaterThan(int minNum,const ENUM_TYPE etype,std::vector<iprate> &data);

    bool lock();
    void unlock();
    int destory();
    int  destroyMutex();
    void check_shm(const char *shm_name, long count, int mutex_ipc_key,bool bInitMutex = false);

    static uint32_t IPStr2Int(const char* pStr);
    static void IPInt2Str(uint32_t ip,char *pIP);
    static void time2str(time_t t,std::string &str);

private:
    int getSecCountGreaterThan(int minNum,std::vector<iprate> &data);
    int getMinCountGreaterThan(int minNum,std::vector<iprate> &data);
    int getHourCountGreaterThan(int minNum,std::vector<iprate> &data);
    int getDayCountGreaterThan(int minNum,std::vector<iprate> &data);

private:
    static iprate_con *con;
    static pthread_mutex_t *mutex;
    static bip::managed_mapped_file *seg;
    static int maxCount;
    volatile bool tag;
};

}

#endif