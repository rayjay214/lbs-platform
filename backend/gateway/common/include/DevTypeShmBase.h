#ifndef _DEV_TYPE_SHM_BASE_H_
#define _DEV_TYPE_SHM_BASE_H_
#include <sys/shm.h>
#include <cstddef>
#include <time.h>

//�û���Ϣ�ṹ��Ŀ
namespace DevTypeShm
{

class CDevType{
	
	public:
		char m_DevType[12];
};

class DevTypeShmBase
{
	private:
		//�����ڴ����ʼָ����ܳ���
		void *m_pShareCache;
		size_t *m_ipTotallSize;

		size_t *m_ipMaxEntryNum;	//�豸��������¼����
		size_t *m_ipEntryNum;		//�豸���͵�ǰ��¼����
		size_t *m_ipVecSize;		//�����ڴ��������豸��������洢�ռ�Ĵ�С
		int *m_ipTimeStamp;			//������µ�ʱ���
		CDevType *m_szDevTypeCache;	//�豸�����ڴ���ָ��

        char m_szErrorMsg[1024];    // ������Ϣ
	public:
		//���캯������������
	    DevTypeShmBase();
	    virtual ~DevTypeShmBase();
		
		const char * GetError()
        {
            return m_szErrorMsg;
        }
	protected:
		/////////////////////////////////////////////////////////////////////////
	    // Function:   _Init
	    // Description: ��ʼ�����乲���ڴ�ռ�
	    // Input:      ������򿪵Ĺ����ڴ��keyֵ�����ɲ����豸������Ŀ��
	    // Return:    �ɹ���ʼ������0�����򷵻�-1
	    //////////////////////////////////////////////////////////////////////////
		int _Init(int lkey, int max_entry = 0);

		// �������ܣ���չ����ڴ�ռ�
		/////////////////////////////////////////////////////////////////////////
	    // Function:	_InsertDevType
	    // Description:�����ݿ��е����豸���ͣ�д�빲���ڴ���
	    // Input:		dwMaxSize       �豸ID������
	    // Return:    �ɹ����뷵��0�����򷵻�-1
	    //////////////////////////////////////////////////////////////////////////
	    int _InsertDevType(int iDevID, const std::string & szDevType);

		/////////////////////////////////////////////////////////////////////////
	    // Function:	_GetDevType
	    // Description: �����豸ID����ȡ���豸����		
	    // Input:		dwDevID		�豸ID
	    // Output:	strDevType	�豸����
	    // Return:	���ɹ���ȡ������0�����򷵻�-1
	    //////////////////////////////////////////////////////////////////////////
	    int _GetDevType(size_t dwDevID, std::string& strDevType);

		//��������:��չ����ڴ��е��豸���ͼ�¼������ռ�
		void _Clear();

		// �������ܣ���ȡ����ǰ�����豸���ͼ�¼��Ŀ
		// ����ֵ�� ������-1ʱ����ʾ��ȡʧ�ܣ�����>=0ʱ����ʾ�����ڴ��е�ǰ�����豸���ͼ�¼��Ŀ
		int _GetCount();

		// �������ܣ���ӡ�����ڴ��������Ч��¼
		void _Printall();

		// �������ܣ����ù����ڴ��е�ʱ���ֶ�Ϊָ����ʱ��ֵ
		void _SetTIme(int time);

		// �������ܣ���ȡ�����ڴ��е�ʱ���ֶ�Ϊָ����ʱ��ֵ
		// ����ֵ��������-1ʱ����ʾ��ȡʧ�ܣ�����>=0ʱ����ʾ�����ڴ��е�ǰʱ���ֶ�
		int _GetTIme();
};

}    // namespace

#endif // _DEV_TYPE_SHM_BASE_H_
