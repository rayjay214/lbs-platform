#ifndef __CTREE_PERMISSION__
#define __CTREE_PERMISSION__
// TODO; 2016.03.15 Ou Yibao计划将该文件从common库中移除
#include "TreeOp.h"
#include "gns_api.h"
#include "GRedisClient.h"
#include "string"

// TODO: 该文件的接口仅被亿级架构改造前的CPPBO，authbo和cppbocgi引用。建议放到svn/goocar/common下，不再放到全局common库里

namespace CTREE_PERMISSION
{
int ConnectToCTree(std::string caller, std::string callee);

// TODO: 请用svn/goocar/common/include/Gtree.h下的IsAncestor()接口替代此接口
//如果要查询的客户id是登陆客户id 的子客户，返回0，其他返回<0
int check_permission_byeid(std::string login_eid, uint64_t target_eid, std::string caller, std::string callee);

//根据uid拿到设备信息，拿不到 返回<0， 查到了，就拿到这个设备所属的客户id--》eid， 如果eid是登陆客户id 的子客户，就返回0，否则返回<0
int check_permission_byid(std::string login_eid, uint64_t target_userid, std::string caller, std::string callee);


//根据uid拿到设备信息，拿不到 返回<0， 查到了，就拿到这个设备所属的客户id--》eid， 如果eid是登陆客户id 的子客户，就返回0，否则返回<0
int check_permission_byimei(std::string login_eid, std::string target_imei, std::string caller, std::string callee);

}
#endif
