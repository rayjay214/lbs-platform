// ʱ���ʽת�����ú���

#ifndef __H_TIME_TRANSFORMER_
#define __H_TIME_TRANSFORMER_

#include <time.h>
#include <stdint.h>
#include <string>

namespace Goome
{
    // BCD��ʽ�ַ������������趨
    const int kBcdSize6 = 6;
    const int kBcdSize7 = 7;


    // ��ȡ��ǰʱ���΢����ʽ
    // Return: ��ǰʱ��(us)
    uint64_t GetTimeMicrosecond();
    
    // ��ȡ��ǰʱ����������ʽ
    // Return: ��ǰʱ��(ms)
    uint64_t GetTimeMillisecond();

    // ��unixʱ��ת��Ϊ�ض���ʽ��ʱ���ַ�������"2014-05-08 10:27:27"��
    // Input:   t           unixʱ��
    // Input:   pFMT        pTimeStr�ĸ�ʽ��Ĭ��Ϊ֧��"2014-05-08 10:27:27"�����ĸ�ʽ
    // Return:  t��Ӧ��ʱ���ַ�������ʧ�ܣ����ش�����Ϊ��
    std::string UnixTime2TimeString(time_t t, const char * pFMT = "%Y-%m-%d %T");

    // ���ض���ʽ��ʱ���ַ�������"2014-05-08 10:27:27"��ת��Ϊunixʱ��
    // Input:   pTimeStr    ʱ���ַ���
    // Input:   pFMT        pTimeStr�ĸ�ʽ��Ĭ��Ϊ֧��"2014-05-08 10:27:27"�����ĸ�ʽ
    // Return:  ��ת���ɹ���Ϊ��Ӧ��unixʱ��ֵ����ת��ʧ�ܣ�����-1
    time_t TimeString2UnixTime(const char * pTimeStr, const char * pFMT = "%Y-%m-%d %T");

    // ��16���Ƶ��ַ�ת��Ϊ10����
    // Input:   c   ��ת����16�����ַ�
    // Return:      ת�����10�����ַ�
    inline char CharHex2Dec(char c)
    {
        return (c/16)*10 + c % 16;
    }

    // ��10���Ƶ��ַ�ת��Ϊ16����
    // Input:   c   ��ת����10�����ַ�
    // Return:      ת�����16�����ַ�
    inline char CharDec2Hex(char c)
    {
        return (c/10)*16 + c % 10;
    }

    // ��BCD��ʽ���ַ���ת��Ϊunixʱ��ֵ
    // Input:   pBCD    ��ת����BCD��ʽ���ַ���
    // Input:   dwLen   BCD��ʽ���ַ������ȣ���ǰ�ú�����֧��6�ֽ�BCD�ַ�������ʽ�����������ò�������չ��
    // Return:  ��Ӧ��unixʱ��ֵ
    time_t BCD2Time(const char * pBCD, int dwLen);

    // ��unixʱ��ֵת��ΪBCD��ʽ���ַ���
    // Input:   dwTime      ��ת����unixʱ��ֵ
    // Input:   dwBcdLen    BCD��ʽ���ַ������ȣ���ǰ�ú���֧��6��7�ֽ�BCD�ַ�������ʽ����
    // Output:  pBuf        ����ת�����BCD�ַ����Ļ���������������ΪdwBcdLen�ֽ�
    // Return:  ��Ӧ��BCD��ʽ���ַ���
    char * Time2BCD(time_t dwTime, int dwBcdLen, char * pBuf);
}

#endif  // __H_TIME_TRANSFORMER_


