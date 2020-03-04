#ifndef     __STRING_UTILITY_H__
#define     __STRING_UTILITY_H__

// �ַ�����ش���

// #pragma warning( disable : 4786 )
// #pragma warning( disable : 4996 )

#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

// ���ִ�Сд���ַ����ȽϺ���
#ifdef WIN32
    #define STRNCASECMP			strnicmp
    #define STRCASECMP			stricmp
	#define snprintf            _snprintf 
#else 
    #define STRNCASECMP			strncasecmp
    #define STRCASECMP			strcasecmp
#endif

namespace Goome
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ������ݴ���

    /** ����: ɾ����strValue����ͷ����Ч�ַ� */
    std::string TrimBothSides(const std::string& strValue, 
									 const std::string& strTarget = " \t\r\n");

    /** ����: ɾ����strValue����ߵ���Ч�ַ� */
    std::string TrimLeft(const std::string& strValue, 
								const std::string& strTarget = " \t\r\n");

    /** ����: ɾ����strValue���ұߵ���Ч�ַ� */
    std::string TrimRight(const std::string& strValue, 
								 const std::string& strTarget = " \t\r\n");

    /** ����: ��һ���ַ������ֳɶ���ַ��� */
    void SplitString(const std::string& strMain, 
				    const  std::string& strSpliter, 
				    std::vector<std::string>& strList);

    /** ����:  �򵥵ķִʺ�������һ���ַ������ָ����Ż��ֳɶ���ַ��� */
    void SplitString(const std::string& strMain, 
				    std::vector<std::string>& strList);


    /** ���ַ���A�е��Ӵ�B�滻ΪC */
    std::string ReplaceStr(const std::string& strValue, 
						   const std::string& oldStr, 
						   const std::string& newStr);

	/** ��һ�������е��ַ�ת��Ϊ��Ӧ����һ���������ַ�
	 *  ���磺�� <��Ϊ[, > ��Ϊ]
	 *        strNew = ReplaceCharSet(strOld, "<>", "[]");
	 */
	std::string ReplaceCharSet(const std::string& strOld, 
								 std::string strFromSet, 
								 std::string strToSet);

	/** �滻�ַ����е�ĳЩ���ַ��� */
	std::string ReplaceStrSet(const std::string& strRawData, 
									 std::map<std::string, std::string>& mSet);

    /** ���ַ���A�е��ӷ�ɾ�� */
    std::string RemoveChar(std::string& strRawData, char ch);

    /** ���ַ���A�е��ַ���ɾ�� */
    std::string RemoveChars(std::string& strRawData, const std::string & chs = " \t\r\n");

    /** ���ַ���A�е�ĳ�����ַ���ɾ�� */
    std::string RemoveStr(std::string& strRawData, std::string strSubStr, bool bNeedSeparator = false);

	/** ɾ���ַ����е�ĳЩ���ַ��� */
	std::string RemoveStrSet(const std::string& strRawData, 
									 std::set<std::string>& sStrSet, bool bNeedSeparator = false);

	/** ɾ��һ���ַ����е�һ�����ַ��� */
	std::string DeleteSubstr(std::string& strRawData, std::string strBegin, std::string strEnd);

    /** ��һ���ַ�������ȡ����ע�Ĳ��֣�����"[*]" */
    std::string ExtractSubstr( const std::string& strInput, std::string& strPattern);

	/** ���ַ�����һЩ��Ҫת����ַ���ת�崦�� */
	std::string QuoteWild(const char* strRaw);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ������ص��ַ������ݴ���

    // ����strSrc�е�UTF8�ַ���Ŀ
    size_t Utf8StringLength(const char * strSrc);

    // ��ȡһ��UTF8�ַ����У�λ��[udwStart, udwEnd)֮����ַ������ֽڣ�
    bool Utf8SubString(const std::string & strSrc, unsigned int udwStart, unsigned int udwEnd, std::string & strDest);

    // ��ȡUTF8�ַ���������ǰN���ֽڵ��ַ�
    bool Utf8StringGetPrefix(const std::string & strSrc, unsigned int udwMaxBytes, std::string & strDest);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ��������ж�

    /** �Ƚ��ַ��������ִ�Сд */
	bool CmpNoCase(char c1, char c2);

	/** ���ַ����в����ӷ����������ִ�Сд */
	std::string::size_type FindNoCase(std::string strMain, std::string strSub, int dwFrom);

	/** ���һ���ַ������ַ����� */
    inline int GetCharCount(std::string& strWord);

    /** ���ص�ǰλ�ÿ�ʼ����һ���ַ� */
    std::string GetNextChar(std::string& str, size_t dwCurPos);

    /** ����ַ������Ƿ������ĳЩ���ַ��� */
    bool IsSubStrContained(std::string& strRawData, std::set<std::string>& sSubStr);

    /** �Ƿ�������IP */
    bool IsDigitIp(std::string strIp);

    /** �ж�һ���ַ����Ƿ��ǿհ��ַ� */
    bool IsWhiteString(char const *pStr);

    /** �ж�һ���ַ����Ƿ�ȫ����Ϊ��ĸ,  ���һ��������ʾ�Ƿ�Ҫ��'_'������ĸ */
    bool IsEnglishString(const char* pStr, bool bLowLineAsAlpha = false);

    /** �ж�һ���ַ����Ƿ�ȫ��������� */
    bool IsDigitString(const char* pStr);

    /** ����GB2312���ֵ����,����ֵ��0��6767֮����� */
    int GetGB2312HanziIndex(std::string& str);

    /** �ж�һ���ַ�����ǰ�����ֽ��Ƿ���GB2312���� */
    bool IsGB2312Hanzi(std::string& str);

	/** �ж�һ���ַ����Ƿ���ȫ��GB2312�ĺ������ */
    bool IsOnlyComposedOfGB2312Hanzi(std::string& strWord);

	/** �ж��ַ��Ƿ��ǿո�windows�µ�isspace()��bug */
    bool IsSpace(char c);

	/** �ж��ַ��Ƿ������ֻ���ĸ��windows�µ�isalnum()��bug */
    bool IsAlnum(char c);

	/** �ж��ַ��Ƿ������֣�windows�µ�isdigit()��bug */
    bool IsDigit(char c);

	/** �ж��ַ��Ƿ�����ĸ��windows�µ�isalpha()��bug */
    bool IsAlpha(char c);

	/** �ж��ַ��Ƿ��Ƿֽ�� */
    bool IsSeparator(char c);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ�����Сдת��

	/** ���ַ�תΪСд */
    char CharToLower(char c);

	/** ���ַ�תΪ��д */
    char CharToUpper(char c);

    /** ���ַ���תΪСд */
    std::string ToLower(const char *pszValue);

    /** ���ַ���תΪ��д */
    std::string ToUpper(const char *pszValue);

	///////////////////////////////////////////////////////////////////////
	// �ַ�������������������ʮ��������֮���ת��

