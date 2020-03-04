
#ifndef GOOME_ZK_BASE_API_H_
#define GOOME_ZK_BASE_API_H_

#include "zookeeper.h"
#include "zookeeper.jute.h"
#include <string>
#include <stdio.h>


// �ο��� Zookeeper������������ tech.uc.cn/?p=1189
// www.cnblogs.com/haippy/archive/2013/02/21/2919365.html


#define NODE_INDEX_SEPARATOR    ":"     // ��ʱ�ڵ���ǰ׺����ŵķָ���

// �Զ���������ͣ���ZOOKEEPER�ٷ�API��ZOO_ERRORS�Ĳ���
enum ZkApiError
{
    // 0��-199Ϊzookeeper����Ԥ��
    ZKAPI_ERR_BEGIN = -200,
    ZKAPI_ERR_UNINITED = -201,              // APIδ��ʼ��
    ZKAPI_ERR_CONVERSATION_UNSETUP = -202,  // �Ự��δ����
    ZKAPI_ERR_GNS_FAIL = -203,              // GNS API����ʧ��
    ZKAPI_ERR_OP_OVERTIME = -204,           // ������ʱ
    ZKAPI_ERR_AUTH_PARAM = -205,            //����ļ�Ȩ��Ϣ
    ZKAPI_ERR_NO_HOSTS = -206,              //δ��ʼ��Hosts��Ϣ
    ZKAPI_ERR_END = -300
};

// ��װZookeeper�ٷ��ṩ��API���ṩelector��observer����ʹ���ߵĹ���������
class ZkBaseApi
{
public:
    ZkBaseApi();

    ~ZkBaseApi();

    // ����Zookeeper��ip��port�б��ڵ�һ��InitZookeeperǰ��������øú���
    virtual void SetHosts(const char * pHosts);

    // ��ʼ��ZK���Ӿ��������Ŀ������
    virtual int InitZookeeper();

    virtual int CloseHandle();

    // ��Zookeeper session���ں���������ʱҪ���Ĵ���
    virtual int ReconnectDeal() = 0;

    // ����ZK��ʱʱ��
    virtual void SetTimeout(int t);

    // ����ZK��־����
    virtual void SetLogLevel(int level);

    // ����ZK��־��
    virtual void SetLogStream(FILE * fp);

    // ��ȡ������
    virtual int GetErrorCode();

    // ���ݴ������ͻ�ȡ��������
    static const char * GetErrorDesc(int e);
public:
    zhandle_t* m_pHandle;   // ZK���Ӿ��

    int m_dwErrCode;        // ������

protected:
    virtual int Uninit();
protected:
    std::string m_strHosts; // Zookeeper��ip��port�б�
    int m_dwTimeout;    // ��ZK���ӵĳ�ʱʱ�䣬��λΪms��ȡֵ��4s-40s֮�䡣
    int m_dwLogLevel;   // ZK��־����
    FILE * m_pLogFile;  // ZK��־��������ΪNULL���ʾʹ�ñ�׼����
};


namespace Goome
{
    // Function: ���ݽڵ�ȫ·������ȡ�ڵ���
    // Input: buf     �ڵ�ȫ·��
    // Output: node   �ڵ���
    void GetZkNodeName(const char *buf, char *node);

    // Function: ����ָ���Ľڵ�����һ���ڵ������ϣ��жϸýڵ��ǲ��Ǽ�����ӵ����ͬǰ׺�Ľڵ��������С�ġ���ӵ����С�����һ�����Ϊ��Master��
    // Input: strMyNodeName     ���жϵĽڵ���
    // Input: oNodeList         ���ڱȽϵĽڵ�������
    // Return:  ����ýڵ������ڼ���������С�ģ��򷵻�true�����򷵻�false
    bool IsMinimalZkNode(const std::string & strMyNodeName, const struct String_vector & oNodeList);
}


#endif /*GOOME_ZK_BASE_API_H_*/
