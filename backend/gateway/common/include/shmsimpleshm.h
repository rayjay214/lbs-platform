/**
*   Copyright (C) GOOME. All rights reserved.

  *   \file     shmsimpleshm.h
  *   \author   xin.qiu
  *   \date     2012.7.24
  *   \version  1.0.0
  *   \brief    
*/
#ifndef GM_SIMPLE_SHM_CLASS_H_H_H
#define GM_SIMPLE_SHM_CLASS_H_H_H

#include "shmtypedef.h"

enum SHM_GET_MODE
{
    MYSHM_RDONLY,
        MYSHM_RW,
};

class CSimpleShm
{
public:
    CSimpleShm();
    ~CSimpleShm();
    
    bool    Create(int size);
    
    void*   GetShm(int size, SHM_GET_MODE mode);
    bool    UnUseShm(char* shmaddr);
    
    bool    RemoveShm(int size);//remove global shm
    
protected:
private:
    int     m_shmid_create;
    void*   m_addr;
};

#endif
