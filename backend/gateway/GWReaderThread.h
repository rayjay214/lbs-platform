    /*
    * threads.h
    *
    */

#ifndef GWREADERTHREAD_H_
#define GWREADERTHREAD_H_
#include <string>
#include <set>
#include <vector>

#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_queue.h"

#include <IceUtil/Thread.h>
#include <Ice/Ice.h>
#include <sys/epoll.h>
#include "common.h"
#include "global.h"

#define EPOLL_TIMEOUT_MS 2000
#define MAX_READ_EVENTS 1000000

class GWReader: public IceUtil::Thread
{
public:
    GWReader(int id,std::vector<int>&  listenfds);
    virtual ~GWReader();

    virtual void run();
    void setThreadName(std::string threadName);
    void setExpir(int expir);
    void setPurgeTime(long time);
    void stop();

    void setAcceptEnable(bool bEnable);
	int GetBufferSize();
	void GetConnCount(uint64_t& purgeCount, uint64_t& connCount,uint64_t& failMsgCount);

	static int SendMsgToClient(uint64_t devid, const std::string &msg);

private:
    int handleRequest(int fd);
	int handleT808Msg(int fd);
	
	void insertIdTable(uint64_t id,int cfd);
	bool purgeFd(int purge_cfd, bool force,int reason);	
	std::string GetStrIp(uint32_t ip);

public:
    std::string _threadName;
    int			_Id;
    std::set<int> _listenfds;
    bool _isStop;
	
    // epoll relative
    int _epfd;
    struct epoll_event _evlist[MAX_READ_EVENTS];
    
    int _expir;
    long _purgeTime;
    int	_numOpenFds;
    IceUtil::Monitor<IceUtil::Mutex> m_monitor;

private:
	uint64_t m_purgeCount;
	uint64_t m_connCount;
	uint64_t m_failMsgCount;
};

typedef IceUtil::Handle<GWReader> GWReaderPtr;

#endif /* GWREADERTHREAD_H_ */

