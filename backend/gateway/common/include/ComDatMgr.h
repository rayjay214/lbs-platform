/* 
	�������ݹ����ڴ������
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

    bool            AddUser(int usrid, GMDB_USER &usr);//���� �޸� ���Ǵ˽ӿ�
    GMDB_USER*      GetUser(int usrid, bool &expired);
    bool            DelUser(int usrid);
    int             SizeUser();
    
    bool            AddUserExt(int usrid, GMDB_USER_EXT &usr);//���� �޸� ���Ǵ˽ӿ�
    GMDB_USER_EXT*  GetUserExt(int usrid, bool &expired);
    bool            DelUserExt(int usrid);
    int             SizeUserExt();

    bool            AddSchoolUsrGroup(int schoolid, GMDB_SCHOOL_USRGROP &item);//��ʼ����ʱ������ �ô˽ӿ�

    bool            GmqAddSchoolUsrGroup(int schoolid, GMDB_USERGROUP &item); //GMQ��t_user_group����������1����ʱ������
    bool            GmqDelSchoolUsrGroup(int schoolid, int groupid); //GMQ��t_user_group������ɾ��1����ʱ������
    bool            GmqEdtSchoolUsrGroup(int schoolid, GMDB_USERGROUP &item);//GMQ��t_user_group�����и���1����¼��ʱ������

    GMDB_SCHOOL_USRGROP* GetSchoolUsrGroup(int schoolid, bool &expired);//�ӹ����ڴ������
    bool                DelSchoolUsrGroup(int schoolid);//ɾ������schoolid�µ�user_groups
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
