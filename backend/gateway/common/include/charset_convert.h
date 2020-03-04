#ifndef _CHAR_CONVER_H
#define _CHAR_CONVER_H

#include <stdint.h>
#include <string>

namespace Goome
{

    // ����:�ַ�������ת��
    // �������:   pIn �����ַ���, pOut����ת�����ַ���, insize�����ַ������ȣ�pToΪĿ�ı���, pFromΪ�ַ�����ԭʼ����
    // �������:   ����ַ�������
    int CharsetConvert(const char* pIn, char *pOut, int insize, int outsize, const char *pTo, const char *pFrom);

	//����:��GBK�����ַ���ת��Ϊutf8�����ַ���
	//�������:strGbkΪgbk������ַ�����strUtf8Ϊutf8������ַ���
	//�������:ת�����strUtf8�ַ����ĳ���
	int gbk2utf8(const std::string& strGbk, std::string& strUtf8);

	//����:��utf8�����ַ���ת��Ϊgbk�����ַ���
	//�������:strGbkΪgbk������ַ�����strUtf8Ϊutf8������ַ���
	//�������:ת�����strGbk�ַ����ĳ���
	int utf82gbk(const std::string& strUtf8, std::string& strGbk);
}

#endif	// _CHAR_CONVER_H

