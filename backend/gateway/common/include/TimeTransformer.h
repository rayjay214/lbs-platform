// 时间格式转化常用函数

#ifndef __H_TIME_TRANSFORMER_
#define __H_TIME_TRANSFORMER_

#include <time.h>
#include <stdint.h>
#include <string>

namespace Goome
{
    // BCD格式字符串长度限制设定
    const int kBcdSize6 = 6;
    const int kBcdSize7 = 7;


    // 获取当前时间的微秒形式
    // Return: 当前时间(us)
    uint64_t GetTimeMicrosecond();
    
    // 获取当前时间的年毫秒形式
    // Return: 当前时间(ms)
    uint64_t GetTimeMillisecond();

    // 将unix时间转化为特定格式的时间字符串（如"2014-05-08 10:27:27"）
    // Input:   t           unix时间
    // Input:   pFMT        pTimeStr的格式，默认为支持"2014-05-08 10:27:27"这样的格式
    // Return:  t对应的时间字符串，若失败，返回串内容为空
    std::string UnixTime2TimeString(time_t t, const char * pFMT = "%Y-%m-%d %T");

    // 将特定格式的时间字符串（如"2014-05-08 10:27:27"）转化为unix时间
    // Input:   pTimeStr    时间字符串
    // Input:   pFMT        pTimeStr的格式，默认为支持"2014-05-08 10:27:27"这样的格式
    // Return:  若转化成功，为对应的unix时间值；若转化失败，返回-1
    time_t TimeString2UnixTime(const char * pTimeStr, const char * pFMT = "%Y-%m-%d %T");

    // 将16进制的字符转化为10进制
    // Input:   c   待转化的16进制字符
    // Return:      转化后的10进制字符
    inline char CharHex2Dec(char c)
    {
        return (c/16)*10 + c % 16;
    }

    // 将10进制的字符转化为16进制
    // Input:   c   待转化的10进制字符
    // Return:      转化后的16进制字符
    inline char CharDec2Hex(char c)
    {
        return (c/10)*16 + c % 10;
    }

    // 将BCD格式的字符串转化为unix时间值
    // Input:   pBCD    待转化的BCD格式的字符串
    // Input:   dwLen   BCD格式的字符串长度，当前该函数仅支持6字节BCD字符串，格式定死。保留该参数供扩展用
    // Return:  对应的unix时间值
    time_t BCD2Time(const char * pBCD, int dwLen);

    // 将unix时间值转化为BCD格式的字符串
    // Input:   dwTime      待转化的unix时间值
    // Input:   dwBcdLen    BCD格式的字符串长度，当前该函数支持6或7字节BCD字符串，格式定死
    // Output:  pBuf        保存转化后的BCD字符串的缓冲区，长度至少为dwBcdLen字节
    // Return:  对应的BCD格式的字符串
    char * Time2BCD(time_t dwTime, int dwBcdLen, char * pBuf);
}

#endif  // __H_TIME_TRANSFORMER_


