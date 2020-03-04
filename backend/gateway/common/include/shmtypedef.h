/**
*   Copyright (C) GOOME. All rights reserved.

  *   \file     shmtypedef.h
  *   \author   xin.qiu
  *   \date     2012.7.24
  *   \version  1.0.0
  *   \brief    
*/

#ifndef GM_SHM_TYPEDEF_H_H_H
#define GM_SHM_TYPEDEF_H_H_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <memory.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/times.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <linux/reboot.h>
#include <netdb.h>
#include <fcntl.h>

#include <math.h>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

#define MAXLEN 255
#define GOOGLEFILE "./data/google.txt"
#define NULL_DELTA 360
#define LOAD_TOTALFILE 1
#define LOAD_LINE   100000
#define CAL_LINE     10000
#define PRECISION 100

#define IPCKEY 0x366378
#define PRIMODE 0644
#define SHM_SIZE (1200*1024*1024)
#define SHM_GOOGLE_LAT_SEC_START (300*1024*1024)
#define SHM_BAIDU_START (600*1024*1024)
#define SHM_BAIDU_LAT_SEC_START (900*1024*0124)		// TODO: 这宏坑爹的写错了，导致现在共享内存里google地图数据是被百度覆盖的。要修复这个bug的话不但要所有机器都重做共享内存，还要让所有用到这个库的程序都升级……日了！

#define MAXLEN 255
#define DM2_DIS_MAX 16
#define MAX_POINT       10000
#define LOAD_FILE_NAME "/home/common/qiuxin/data/0.01offset.txt"
#define LOAD_FILE_RVS_NAME "/home/common/qiuxin/data/0.01offset_rvs.txt"
#define MAX_DOUBLE_DIS 99999999999999.0

#define PI                      3.14159265
#define EARTH_RADIUS            6378.137        //地球近似半径

#define CN_MIN_LON 73.40
#define CN_MAX_LON 135.00
#define CN_MIN_LAT 17.90
#define CN_MAX_LAT 53.68

#define LOAD_FILE_NAME_BAIDU0 "/home/common/qiuxin/data/offset-baidu.txt"
#define LOAD_FILE_NAME_BAIDU "/home/common/qiuxin/data/offset-baidu_.txt"
#define LOAD_FILE_NAME_BAIDU_RVS "/home/common/qiuxin/data/offset-baidu_rvs.txt"

using namespace std;

typedef struct  
{
    int loncount;   //以lon优先的lon个数 llistlon.size()s
    int latcount;   //lat第一维数组长度 listlat.size()
    
    int minlon;     //最小lon
    int maxlon;     //最大lon
    
    int minlat;     //最小lat
    int maxlat;     //最大lat
}HEAD_STAT;

typedef struct  
{
    unsigned long offsetsecd;//第二维度数组起始位置 (不能记物理地址，只能记偏移地址!!!)
    int           secdlen;  //第二纬度数组长度
    int           first;     //第一维度的值
}FIRST_DAT;

typedef struct  
{
    double  offsetx;
    double  offsety;
    int     second;
}SECOND_DAT;

typedef vector<SECOND_DAT> SEC_DAT_ARRAY;

typedef struct _first_item
{
    int             first;
    SEC_DAT_ARRAY   array;
}FIRST_ITEM;

typedef struct _dim2dis
{
    double lon;
    double lat;
    double offx;
    double offy;
    double dis;
}DIM2DIS;

typedef vector<FIRST_ITEM> LIST_LOAD;

typedef struct
{
    double  offsetx;
    double  offsety;
    double  second;
}LAT_LOG;

typedef struct  
{
    double inlon;
    double inlat;
    
    double nearlon;
    double nearlat;
    
    double offlat;
    double offlon;
    
    double distance;
}FIND_RES_DETAIL;

typedef vector<LAT_LOG> LAT_LIST;

typedef map<double, LAT_LIST> LON_LAT_LIST;

using namespace std;

typedef struct
{
    double lon;
    double lat;
}GEO_POS;

typedef std::vector<GEO_POS> GEO_POS_LIST;

bool load_file(bool bgoogle = true, bool byLon=true);

bool insert_sec_array(SEC_DAT_ARRAY &latArry, int lat, double offsetx, double offsety);
bool insert_pt(bool bgoogle, bool byLon, int firstdat, int secdat, double offx, double offy);

int  binary_search_bylon(bool byLon, double goal, int& lowpos, int& highpos, double* nearlon, bool bgoogle=true);
int  bin_search_bylat(bool byLon, int firstpos, double inputlat, int& low2, int& high2, LAT_LOG* nearlat, bool bgoogle);
void bin_search_lat_wrap(bool byLon, int firstpos, double mylon, double inputlat, 
                         int& low2, int& high2, LAT_LOG* nearlat, bool bgoogle);

void   fin_lonlat_bylon(bool bylon, double inputlon, double inputlat, FIND_RES_DETAIL* pFindRes, bool bgoogle=true);

bool   shm_data_init(void* pAddr);

bool   fill_find_res_detail(double inputlon, double inputlat, FIND_RES_DETAIL *pFindRes);

double cal_p2p_dis(double x1, double y1, double x2, double y2);

//rvs map gen
bool  google_gen_rvs_adjdat();//google

//tow points distance
double radian(double d);
double get_distance(double lat1, double lng1, double lat2, double lng2);

//data
extern LIST_LOAD     g_listLon;
extern LIST_LOAD     g_listLat;

extern LIST_LOAD     g_listLonBaidu;
extern LIST_LOAD     g_listLatBaidu;

extern LON_LAT_LIST  g_search_result;
extern DIM2DIS       g_dim2_dis[DM2_DIS_MAX];
extern double        g_elapseUSecond[MAX_POINT];
extern unsigned long g_shmaddr;

extern std::string g_google_lon_file;
extern std::string g_google_lat_file;
extern std::string g_baidu_lon_file;
extern std::string g_baidu_lat_file;

#endif
