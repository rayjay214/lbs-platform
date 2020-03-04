#ifndef     __STRING_UTILITY_H__
#define     __STRING_UTILITY_H__

// 字符串相关处理

// #pragma warning( disable : 4786 )
// #pragma warning( disable : 4996 )

#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

// 不分大小写的字符串比较函数
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
	// 字符串内容处理

    /** 功能: 删除在strValue中两头的无效字符 */
    std::string TrimBothSides(const std::string& strValue, 
									 const std::string& strTarget = " \t\r\n");

    /** 功能: 删除在strValue中左边的无效字符 */
    std::string TrimLeft(const std::string& strValue, 
								const std::string& strTarget = " \t\r\n");

    /** 功能: 删除在strValue中右边的无效字符 */
    std::string TrimRight(const std::string& strValue, 
								 const std::string& strTarget = " \t\r\n");

    /** 功能: 把一个字符串划分成多个字符串 */
    void SplitString(const std::string& strMain, 
				    const  std::string& strSpliter, 
				    std::vector<std::string>& strList);

    /** 功能:  简单的分词函数：把一个字符串按分隔符号划分成多个字符串 */
    void SplitString(const std::string& strMain, 
				    std::vector<std::string>& strList);


    /** 把字符串A中的子串B替换为C */
    std::string ReplaceStr(const std::string& strValue, 
						   const std::string& oldStr, 
						   const std::string& newStr);

	/** 把一个集合中的字符转换为对应的另一个集合中字符
	 *  例如：把 <变为[, > 变为]
	 *        strNew = ReplaceCharSet(strOld, "<>", "[]");
	 */
	std::string ReplaceCharSet(const std::string& strOld, 
								 std::string strFromSet, 
								 std::string strToSet);

	/** 替换字符串中的某些子字符串 */
	std::string ReplaceStrSet(const std::string& strRawData, 
									 std::map<std::string, std::string>& mSet);

    /** 把字符串A中的子符删除 */
    std::string RemoveChar(std::string& strRawData, char ch);

    /** 把字符串A中的字符集删除 */
    std::string RemoveChars(std::string& strRawData, const std::string & chs = " \t\r\n");

    /** 把字符串A中的某个子字符串删除 */
    std::string RemoveStr(std::string& strRawData, std::string strSubStr, bool bNeedSeparator = false);

	/** 删除字符串中的某些子字符串 */
	std::string RemoveStrSet(const std::string& strRawData, 
									 std::set<std::string>& sStrSet, bool bNeedSeparator = false);

	/** 删除一个字符串中的一段子字符串 */
	std::string DeleteSubstr(std::string& strRawData, std::string strBegin, std::string strEnd);

    /** 从一个字符串中提取出关注的部分，例如"[*]" */
    std::string ExtractSubstr( const std::string& strInput, std::string& strPattern);

	/** 把字符串中一些需要转义的字符做转义处理 */
	std::string QuoteWild(const char* strRaw);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 编码相关的字符串内容处理

    // 计算strSrc中的UTF8字符数目
    size_t Utf8StringLength(const char * strSrc);

    // 截取一段UTF8字符串中，位于[udwStart, udwEnd)之间的字符（非字节）
    bool Utf8SubString(const std::string & strSrc, unsigned int udwStart, unsigned int udwEnd, std::string & strDest);

    // 获取UTF8字符串中至多前N个字节的字符
    bool Utf8StringGetPrefix(const std::string & strSrc, unsigned int udwMaxBytes, std::string & strDest);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 字符串内容判断

    /** 比较字符，不区分大小写 */
	bool CmpNoCase(char c1, char c2);

	/** 在字符串中查找子符串，不区分大小写 */
	std::string::size_type FindNoCase(std::string strMain, std::string strSub, int dwFrom);

	/** 获得一个字符串的字符个数 */
    inline int GetCharCount(std::string& strWord);

    /** 返回当前位置开始的下一个字符 */
    std::string GetNextChar(std::string& str, size_t dwCurPos);

    /** 检测字符串中是否包含了某些子字符串 */
    bool IsSubStrContained(std::string& strRawData, std::set<std::string>& sSubStr);

    /** 是否是数字IP */
    bool IsDigitIp(std::string strIp);

    /** 判断一个字符串是否是空白字符 */
    bool IsWhiteString(char const *pStr);

    /** 判断一个字符串是否全是因为字母,  最后一个参数表示是否要把'_'算作字母 */
    bool IsEnglishString(const char* pStr, bool bLowLineAsAlpha = false);

    /** 判断一个字符串是否全是数字组成 */
    bool IsDigitString(const char* pStr);

    /** 计算GB2312汉字的序号,返回值是0到6767之间的数 */
    int GetGB2312HanziIndex(std::string& str);

    /** 判断一个字符串的前两个字节是否是GB2312汉字 */
    bool IsGB2312Hanzi(std::string& str);

	/** 判断一个字符串是否完全由GB2312的汉字组成 */
    bool IsOnlyComposedOfGB2312Hanzi(std::string& strWord);

	/** 判断字符是否是空格，windows下的isspace()有bug */
    bool IsSpace(char c);

	/** 判断字符是否是数字或字母，windows下的isalnum()有bug */
    bool IsAlnum(char c);

	/** 判断字符是否是数字，windows下的isdigit()有bug */
    bool IsDigit(char c);

	/** 判断字符是否是字母，windows下的isalpha()有bug */
    bool IsAlpha(char c);

	/** 判断字符是否是分界符 */
    bool IsSeparator(char c);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 字符串大小写转换

	/** 把字符转为小写 */
    char CharToLower(char c);

	/** 把字符转为大写 */
    char CharToUpper(char c);

    /** 把字符串转为小写 */
    std::string ToLower(const char *pszValue);

    /** 把字符串转为大写 */
    std::string ToUpper(const char *pszValue);

	///////////////////////////////////////////////////////////////////////
	// 字符串和整数、浮点数、十六进制数之间的转换