#ifdef WIN32
    /** ��һ��intֵת����Ϊ�ַ���*/
//    std::string Int2Str(int dwValue);
#endif

    std::string SignedInt2Str(int dwValue);

    /** ��һ��unsigned intֵת����Ϊ�ַ���*/
    std::string Int2Str(unsigned int  dwValue);

    /** ���ַ���תΪ������ */
    int Str2Int(const std::string& str);

    /** ��16���Ƶ��ַ���תΪ������ */
    int HexStr2long(const char* pcHex);

    /** �Ѷ���������תΪ16���Ƶ��ַ��� */
    std::string Binary2HexFormatString(const char* pcBinData, size_t udwDataLen);

    /** ��һ���ַ�ת����16���Ƶ���ʽ */
    std::string Char2Hex(unsigned char ch);

	char char2Bin(const char cHex);

	/** ��һ����ֵ�ַ���ת������������ */
	std::string Hex2Bin(const std::string &str);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����ʱ�õ����ַ�����ӡ

    /** ��ʮ�����Ƶĸ�ʽ��ӡ������ʱ��ʹ�� */
    void BinPrint(char * title, unsigned char * p,int len);

    /** �ж�һ���ַ��Ƿ���һ���ַ����� */
    bool IsCharInStr(unsigned char ch, const std::string& strTarget);

    /** ��һ���ַ����е������ո�תΪ�����ո� */
    std::string SingleBlank(const std::string& strRaw); 

	/** ɾ��html��xml��ʽ��ע�� <!-- --> */
	std::string StripComments(const std::string& strRawFile);

    /** ����һ���ַ�����hashֵ */
    unsigned int GetStrHashValue(const char* pcStr);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ַ���ģ��ƥ��ʵ�֣����Ǽ򵥵�������ʽ�Լ���Χƥ�书�ܣ�

    /** ���һ���ַ����Ƿ���ƥ�䵽һ��ͨ�����
    *  MatchWildcard("he*o","hello"): true
    *  MatchWildcard("he*p","hello"): false
    *  MatchWildcard("he??o","hello"): true
    *  MatchWildcard("he?o","hello"): false
    *  MatchWildcard("[a-z][0-9]","h7"): true
    *  MatchWildcard("172.16.*","172.16.68.29"): true
    */
    bool MatchWildcard(const std::string& strWild, const std::string& strMatch);

	/** �����ִ�����λ��λ�÷ָ�ŷָ�123456789  123,456,789*/
std::string SeperateDigist(const std::string& strValue, const std::string& seperStr);

    // ����һ���ַ��Ƿ���һ�������У�����'8'����"0-9"
    bool _MatchSet(const std::string& strPattern, char strMatch);

    // ƥ��ʣ�µ�ͨ������ݹ����
    bool _MatchRemainder(const std::string& strWild, 
							    std::string::const_iterator itWild, 
							    const std::string& strMatch, 
							    std::string::const_iterator itMatch);
};

#endif


