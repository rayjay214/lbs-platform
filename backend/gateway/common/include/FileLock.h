// FileLock.h: interface for the CFileLock class.(file lock)
// Created by xin.qiu  2012.8.23
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELOCK_H__17F5D12D_E8E5_4CEE_B440_6A2F9E82C08F__INCLUDED_)
#define AFX_FILELOCK_H__17F5D12D_E8E5_4CEE_B440_6A2F9E82C08F__INCLUDED_

#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <string>

class CFileLock  
{
public:
    CFileLock(std::string fileName);
	~CFileLock();

 bool Lock();
 bool UnLock();


private:
    std::string m_FileName;
    int         m_fd;
    bool        m_bLock;

};

#endif // !defined(AFX_FILELOCK_H__17F5D12D_E8E5_4CEE_B440_6A2F9E82C08F__INCLUDED_)
