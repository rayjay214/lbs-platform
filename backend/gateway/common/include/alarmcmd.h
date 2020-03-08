#ifndef _BUS_LINE_ALARM_CMD_H_
#define _BUS_LINE_ALARM_CMD_H_

#include <inttypes.h>
#include <gns_api_ice.h>
#include "gmqi.h"

#define ALARM_CMD_QUEUE_GNS  "GOOCAR_GMQ_ALARM_CMD"
#define ALARM_CMD_QUEUE_OBJECT  "gmqObjectId"
#define GPS_STATUS_CHAR_LENGTH 16

// 平台报警类型id代表的含义：
// 注意：在这里增加或修改类型及其对应的报警ID时，必须保证这个对应关系和爱车安gpsbox数据库中t_alarm_type的保持一致！
enum ALARM_CMD_TYPE
{
     // 1 震动报警
    ALARM_CMD_SHAKE = 1, 

    // 2 断电报警
    ALARM_CMD_NO_POWER = 2, 

    // 3 低电报警
    ALARM_CMD_LOW_POWER = 3, 

    // 4 SOS求救
    ALARM_CMD_SOS = 4, 

    // 5 超速报警
    ALARM_CMD_EXSPEED = 5, 

    // 6 离线报警
    ALARM_CMD_NOT_ONLINE = 6, 

    // 7 栅栏报警
    ALARM_CMD_BARRIER = 7, 

    // 8 出围栏报警
    ALARM_CMD_OUT_RAIL = 8, 

    //9 位移报警 
    ALARM_CMD_MOVED = 9, 

    // 不按线路行驶
    ALARM_CMD_OUTLINE = 10,  

    // 停留时间超长
    ALARM_CMD_LONG_STAY = 11,  

    // 甩站（过站不停）
    ALARM_CMD_NOT_STOPED = 12,  

    // 提前到站
    ALARM_CMD_ADVANCE_STOPED = 13,  

    // 超速
    ALARM_CMD_TOO_FAST = 14,  

    // 半路调头
    ALARM_CMD_MIDWAY_TURN = 15,  

    ALARM_CMD_MIDWAY_OFFLINE = 16,  
    ALARM_CMD_STAY_TOO_LONG = 17,  

    // 20 天线短路报警
    ALARM_CMD_SHORT_CIRCUIT = 20, 

    //21 天线开路报警
    ALARM_CMD_BREAK_CIRCUIT = 21, 

    // 进围栏报警
    ALARM_CMD_IN_RAIL = 24, 

    //启动(移动)报警  
    ALARM_CMD_STARTING = 29, 

    //不按线路行驶超速
    ALARM_CMD_TOO_FAST_NOT_ON_LINE = 30,
	
	//出区域围栏报警
	ALARM_CMD_OUT_AREA_FENCE = 31, 

	//拆机报警
	ALARM_CMD_REMOVE = 32,

   //进二押点告警
   ALARM_CMD_ENTER_RISK_AREA=40,

   //出二押点告警
   ALARM_CMD_LEAVE_RISK_AREA=41,

  //二押点久留告警 
  ALARM_CMD_RISK_AREA_LONG_STAY=42,
};


class CSendAlarmCmd
{
private:
    int m_dwLocalGmqPort;           // 本地GMQ端口
    std::string m_GmqObj;           // gmq的ICE Object名称

    Ice::CommunicatorPtr m_pIC;     // ice通信句柄
    GMQ::MsgQueuePrx m_pGmqPrx;     // 本地GMQ连接句柄
    std::string err_msg;            // 保存出错信息
    
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

    // 设置报警用的参数
    // 输入参数：dwLocalGmqPort     本地GMQ进程使用的端口（爱车安一般为22333，酷米客一般为22300）
    // 输入参数：GmqObj             GMQ的ICE Object名称
    bool SetAlarmICEParm(int dwLocalGmqPort,const std::string &GmqObj)
    {
        m_dwLocalGmqPort = dwLocalGmqPort;
        m_GmqObj = GmqObj;     
	    return true;
    }

    // 获取出错信息
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

