/**
 *   Copyright (C) GOOME. All rights reserved.

 *   \file     SdkShmLonLatAdjust.h
 *   \author   xin.qiu
 *   \date     2012.7.24
 *   \version  1.0.0
 *   \brief      GOOGLE地图经纬度校正SDK头文件
 */

#ifndef GM_SDK_SHM_LONLAT_ADJUST_H_H_H
#define GM_SDK_SHM_LONLAT_ADJUST_H_H_H

#include "pinpoly.h"

typedef struct 
{
    double inputlon;
    double inputlat;
    double nearestlon;
    double nearestlat;
    double offsetlon;
    double offsetlat;
}ADJUST_DTL;


//////////////////////////////////////////////////////////////////////////
//返回0表示调用成功，返回-1表示获取共享内存失败
//pDtl返回详细校正信息，不需要详细信息赋值NULL即可（默认）
//如果点在国外，则lon，lat原样返回
int GMSDK_ShmLonLatAdjust(double& lon, double& lat, MAPTYPE maptype = MAPTYPE_GOOGLE, ADJUST_DTL* pDtl = NULL);

//反校准
int GMSDK_ShmLonLatAdjRvs(double& lon, double& lat, MAPTYPE maptype = MAPTYPE_GOOGLE, ADJUST_DTL* pDtl = NULL);

#endif

