#ifndef __REDIS_CONNECT_POOL_HEAD_H_
#define __REDIS_CONNECT_POOL_HEAD_H_

#include <vector>
#include <string>
#include <IceUtil/IceUtil.h>

template<typename T>
class BaseConnectPool
{

public:
	typedef  T        value_type;
    typedef  T&       reference;
	typedef  T*       pointer;
	
	
    BaseConnectPool(int max_conn)
    {
        m_cur_conn = 0;
        m_max_conn = max_conn;
        m_conns.reserve(m_max_conn + 1);
    }

    virtual ~BaseConnectPool(){}   

    virtual pointer create() = 0;

    virtual void destroy(pointer) = 0;

    pointer grab()
    {
        pointer p = NULL;
        IceUtil::Mutex::Lock _lock(m_mutex);
        if(!m_conns.empty())
        {
            p = m_conns.back();
            m_conns.pop_back();
        }
        else 
        {
            if(m_cur_conn >= m_max_conn)
            {
                return NULL;
            }

            p = create();
			if(!p)
			{
			    return NULL;
			}

			++m_cur_conn;
        }

        return p;
    }

    void Release(pointer con)
    {
         if(con == NULL)
              return;

         IceUtil::Mutex::Lock _lock(m_mutex);
         if(m_conns.size() < (size_t)m_max_conn)
             m_conns.push_back(con);
		 else
		 {
		 	 destroy(con);
			 if(m_cur_conn > 0) --m_cur_conn;
		 }
    }

	virtual void Remove(pointer con)
	{
	    destroy(con);
		if(m_cur_conn > 0)  --m_cur_conn;
	}
	
protected:
	int                  m_cur_conn;
    int                  m_max_conn;
    IceUtil::Mutex       m_mutex;       
    std::vector<pointer> m_conns;
};



#endif

