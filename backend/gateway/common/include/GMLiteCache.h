/**
 *   Copyright (C) GOOME. All rights reserved.

 *   \file     GMLiteCache.h
 *   \author   xin.qiu
 *   \date     2012.7.26
 *   \version  1.0.0
 *   \brief    a simple cache template class, support size and timeout
 */
#ifndef GM_LITE_CACHE_H_H_H
#define GM_LITE_CACHE_H_H_H

#include <map>
#define LITE_CACHE_INFINITE_SIZE -1
#define LITE_CACHE_NO_EXPIRE     -1

template <typename KEY_TYPE, typename VAL_TYPE>
class CGMLiteCache  
{
public:
    typedef struct  
    {
        VAL_TYPE val;
        int      timestamp;
    }VAL_STMP;

    CGMLiteCache(int size, int timeout)
    {
        m_size    = size;
        m_timeout = timeout;
    }

    ~CGMLiteCache() {m_map.clear();}

    void SetTimeout(int timeout)
	{
		m_timeout = timeout;
	}

    bool Set(KEY_TYPE key, VAL_TYPE val)
    {
        VAL_STMP    tmp;
        bool        bExist;

        if (LITE_CACHE_INFINITE_SIZE == m_size)
        {
            tmp.val = val;
            tmp.timestamp = time(NULL);
            
            m_map[key] = tmp;
            
            return true;
        }
        bExist = (m_map.find(key) != m_map.end());
        if (  bExist || (!bExist && m_map.size() < m_size)  )
        {
            tmp.val = val;
            tmp.timestamp = time(NULL);
            
            m_map[key] = tmp;
            
            return true;
        }
        return false;
    }

    bool Get(KEY_TYPE key, VAL_TYPE& val)
    {
        int  cursecond;
        bool bFound;
        
        bFound = m_map.find(key) != m_map.end();        
        if (!bFound)
        {
            return false;
        }

        //found
        if (m_timeout == LITE_CACHE_NO_EXPIRE)
        {
            val = m_map[key].val;
            return true;
        }

        //失效时间计算
        cursecond = time(NULL);
        if ((m_map[key].timestamp + m_timeout) < cursecond)
        {
            return false;
        }
        val = m_map[key].val;

        return true;
    }
    
private:
    std::map<KEY_TYPE, VAL_STMP>    m_map;
    int                             m_size;
    int                             m_timeout;//second
};
#endif // end of #ifndef GM_LITE_CACHE_H_H_H
