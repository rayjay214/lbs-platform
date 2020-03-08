#ifndef     __GOOME_PUBLIC_COMMON_RAII_H__
#define     __GOOME_PUBLIC_COMMON_RAII_H__


#include <stdio.h>

class ScopedFile
{
public:
    ScopedFile(FILE * fp): m_fp(fp){}
    virtual ~ScopedFile()
    {
        if (NULL != m_fp)
        {
            fclose(m_fp);
        }
    }
    FILE * m_fp;
};


#endif
