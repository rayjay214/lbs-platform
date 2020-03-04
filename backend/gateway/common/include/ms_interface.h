
#ifndef GOOME_MS_INTERFACE_H_
#define GOOME_MS_INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <map>
#include <set>
#include "ms_observer.h"

namespace Goome
{
    // ���������ӵ��ͬ����GNSʱ��ʹ�øýṹ�ġ���Ƭ��+�ҶȺš����Թ��ࡣ
    struct GnsSvrAttribute
    {
        int dwShardID;
        int dwGreyID;
        GnsSvrAttribute():dwShardID(0), dwGreyID()
        {
        }
        bool operator<(const GnsSvrAttribute& other) const
        {
            return (dwShardID == other.dwShardID) ? (dwGreyID < other.dwGreyID) : (dwShardID < other.dwShardID);
        }
        bool operator==(const GnsSvrAttribute& other) const
        {
            return ((dwShardID == other.dwShardID) && (dwGreyID == other.dwGreyID));
        }
    };

    // ����ʵ��������Ϣ
    struct GnsSvrLocation
    {
        std::string strIp;
        int dwPort;
        GnsSvrLocation():dwPort(0)
        {
        }
        bool operator<(const GnsSvrLocation& other) const
        {
            return (strIp == other.strIp) ? (dwPort < other.dwPort) : (strIp < other.strIp);
        }
        bool operator==(const GnsSvrLocation& other) const
        {
            return ((strIp == other.strIp) && (dwPort == other.dwPort));
        }
    };

    typedef std::map<GnsSvrAttribute, std::set<GnsSvrLocation> > GnsAttr2Location;

    struct GnsSvrMsInfo 
    {
        MsObserverApi * pMsApi;
        GnsAttr2Location oAttr2Location;   // ����GNS���õķ�Ƭ�ͻҶ��ҵ���master�����ip��port
    };
}

// ���ӹ�����
class MsMgr
{
public:
    MsMgr();
    ~MsMgr();

    // Function: ��ȡĳһ�����Master�ڵ���Ϣ
    // Input:   strZkPath   Ŀ�������Zookeeper��ע��������Ϣʱʹ�õ�·��
    // Input:   strZkHost   Ŀ�����ע��������Ϣʱʹ�õ�Zookeeper��������host�б���ʽ��"127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"��
    // Output:  oAttr2Location     ����������Master�Ĳ���λ��
    // Return: �����Ƿ�ɹ���
    bool GetMaster(const std::string & strZkPath, const std::string & strZkHost, Goome::GnsAttr2Location & oAttr2Location);

    // Function: ��ȡĳһ����ָ����Ƭ���Ҷȵ�Master��Ϣ��IP+�˿ڣ�
    // Input:   strZkPath   Ŀ�������Zookeeper��ע��������Ϣʱʹ�õ�·��
    // Input:   dwShard     ָ��Ŀ�����ķ�Ƭ
    // Input:   dwGrey      ָ��Ŀ�����ĻҶȰ汾
    // Input:   strZkHost   Ŀ�����ע��������Ϣʱʹ�õ�Zookeeper��������host�б���ʽ��"127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002"��
    // Output:  oSvrLoc     ����������Master�Ĳ���λ��
    bool GetMaster(const std::string & strZkPath, int dwShard, int dwGrey, const std::string & strZkHost, std::set<Goome::GnsSvrLocation> & oSvrLoc);

    // Function: ����ĳһZookeeper Node·�������ӷ������Ϣ����Ҫ��Zookeeper�ص�����ʹ��
    // Input:   strPath             Ŀ��Zookeeper Node·��
    // Input:   oAttr2Location      ���ӷ������Ϣ
    void SetLocation(const std::string & strPath, const Goome::GnsAttr2Location & oAttr2Location);

    // Function: ����������Ϣ�仯ʱ�Ļص�����
    void SetCallback(MsApiCallback f);

    // Function: ��ô�����Ϣ
    // Return:   ������Ϣ�ַ���
    std::string GetError()
    {
        return m_strErr;
    }
protected:
    // Function: ��Zookeeper�ϻ�ȡ������ָ��·������Ϣ
    // Input:   strZkHost   Ŀ��Zookeeper��������Ϣ
    // Input:   strZkPath   Ŀ��·��
    // Return: �����Ƿ�ɹ���
    bool Monitor(const std::string & strZkHost, const std::string & strZkPath);

    friend void MsMgrWatcher(void * pUserParam, const std::string & strZkPath, const std::vector<ZNodeInfo> & oNodes);

protected:
    MsApiCallback m_fCallback;      // �û��Զ���ص�����

    std::string m_strErr;

    std::map<std::string, Goome::GnsSvrMsInfo> m_oSvr2ZkApi;    // ��Zookeeper·�� -> ��������Ϣ���ı��ػ���
    // ע�����ڴ˴�Key��Zookeeper��Znode·��������պ�ϵͳ�в����˶��׻�������Zookeeper����ʱĳCaller�޷�ȥ����ZK��ȡͬһ·�����������ݡ�

    pthread_mutex_t m_oLock;    // TODO: ��������ɶ�д�����������
private:
};

#endif // GOOME_MS_INTERFACE_H_

