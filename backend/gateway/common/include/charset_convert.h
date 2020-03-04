#ifndef _CHAR_CONVER_H
#define _CHAR_CONVER_H

#include <stdint.h>
#include <string>

namespace Goome
{

    // 功能:字符串编码转换
    // 输入参数:   pIn 输入字符串, pOut编码转换后字符串, insize输入字符串长度，pTo为目的编码, pFrom为字符串的原始编码
    // 输出参数:   输出字符串长度
    int CharsetConvert(const char* pIn, char *pOut, int insize, int outsize, const char *pTo, const char *pFrom);

	//功能:将GBK编码字符串转换为utf8编码字符串
	//输入参数:strGbk为gbk编码的字符串，strUtf8为utf8编码的字符串
	//输出参数:转换后的strUtf8字符串的长度
	int gbk2utf8(const std::string& strGbk, std::string& strUtf8);

	//功能:将utf8编码字符串转换为gbk编码字符串
	//输入参数:strGbk为gbk编码的字符串，strUtf8为utf8编码的字符串
	//输出参数:转换后的strGbk字符串的长度
	int utf82gbk(const std::string& strUtf8, std::string& strGbk);
}

#endif	// _CHAR_CONVER_H

