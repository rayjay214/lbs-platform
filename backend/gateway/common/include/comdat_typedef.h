/* 
	common library for share mem common data 
		Qiu Xin @ 2012-08-24
*/
#ifndef COM_DAT_SHM_COMMON_H_H_H
#define COM_DAT_SHM_COMMON_H_H_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  SCHOOL_USRGROUP_MAX_SIZE 100
#define MAX_UID_LEN 600

typedef unsigned int UINT;

typedef struct
{
    int     user_id; //`USER_ID` bigint(20) NOT NULL,
    int     school_id;//`SCHOOL_ID` bigint(12) NOT NULL,
    char    tel[20]; //`TEL` varchar(20) NOT NULL COMMENT '联系人电话',
    char    sim_id[15];//`SIM_ID` varchar(15) CHARACTER SET gbk DEFAULT NULL,
    char    phone_num[15];//`PHONE_NUM` varchar(15) DEFAULT NULL COMMENT '设备电话
    char    user_name[20];//`USER_NAME` varchar(20) CHARACTER SET gbk NOT NULL,
    char    pswd[20];//`PAWSS` char(20) DEFAULT NULL,
    char    status[5];//`STATUS` char(5) NOT NULL COMMENT '0:刚注册、1：已有GPS数据、10：无效用户',
    char    grade[5];//`GRADE` varchar(5) NOT NULL COMMENT '年级、速度阀值',
    char    classes[5];//`CLASS` varchar(5) NOT NULL COMMENT '班级、急刹车阀值',

    char    createtime[24];//`CREATE_TIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    char    sex[20];//`SEX` varchar(20) CHARACTER SET gbk DEFAULT '0' COMMENT '性别、急加速阀值',
    char    use_time[24];//`USE_TIME` timestamp NULL DEFAULT NULL,
    //char    remark[200];//`REMARK` varchar(200) CHARACTER SET gbk DEFAULT NULL,
}__attribute__((packed)) GMDB_USER;

typedef struct  
{
    UINT      checksum;
    GMDB_USER usrone;
}__attribute__((packed))GMDB_USER_CHK;

typedef struct  
{
    int  user_id; //`USER_ID` bigint(12) NOT NULL,
    char os_id[50];//`OS_ID` varchar(50) DEFAULT '0',
    char sale_type[5];//`SALE_TYPE` varchar(5) DEFAULT '0',
    char service_flag[5];//`SERVICE_FLAG` varchar(5) DEFAULT '1',
    char product_type[20];//`PRODUCT_TYPE` varchar(20) CHARACTER SET gbk DEFAULT 'GT02',
    char icontype[20];//`ICONTYPE` varchar(20) DEFAULT '22',
    char tz_interval[10];//`TZ_INTERVAL` varchar(10) NOT NULL DEFAULT '0',
    //char owner[100];//`owner` varchar(100) CHARACTER SET gbk DEFAULT '  ',
    int  school_id_bef;//`school_id_bef` bigint(12) unsigned DEFAULT '0',
    char create_time[24];//`create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    char out_time[24];//`out_time` timestamp NULL DEFAULT NULL,
    //char car_version[100];//`CAR_VERSION` varchar(100) DEFAULT NULL COMMENT '设备版本',
    char car_locale[20];//`CAR_LOCALE` varchar(20) DEFAULT NULL COMMENT '设备语言',
    char car_length[20];//`CAR_LENGTH` varchar(20) DEFAULT NULL COMMENT '车辆长度',
    char car_weight[20];//`CAR_WEIGHT` varchar(20) DEFAULT NULL COMMENT '车辆载重',
    char car_color[20];//`CAR_COLOR` varchar(20) DEFAULT NULL COMMENT '车牌颜色',
    char car_servicestate[20];//`CAR_SERVICESTATE` varchar(20) DEFAULT NULL COMMENT '设备服务状态',
    //char remarks[200];
}__attribute__((packed)) GMDB_USER_EXT;

typedef struct  
{
    UINT checksum;
    GMDB_USER_EXT user_ext_one;
}__attribute__((packed)) GMDB_USER_EXT_CHK;

typedef struct  
{
    int  group_id;
    char group_name[50];
}__attribute__((packed)) GMDB_USERGROUP;

typedef struct  
{
    int school_id;
    GMDB_USERGROUP ugropones[SCHOOL_USRGROUP_MAX_SIZE];
}__attribute__((packed)) GMDB_SCHOOL_USRGROP;

typedef struct  
{
    UINT checksum;
    GMDB_SCHOOL_USRGROP school_usrgrp_one;
}__attribute__((packed)) GMDB_SCHOOL_USRGROP_CHK;

typedef struct 
{
    int uid[MAX_UID_LEN];
}__attribute__((packed)) MYUIDS;

typedef struct 
{
    UINT            checksum;
    MYUIDS          data;    
}__attribute__((packed)) MYUIDS_CHK;

bool                    comdat_shm_init(int timeoutsec);//init common data share memory, need be called when app init

GMDB_USER*              comdat_shm_get_user(int userid); //get user info from common data share mem
GMDB_USER_EXT*          comdat_shm_get_user_ext(int userid);//get user extend info from common data share mem
GMDB_SCHOOL_USRGROP*    comdat_shm_get_school_usrgroup(int schoolid);//get user groups under a schoolid
MYUIDS*                 comdat_shm_get_eiduids(int eid, bool &expired); //get uids of a eid
bool                    comdat_shm_set_eiduids(int eid, MYUIDS &uids);

#endif

