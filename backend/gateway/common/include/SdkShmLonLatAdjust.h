/**
 *   Copyright (C) GOOME. All rights reserved.

 *   \file     SdkShmLonLatAdjust.h
 *   \author   xin.qiu
 *   \date     2012.7.24
 *   \version  1.0.0
 *   \brief      GOOGLE��ͼ��γ��У��SDKͷ�ļ�
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
//����0��ʾ���óɹ�������-1��ʾ��ȡ�����ڴ�ʧ��
//pDtl������ϸУ����Ϣ������Ҫ��ϸ��Ϣ��ֵNULL���ɣ�Ĭ�ϣ�
//������ڹ��⣬��lon��latԭ������
int GMSDK_ShmLonLatAdjust(double& lon, double& lat, MAPTYPE maptype = MAPTYPE_GOOGLE, ADJUST_DTL* pDtl = NULL);

//��У׼
int GMSDK_ShmLonLatAdjRvs(double& lon, double& lat, MAPTYPE maptype = MAPTYPE_GOOGLE, ADJUST_DTL* pDtl = NULL);

#endif

