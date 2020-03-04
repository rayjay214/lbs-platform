#ifndef __CTREE_PERMISSION__
#define __CTREE_PERMISSION__
// TODO; 2016.03.15 Ou Yibao�ƻ������ļ���common�����Ƴ�
#include "TreeOp.h"
#include "gns_api.h"
#include "GRedisClient.h"
#include "string"

// TODO: ���ļ��Ľӿڽ����ڼ��ܹ�����ǰ��CPPBO��authbo��cppbocgi���á�����ŵ�svn/goocar/common�£����ٷŵ�ȫ��common����

namespace CTREE_PERMISSION
{
int ConnectToCTree(std::string caller, std::string callee);

// TODO: ����svn/goocar/common/include/Gtree.h�µ�IsAncestor()�ӿ�����˽ӿ�
//���Ҫ��ѯ�Ŀͻ�id�ǵ�½�ͻ�id ���ӿͻ�������0����������<0
int check_permission_byeid(std::string login_eid, uint64_t target_eid, std::string caller, std::string callee);

//����uid�õ��豸��Ϣ���ò��� ����<0�� �鵽�ˣ����õ�����豸�����Ŀͻ�id--��eid�� ���eid�ǵ�½�ͻ�id ���ӿͻ����ͷ���0�����򷵻�<0
int check_permission_byid(std::string login_eid, uint64_t target_userid, std::string caller, std::string callee);


//����uid�õ��豸��Ϣ���ò��� ����<0�� �鵽�ˣ����õ�����豸�����Ŀͻ�id--��eid�� ���eid�ǵ�½�ͻ�id ���ӿͻ����ͷ���0�����򷵻�<0
int check_permission_byimei(std::string login_eid, std::string target_imei, std::string caller, std::string callee);

}
#endif
