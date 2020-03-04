/*
* created by xin.qiu 2012.8.23
* key为int，value为定长自定义结构体的简单map (基于共享内存)
*/
#ifndef GM_SHMMAP_KEYINT_HPP_
#define GM_SHMMAP_KEYINT_HPP_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MEXP_NEW 0x0098233

template<class T>
class CGMShmMapKeyInt
{
public:
    CGMShmMapKeyInt(int t_shmkey,int t_max_size, int t_key_timeout) //t_gtimeout global time out
    {
        max_size = t_max_size;
        key_timeout_sec =  t_key_timeout;
        sizePtr = NULL;
        keyPtr = NULL;
        elementPtr = NULL;
        int shmid;
        shmid=shmget(t_shmkey,sizeof(int)*8+(sizeof(int)*4)*t_max_size+t_max_size*sizeof(T),IPC_CREAT|0666);
        if(shmid<=0)
        {
            fprintf(stderr," shm new exception: file=[%s] line=[%d] code=[%d] ",__FILE__,__LINE__,MEXP_NEW);
            throw MEXP_NEW;
        }
        void* addr = shmat(shmid, 0, 0);
        if(addr == (void*)-1)
        {
            fprintf(stderr," shm mat exception: file=[%s] line=[%d] code=[%d] ",__FILE__,__LINE__,MEXP_NEW);
            throw MEXP_NEW;
        }
        shm_addr = (char *)addr;
        
        //共享内存第一段int 存个数
        sizePtr = (int *)shm_addr;
        
        //第二位存最小空闲块
        minfreePtr = (int *)(shm_addr+sizeof(int));
        
        //存全局超时时间 s
        pGlobalTmout = (int *)(shm_addr+sizeof(int)*2);
        
        //共享内存第二段,存 key 列表
        keyPtr = (int *)(shm_addr+sizeof(int)*3);
        
        //存每个key的写入时间
        keyWriteTimePtr = (int*)(shm_addr+sizeof(int)*3 + sizeof(int)*max_size);
        
        //共享内存第三段,存 块占用标记 记录某个key对应的value数组存放的索引
        indexPtr = (int *)(shm_addr+sizeof(int)*3+sizeof(int)*max_size + sizeof(int)*max_size);
        //存内存使用标记 每块存储块是否被使用了
        flagPtr = (int *)(shm_addr+sizeof(int)*3 +sizeof(int)*max_size + sizeof(int)*max_size + sizeof(int)*max_size);
        //共享内存第四段,数据区域
        elementPtr = (T *)(shm_addr+sizeof(int)*3+sizeof(int)*max_size  +sizeof(int)*max_size + sizeof(int)*max_size + sizeof(int)*max_size);
        
        //第一次创建
        struct shmid_ds shmid_st;
        shmctl(shmid, IPC_STAT, &shmid_st);
        if (shmid_st.shm_cpid == getpid())
        {
            *sizePtr = 0;
            *minfreePtr = 0;
            for(int i=0;i<max_size;i++)
            {
                flagPtr[i] = 0;
                keyWriteTimePtr[i] = 0;
            }
        }
        
    }
    
    ~CGMShmMapKeyInt()
    {
    }
    
    T* add(int key,const T& obj)
    {       
        if((*sizePtr)>=max_size)
        {            
            return NULL;
        }
        int pos = 0;
        int flag = findPos(key,pos);
        if(flag)
        {
            //flagPtr[indexPtr[pos]] = 0;
            elementPtr[indexPtr[pos]] = obj;
        }
        else
        {
            for(int i=(*sizePtr);i>pos;i--)
            {
                keyPtr[i] = keyPtr[i-1];
                indexPtr[i] = indexPtr[i-1];
                keyWriteTimePtr[i] = keyWriteTimePtr[i-1];
            }
            int tpos = -1;
            for(int i=(*minfreePtr);i<max_size;i++)
            {
                if(flagPtr[i]==0)
                {
                    tpos = i;
                    break;
                }
            }
            flagPtr[tpos] = 1;
            (*minfreePtr) = tpos+1;
            keyPtr[pos] = key;
            indexPtr[pos] = tpos;
            elementPtr[tpos] = obj;
            (*sizePtr)++;
        }
        keyWriteTimePtr[pos] = time(NULL);
        return &elementPtr[indexPtr[pos]];
    }
    
