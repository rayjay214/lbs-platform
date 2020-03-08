/* 
	基础数据共享内存管理类
		Qiu Xin @ 2012-08-23
*/
#if !defined(AFX_COMDATSHMMGR_H__519DD2D7_614C_4496_8C30_FEEACF36946A__INCLUDED_)
#define AFX_COMDATSHMMGR_H__519DD2D7_614C_4496_8C30_FEEACF36946A__INCLUDED_

#include "comdat_typedef.h"
#include "shmmap.h"
#include "FileLock.h"

class CComDatMgr  
{
    typedef CGMShmMapKeyInt<GMDB_USER_CHK>     GMDB_USER_MAP_CACHE;
    typedef CGMShmMapKeyInt<GMDB_USER_EXT_CHK> GMDB_USER_EXT_MAP_CACHE;
    typedef CGMShmMapKeyInt<GMDB_SCHOOL_USRGROP_CHK> GMDB_SCHOOL_USRGROP_MAP_CACHE;
    typedef CGMShmMapKeyInt<MYUIDS_CHK>              GMBIZ_EID_UIDS_CACHE;

public:
	static CComDatMgr* InitShareMem();
    static CComDatMgr* Inst();
    static void        InitMaxSizeTimemout(int user_max_size, int user_ext_maxsize, int school_usrgroup_maxsize, 
        int eid_uids_maxsize, int key_timeout);

	virtual ~CComDatMgr();

    bool            AddUser(int usrid, GMDB_USER &usr);//增加 修改 都是此接口
    GMDB_USER*      GetUser(int usrid, bool &expired);
    bool            DelUser(int usrid);
    int             SizeUser();
    
    bool            AddUserExt(int usrid, GMDB_USER_EXT &usr);//增加 修改 都是此接口
    GMDB_USER_EXT*  GetUserExt(int usrid, bool &expired);
    bool            DelUserExt(int usrid);
    int             SizeUserExt();

    bool            AddSchoolUsrGroup(int schoolid, GMDB_SCHOOL_USRGROP &item);//初始化的时候增加 用此接口

    bool            GmqAddSchoolUsrGroup(int schoolid, GMDB_USERGROUP &item); //GMQ在t_user_group表里有新增1条的时候增加
    bool            GmqDelSchoolUsrGroup(int schoolid, int groupid); //GMQ在t_user_group表里有删除1条的时候增加
    bool            GmqEdtSchoolUsrGroup(int schoolid, GMDB_USERGROUP &item);//GMQ在t_user_group表里有更新1条记录的时候增加

    GMDB_SCHOOL_USRGROP* GetSchoolUsrGroup(int schoolid, bool &expired);//从共享内存里面查
    bool                DelSchoolUsrGroup(int schoolid);//删除整个schoolid下的user_groups
    int                 SizeSchoolUsrGroup();

    bool                AddEidUids(int eid, MYUIDS &uids);
    MYUIDS*             GetEidUids(int eid, bool &expired);
    bool                DelEidUids(int eid);
    int                 SizeEidUids();

    UINT            CheckSum(UINT* buffer, int size);

protected:
    CComDatMgr();

private:
    GMDB_USER_MAP_CACHE     m_map_user;
    GMDB_USER_EXT_MAP_CACHE m_map_user_ext;
    GMDB_SCHOOL_USRGROP_MAP_CACHE m_map_school_usrgrop;
    GMBIZ_EID_UIDS_CACHE            m_map_eid_uids;

    CFileLock m_FileLockUser;
    CFileLock m_FileLockUserExt;
    CFileLock m_FileLockUsrGrop;
    CFileLock m_FileLockEidUids;

    static int  m_user_maxsize;
    static int  m_user_ext_maxsize;
    static int  m_school_usrgroup_maxsize;
    static int  m_eid_uids_maxsize;
    static int  m_timeout;//second

    static bool m_bMemInited;
    static bool m_bParamInit;
};

#endif // 
