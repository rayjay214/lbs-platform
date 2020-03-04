/********************************************************************
 * Copyright (C), 2009-2012, Goome Tech. Co., Ltd.
 * File name:   common.cpp
 * Author:      cen jie
 * Version:     1.0
 * Date:        2012-7-18
 * Description: 公共定义
 * Function List:  主要函数列表，每条记录应包括函数名及功能简要说明
*********************************************************************/
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <string>

#include <stdlib.h>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include "common.h"
#include "string.h"
#include "tostring.h"
#include "StringUtility.h"
#include "RedisOp.h"
#include "RedisConnectPool.h"

int keepalive(int cfd, int expir)
{
	int keepAlive = 1;
	int keepIdle = expir;
	int keepInterval = 10;
	int keepCount = 3;

	if(setsockopt(cfd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive)) == -1)
	{
		return -1;
	}

	if(setsockopt(cfd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle)) == -1)
	{
		return -1;
	}

	if(setsockopt(cfd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval)) == -1)
	{
		return -1;
	}

	if(setsockopt(cfd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount)) == -1)
	{
		return -1;
	}

	return 0;
}

int set_nonblock(int socket)
{
	int flags;

	if((flags = fcntl(socket, F_GETFL, 0)) < 0)
	{
		return -1;
	}

	if(fcntl(socket, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		return -1;
	}

	return 0;
}



uint8_t HexCharToInt(char c)
{
    
    if(c >= '0' && c <= '9')
        return c - '0';
    else if( c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if ( c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else
        return 0;
}

uint64_t HexStrToUint64(const std::string &hex)
{
    std::string strHex(hex);
    if (strHex.empty())
    {
        return 0;
    }

    if (strHex.size() % 2 != 0)
    {
        strHex.insert(strHex.begin(),'0');
    }

    //最多只处理8字节 16位
    if (strHex.size() > 16)
    {
        strHex = strHex.substr(strHex.size()-16);
    }

    uint64_t result = 0;
    unsigned char uH = 0, uL = 0;
    for (size_t idx = 0; idx < strHex.size(); idx+=2)
    {
        uH = HexCharToInt(strHex[idx]);
        uL = HexCharToInt(strHex[idx+1]);
        result |= (uint64_t)((uH << 4) | uL) << (7 - idx/2)*8;
    }

    return result;
}

void ImeiDecToHex(uint64_t ullImei, std::string& strImei)
{
    char buf[32] = {0};
    sprintf(buf, "%08x%08x", (uint32_t)(ullImei >> 32), (uint32_t)(ullImei & 0xffffffff));  
    strImei = buf;
    strImei = strImei.substr(1);
}

uint64_t ConvertDecToSameHex(uint64_t decImei)
{
	uint64_t result = 0;
	int index = 0;
	while(true)
	{
		if (decImei == 0)
			break;

		int tail = decImei%10;
		decImei /= 10;
		uint64_t incre = tail * pow(16,index);
		result += incre;
		index ++;
	}
	return result;
}

uint64_t TransformIp2Sim(const char* pIpstr)
{
	if (pIpstr == NULL)
	{
		return 0;
	}

	uint64_t result = 0;

	char cIpFirstByte = *pIpstr;
	char cIpSecondByte = *(pIpstr+1);
	char cIpThirdByte = *(pIpstr+2);
	char cIpFourthByte = *(pIpstr+3);

	if (cIpFirstByte & 0x80)
	{
		result |= 0x08;
	}
	if (cIpSecondByte & 0x80)
	{
		result |= 0x04;
	}
	if (cIpThirdByte & 0x80)
	{
		result |= 0x02;
	}
	if (cIpFourthByte & 0x80)
	{
		result |= 0x01;
	}
	result += 130;

	cIpFirstByte &= 0x7F;
	cIpSecondByte &= 0x7F;
	cIpThirdByte &= 0x7F;
	cIpFourthByte &= 0x7F;

	uint64_t uImeiNumber = 0;
	uImeiNumber = result * 100000000 + (uint64_t)cIpFirstByte * 1000000 + (uint64_t)cIpSecondByte * 10000 + (uint64_t)cIpThirdByte * 100 + (uint64_t)cIpFourthByte;

	return uImeiNumber;
}

void DebugBinaryStr(const char* szStr, unsigned int szLen)
{
    std::string str = Goome::Binary2HexFormatString(szStr, szLen);
    MYLOG_WARN(g_logger,"%s", str.c_str());
}

bool GetRedisDevId(std::string &strImei, uint64_t &devId)
{
    CRedisClient *client = (CRedisClient *)RedisOp::GetRedisClient("status_w");
    AutoRelease release(client);
    if(!client)
    {
        MYLOG_WARN(g_logger,"GetRedisClient. strImei=%s, error=%s", strImei.c_str(), RedisOp::GetErrorMsg().c_str());
        return false;
    }

    //
    std::string strKey = T_FOTA_IMEI2UID;
    strKey += strImei;

    std::string strVal;
    if (!client->Get(strKey, strVal))
    {
        MYLOG_WARN(g_logger,"redis Get failed. key=%s", strKey.c_str());
        return false;
    }

    devId = atoll(strVal.c_str());
    return true;
}


bool saveRedisDevRunInfo(uint64_t uid, std::map<std::string, std::string> &mpParam)
{
    if (mpParam.empty())
    {
        return true;
    }
    
    CRedisClient *client = (CRedisClient *)RedisOp::GetRedisClient("status_w");
    AutoRelease release(client);
    if(!client)
    {
        MYLOG_WARN(g_logger,"GetRedisClient. uid=%lu, error=%s", uid, RedisOp::GetErrorMsg().c_str());
        return false;
    }

    //
    std::string strKey = T_FOTA_DEV_RUNINFO;
    strKey += ToString(uid);

    std::string strParam;
    std::vector<std::string> vecFields;
    std::vector<std::string> vecValues;
    std::map<std::string, std::string>::iterator iter = mpParam.begin();
    for (; iter != mpParam.end(); ++iter)
    {
        vecFields.push_back(iter->first);
        vecValues.push_back(iter->second);
        strParam += iter->first + ":";
        strParam += iter->second + ";";
    }
    if (!client->HMSet(strKey, vecFields, vecValues))
    {
        MYLOG_WARN(g_logger, "redis HMSet failed. uid=%lu, strParam=%s", uid, strParam.c_str());
        return false;
    }
    return true;
}

bool saveRedisBmsInfo(uint64_t uid, std::map<std::string, std::string> &mpParam)
{
    if (mpParam.empty())
    {
        return true;
    }
    
    CRedisClient *client = (CRedisClient *)RedisOp::GetRedisClient("status_w");
    AutoRelease release(client);
    if(!client)
    {
        MYLOG_WARN(g_logger,"GetRedisClient. uid=%lu, error=%s", uid, RedisOp::GetErrorMsg().c_str());
        return false;
    }

    //
    std::string strKey = T_FOTA_BMS_INFO;
    strKey += ToString(uid);

    std::vector<std::string> vecFields;
    std::vector<std::string> vecValues;
    std::map<std::string, std::string>::iterator iter = mpParam.begin();
    for (; iter != mpParam.end(); ++iter)
    {
        vecFields.push_back(iter->first);
        vecValues.push_back(iter->second);
    }
    if (!client->HMSet(strKey, vecFields, vecValues))
    {
        MYLOG_WARN(g_logger, "redis HMSet failed. uid=%lu", uid);
        return false;
    }
    return true;
}


bool saveRedisIccidInfo(uint64_t uid, std::string &strIccid)
{
    CRedisClient *client = (CRedisClient *)RedisOp::GetRedisClient("status_w");
    AutoRelease release(client);
    if(!client)
    {
        MYLOG_WARN(g_logger,"GetRedisClient. uid=%lu, error=%s", uid, RedisOp::GetErrorMsg().c_str());
        return false;
    }

    //
    std::string strKey = T_FOTA_DEV_BASEINFO;
    strKey += ToString(uid);

    std::string strField = "iccid";

    if (!client->HSet(strKey, strField, strIccid))
    {
        MYLOG_WARN(g_logger, "redis HMSet failed. uid=%lu, strIccid=%s", uid, strIccid.c_str());
        return false;
    }
    return true;
}




std::string ToGpsRecordStr(E_PROTOCOL_NO protocol_no, SSMap &mpParam)
{
    /*
    dev_id bigint,
    report_time timestamp,
    direction int,
    latitude int,
    longitude int,
    protocol_no int,
    seq_no int,
    speed int,
    sys_time timestamp,
    */

    /*
     1) "acc_status"
     2) "1"
     3) "alarmcode"
     4) "0"
     5) "alt"
     6) "0"
     7) "course"
     8) "333"
     9) "gps_status"
    10) "2"
    11) "gps_time"
    12) "1579246981"
    13) "lat"
    14) "23.897262"
    15) "lng"
    16) "115.745576"
    17) "speed"
    18) "670"
    19) "status"
    20) "3"
    21) "sys_time"
    22) "1579251299"
    */

    std::string strRecord;
    std::ostringstream oss;
    oss << "gps" << "|";

    const char *pKeyArr[] = {
        "devid",
        "gps_time",
        "course",
        "latitude",
        "longitude",
        "protocol_no",
        "seq_no",
        "speed",
        "sys_time"
    };

    std::string strKey;
	for (unsigned int idx = 0; idx < sizeof(pKeyArr)/sizeof(pKeyArr[0]); ++idx)
	{
        strKey = pKeyArr[idx];

        if (strKey == "protocol_no")
        {
            oss << protocol_no << ";";
        }
        else if (strKey == "seq_no")
        {
            oss << 0 << ";";
        }
        else
        {
            SSMap::iterator iter = mpParam.find(strKey);
    	    if (iter == mpParam.end())
    	    {
    	        MYLOG_DEBUG(g_logger, "value not exist! strKey=%s", strKey.c_str());
                return strRecord;
    	    }

            oss << iter->second << ";";
        }
	}

    strRecord = oss.str();
    if (!strRecord.empty())
    {
        Goome::TrimRight(strRecord, ";");
    }

    return strRecord;
}

std::string getRedisHashVal(const std::string &key, const std::string &filed)
{
    std::string strVal;
    do{
        CRedisClient *client = (CRedisClient *)RedisOp::GetRedisClient("status_w");
        AutoRelease release(client);
        if(!client)
        {
            MYLOG_WARN(g_logger,"GetRedisClient. key=%s, filed=%s, error=%s", 
                key.c_str(), filed.c_str(), RedisOp::GetErrorMsg().c_str());
            break;
        }

        //
        
        if (!client->HGet(key, filed, strVal))
        {
            MYLOG_WARN(g_logger, "redis HGet failed. key=%s, filed=%s", key.c_str(), filed.c_str());
            break;
        }
    }while(0);
    return strVal;
}


uint16_t generateSeqno()
{
    static uint16_t s_seqno = 0;
    if (s_seqno > 0xFFF0)
        s_seqno = 0;

    return s_seqno++;
}