    void del(int key)
    {
        int pos = 0;
        int flag = findPos(key,pos);
        if(flag)
        {
            flagPtr[indexPtr[pos]] = 0;
            if((*minfreePtr)>indexPtr[pos])
            {
                (*minfreePtr) = indexPtr[pos];
            }
            for(int i=pos+1;i<(*sizePtr);i++)
            {
                keyPtr[i-1] = keyPtr[i];
                indexPtr[i-1] = indexPtr[i];
                keyWriteTimePtr[i-1] = keyWriteTimePtr[i];
            }
            (*sizePtr)--;
        }
    }
    
    T& operator[](int key)
    {
        if((*sizePtr)>=max_size)
        {
            fprintf(stderr," shm full exception: file=[%s] line=[%d] code=[%d] ",__FILE__,__LINE__,MEXP_NEW);
            throw MEXP_NEW;
        }
        int pos = 0;
        int flag = findPos(key,pos);
        if(flag)
        {
            return elementPtr[indexPtr[pos]];
        }
        else
        {
            for(int i=(*sizePtr);i>pos;i--)
            {
                keyPtr[i] = keyPtr[i-1];
                indexPtr[i] = indexPtr[i-1];
            }
            int tpos = -1;
            for(int i=(*minfreePtr);i<max_size;i++)
            {
                if(flagPtr[i]==0)
                {
                    tpos = i;
                    break;
                }
            }
            flagPtr[tpos] = 1;
            (*minfreePtr) = tpos+1;
            keyPtr[pos] = key;
            indexPtr[pos] = tpos;
            T obj;
            elementPtr[tpos] = obj;
            (*sizePtr)++;
            return elementPtr[indexPtr[pos]];
        }
    }
    
    T* find(int key, bool &expired)
    {
        int pos = 0;
        int flag = findPos(key,pos);
        int curtime = time(NULL);
        expired = false;
        
        if(flag)
        {            
            if (keyWriteTimePtr[pos] + key_timeout_sec > curtime)
            {
                return &elementPtr[indexPtr[pos]];
            }
            else
            {                
                expired = true;
                return &elementPtr[indexPtr[pos]];
            }
        }
        else
        {
            return NULL;
        }
    }
    int getSize()
    {
        return (*sizePtr);
    }
    
    int getKey(int pos)
    {
        return keyPtr[pos];
    }
    
    T* get(int pos)
    {
        return &elementPtr[indexPtr[pos]];
    }
    
    void clear()
    {
        *sizePtr = 0;
        for(int i=0;i<max_size;i++)
        {
            flagPtr[i] = 0;
        }
        (*minfreePtr) = 0;
    }
protected:
    
private:
    //找到key所在的位置
    //找到key,返回1,pos指向所在位置
    //找不到key,返回0,pos指向所应该插入位置
    int findPos(int key,int &pos)
    {
        if((*sizePtr)==0)
        {
            pos = 0;
            return 0;
        }
        int p0 = 0;
        int p2 = (*sizePtr)-1;
        int p1 = (p0+p2)/2;
        while(true)
        {
            int cmp = 0;
            if(key>keyPtr[p1])
            {
                cmp = 1;
            }
            else if(key<keyPtr[p1])
            {
                cmp = -1;
            }
            else
            {
                cmp = 0;
            }
            if(cmp>0)
            {
                if(p2-p1>1)
                {
                    p0 = p1;
                    p1 = (p0+p2)/2;
                }
                else
                {
                    if(key>keyPtr[p2])
                    {
                        pos = p2+1;
                        return 0;
                    }
                    else if(key<keyPtr[p2])
                    {
                        pos = p1+1;
                        return 0;
                    }
                    else
                    {
                        pos = p2;
                        return 1;
                    }
                }
            }
            else if(cmp<0)
            {
                if(p1-p0>1)
                {
                    p2 = p1;
                    p1 = (p0+p2)/2;
                }
                else
                {
                    if(key<keyPtr[p0])
                    {
                        pos = p0;
                        return 0;
                    }
                    else if(key>keyPtr[p0])
                    {
                        pos = p0+1;
                        return 0;
                    }
                    else
                    {
                        pos = p0;
                        return 1;
                    }
                }
            }
            else
            {
                pos = p1;
                return 1;
            }
        }
    }    
    char * shm_addr;
    int max_size;
    int * minfreePtr;
    int * sizePtr;
    int * keyPtr;
    int * flagPtr;
    int * indexPtr;
    int * pGlobalTmout;
    int * keyWriteTimePtr;
    T* elementPtr;
    int key_timeout_sec;
};

#endif 
