/********************************************************************
 * Copyright (C), 2009-2012, Goome Tech. Co., Ltd.
 * File name:   common.h
 * Author:      Cen Jie
 * Version:     1.0
 * Date:        2012-7-18
 * Description: 
 * Function List:  主要函数列表，每条记录应包括函数名及功能简要说明
*********************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
//#include "svshm_gs.h"
#include <math.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
//#include "tbb/concurrent_unordered_map.h"
//#include "tbb/concurrent_hash_map.h"
//#include "tbb/concurrent_queue.h"

//#include "global_hdr.h"
//#include "inet_sockets.h"
//#include "rdwrn.h"
//#include "restart.h"
//#include "uici.h"
//#include <IceUtil/Thread.h>
//#include <Ice/Ice.h>
#include <sstream>
#include "global.h"
#include "gns_api.h"

#define T_FOTA_IMEI2UID "imei:" //imei
#define T_FOTA_DEV_RUNINFO "devruninfo:" //uid
#define T_FOTA_DEV_BASEINFO "device:" //uid
#define T_FOTA_BMS_INFO "bms:" //uid


int keepalive(int cfd, int expir);
int set_nonblock(int socket);

uint8_t HexCharToInt(char c);
uint64_t HexStrToUint64(const std::string &hex);
void ImeiDecToHex(uint64_t ullImei, std::string& strImei);
uint64_t ConvertDecToSameHex(uint64_t decImei);
uint64_t TransformIp2Sim(const char* pIpstr);
void DebugBinaryStr(const char* szStr, unsigned int szLen);

bool GetRedisDevId(std::string &strImei, uint64_t &devId);
bool saveRedisDevRunInfo(uint64_t uid, std::map<std::string, std::string> &mpParam);
bool saveRedisBmsInfo(uint64_t uid, std::map<std::string, std::string> &mpParam);
bool saveRedisIccidInfo(uint64_t uid, std::string &strIccid);
std::string getRedisHashVal(const std::string &key, const std::string &filed);


std::string ToGpsRecordStr(E_PROTOCOL_NO protocol_no, SSMap &mpParam);
uint16_t generateSeqno();

#endif

