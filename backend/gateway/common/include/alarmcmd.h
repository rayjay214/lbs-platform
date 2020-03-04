#ifndef _BUS_LINE_ALARM_CMD_H_
#define _BUS_LINE_ALARM_CMD_H_

#include <inttypes.h>
#include <gns_api_ice.h>
#include "gmqi.h"

#define ALARM_CMD_QUEUE_GNS  "GOOCAR_GMQ_ALARM_CMD"
#define ALARM_CMD_QUEUE_OBJECT  "gmqObjectId"
#define GPS_STATUS_CHAR_LENGTH 16

// ƽ̨��������id����ĺ��壺
// ע�⣺���������ӻ��޸����ͼ����Ӧ�ı���IDʱ�����뱣֤�����Ӧ��ϵ�Ͱ�����gpsbox���ݿ���t_alarm_type�ı���һ�£�
enum ALARM_CMD_TYPE
{
     // 1 �𶯱���
    ALARM_CMD_SHAKE = 1, 

    // 2 �ϵ籨��
    ALARM_CMD_NO_POWER = 2, 

    // 3 �͵籨��
    ALARM_CMD_LOW_POWER = 3, 

    // 4 SOS���
    ALARM_CMD_SOS = 4, 

    // 5 ���ٱ���
    ALARM_CMD_EXSPEED = 5, 

    // 6 ���߱���
    ALARM_CMD_NOT_ONLINE = 6, 

    // 7 դ������
    ALARM_CMD_BARRIER = 7, 

    // 8 ��Χ������
    ALARM_CMD_OUT_RAIL = 8, 

    //9 λ�Ʊ��� 
    ALARM_CMD_MOVED = 9, 

    // ������·��ʻ
    ALARM_CMD_OUTLINE = 10,  

    // ͣ��ʱ�䳬��
    ALARM_CMD_LONG_STAY = 11,  

    // ˦վ����վ��ͣ��
    ALARM_CMD_NOT_STOPED = 12,  

    // ��ǰ��վ
    ALARM_CMD_ADVANCE_STOPED = 13,  

    // ����
    ALARM_CMD_TOO_FAST = 14,  

    // ��·��ͷ
    ALARM_CMD_MIDWAY_TURN = 15,  

    ALARM_CMD_MIDWAY_OFFLINE = 16,  
    ALARM_CMD_STAY_TOO_LONG = 17,  

    // 20 ���߶�·����
    ALARM_CMD_SHORT_CIRCUIT = 20, 

    //21 ���߿�·����
    ALARM_CMD_BREAK_CIRCUIT = 21, 

    // ��Χ������
    ALARM_CMD_IN_RAIL = 24, 

    //����(�ƶ�)����  
    ALARM_CMD_STARTING = 29, 

    //������·��ʻ����
    ALARM_CMD_TOO_FAST_NOT_ON_LINE = 30,
	
	//������Χ������
	ALARM_CMD_OUT_AREA_FENCE = 31, 

	//�������
	ALARM_CMD_REMOVE = 32,

   //����Ѻ��澯
   ALARM_CMD_ENTER_RISK_AREA=40,

   //����Ѻ��澯
   ALARM_CMD_LEAVE_RISK_AREA=41,

  //��Ѻ������澯 
  ALARM_CMD_RISK_AREA_LONG_STAY=42,
};


class CSendAlarmCmd
{
private:
    int m_dwLocalGmqPort;           // ����GMQ�˿�
    std::string m_GmqObj;           // gmq��ICE Object����

    Ice::CommunicatorPtr m_pIC;     // iceͨ�ž��
    GMQ::MsgQueuePrx m_pGmqPrx;     // ����GMQ���Ӿ��
    std::string err_msg;            // ���������Ϣ
    
public:
    CSendAlarmCmd()
    {
        m_pGmqPrx = NULL;
        m_pIC = NULL;

        int argc = 2;
        char *argv[2] = {(char *)"--Ice.ThreadPool.Client.Size=2", (char *)"--Ice.ThreadPool.Client.SizeMax=8"};

        try
        {
            m_pIC = Ice::initialize(argc, argv);
        }
        catch(const Ice::Exception& ex)
        {
            err_msg = ex.what();
            m_pIC = NULL;
        }
    }

    ~CSendAlarmCmd()
    {
		try
		{
			if(m_pIC)
				m_pIC->destroy();
		}	
		catch(const Ice::Exception& ex)
		{
		}

        m_pIC = NULL;
    }

    // ���ñ����õĲ���
    // ���������dwLocalGmqPort     ����GMQ����ʹ�õĶ˿ڣ�������һ��Ϊ22333�����׿�һ��Ϊ22300��
    // ���������GmqObj             GMQ��ICE Object����
    bool SetAlarmICEParm(int dwLocalGmqPort,const std::string &GmqObj)
    {
        m_dwLocalGmqPort = dwLocalGmqPort;
        m_GmqObj = GmqObj;     
	    return true;
    }

    // ��ȡ������Ϣ
    std::string GetErrMsg()
    {
        return err_msg;
    }

    int SendOutAlarmCmd(const std::string &strCallerID, const std::string &strCalleeID, uint64_t uid, int alarmid, time_t gpstime, double lng, 
        double lat, unsigned int speed, char *status, unsigned int direction);

    int SendOutAlarmCmd(const std::string &strCallerID, const std::vector<std::string> & vecCalleeIDs, uint64_t uid, int alarmid, time_t gpstime, double lng, 
        double lat, unsigned int speed, char *status, unsigned int direction);


    int SendOutBusAlarmCmd(const std::string &strCallerID, const std::string &strCalleeID, uint64_t uid, int alarmid, time_t gpstime, double lng, 
        double lat, unsigned int speed, char *status, unsigned int direction, const std::string &strinfo);

    int SendOutBusAlarmCmd(const std::string &strCallerID, const std::vector<std::string> & vecCalleeIDs, uint64_t uid, int alarmid, time_t gpstime, double lng, 
        double lat, unsigned int speed, char *status, unsigned int direction, const std::string &strinfo);

	int SendOutOpenAlarmCmd(const std::string &strCallerID, const std::string & strCalleeID, const std::string& msg);
	int SendOutOpenAlarmCmd(const std::string &strCallerID, const std::vector<std::string> & vecCalleeIDs, const std::string& msg);

	int SendOutOpenAlarmCmd(const std::string &strCallerID, const std::string &strCalleeID, uint64_t uid, int alarmid, time_t gpstime, double lng, 
		double lat, unsigned int speed, char *status, unsigned int direction, std::string des);

	int SendOutOpenAlarmCmd(const std::string &strCallerID, const std::vector<std::string> & vecCalleeIDs, uint64_t uid, int alarmid, time_t gpstime, double lng, 
		double lat, unsigned int speed, char *status, unsigned int direction, std::string des);
private:
    bool GetLocalGmqProxy();
};


#endif