#ifdef WIN32
    /** 把一个int值转换成为字符串*/
//    std::string Int2Str(int dwValue);
#endif

    std::string SignedInt2Str(int dwValue);

    /** 把一个unsigned int值转换成为字符串*/
    std::string Int2Str(unsigned int  dwValue);

    /** 把字符串转为整型数 */
    int Str2Int(const std::string& str);

    /** 把16进制的字符串转为整型数 */
    int HexStr2long(const char* pcHex);

    /** 把二进制数据转为16进制的字符串 */
    std::string Binary2HexFormatString(const char* pcBinData, size_t udwDataLen);

    /** 把一个字符转换成16进制的形式 */
    std::string Char2Hex(unsigned char ch);

	char char2Bin(const char cHex);

	/** 把一个数值字符串转换成数字数组 */
	std::string Hex2Bin(const std::string &str);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 调试时用到的字符串打印

    /** 以十六进制的格式打印，调试时候使用 */
    void BinPrint(char * title, unsigned char * p,int len);

    /** 判断一个字符是否在一个字符串中 */
    bool IsCharInStr(unsigned char ch, const std::string& strTarget);

    /** 把一个字符串中的连续空格转为单个空格 */
    std::string SingleBlank(const std::string& strRaw); 

	/** 删除html或xml格式的注释 <!-- --> */
	std::string StripComments(const std::string& strRawFile);

    /** 计算一个字符串的hash值 */
    unsigned int GetStrHashValue(const char* pcStr);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 字符串模糊匹配实现（就是简单的正则表达式以及范围匹配功能）

    /** 检查一个字符串是否能匹配到一个通配符号
    *  MatchWildcard("he*o","hello"): true
    *  MatchWildcard("he*p","hello"): false
    *  MatchWildcard("he??o","hello"): true
    *  MatchWildcard("he?o","hello"): false
    *  MatchWildcard("[a-z][0-9]","h7"): true
    *  MatchWildcard("172.16.*","172.16.68.29"): true
    */
    bool MatchWildcard(const std::string& strWild, const std::string& strMatch);

	/** 把数字串按三位三位用分割号分隔123456789  123,456,789*/
std::string SeperateDigist(const std::string& strValue, const std::string& seperStr);

    // 计算一个字符是否在一个集合中，例如'8'属于"0-9"
    bool _MatchSet(const std::string& strPattern, char strMatch);

    // 匹配剩下的通配符，递归调用
    bool _MatchRemainder(const std::string& strWild, 
							    std::string::const_iterator itWild, 
							    const std::string& strMatch, 
							    std::string::const_iterator itMatch);
};

#endif


