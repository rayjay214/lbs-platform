/*
* created by xin.qiu 2012.8.23 
* key为string，value为自定义固定长度结构体的map(基于共享内存)
*/
#ifndef GM_SHMMAP_KEYSTR_H_H_H
#define GM_SHMMAP_KEYSTR_H_H_H

#include "shmmap.h"
#include <string>

template<class T>
class CGMShmMapKeyStr
{
public:
    CGMShmMapKeyStr(int t_shmkey,int t_max_size, int t_key_timeout):m_map(t_shmkey, t_max_size, t_key_timeout) 
    {
    }
    
    ~CGMShmMapKeyStr()
    {
    }
    
    T* add(std::string key,const T& obj)
    {
        return m_map.add(KeyStr2Int(key), obj);
    }
    
    void del(std::string key)
    {
        return m_map.del(KeyStr2Int(key));
    }
    
    T& operator[](std::string key)
    {
        return m_map[KeyStr2Int(key)];
    }
    
    T* find(std::string key, bool &expired)
    {
        return m_map.find(KeyStr2Int(key), expired);
    }
    int getSize()
    {
        return m_map.getSize();
    }

    int getKey(int pos)
    {
        return m_map.getKey(pos);
    }

    T* get(int pos)
    {
        return m_map.get(pos);
    }
    
    void clear()
    {
        m_map.clear();
    }

private:
    int KeyStr2Int(std::string key)
    {
        return (int)BKDRHash(key.c_str());
    }

    unsigned int BKDRHash(const char *strr)
    {
        char buf[2048];
        strcpy(buf, strr);

        char* str = buf;
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;
        
        while (*str)
        {
            hash = hash * seed + (*str++);
        }
        
        return (hash & 0x7FFFFFFF);
    }

private:
    CGMShmMapKeyInt<T> m_map;

};

#endif
