#ifndef __T808_PARSER_H__
#define __T808_PARSER_H__

#include "common.h"

typedef struct _RegisterInfo
{
    uint16_t   province;
    uint16_t   city;
    uint8_t    vendor[5];
    uint8_t    termType[20];
    uint8_t    termId[7];
    uint8_t    color;
    char       license[32];
}__attribute__((packed))RegisterInfo;


typedef struct _HEADER_T808
{
    uint16_t    msg_id;
    uint16_t    attribute;
    uint8_t     phone_no[6];
    uint16_t    seq;
}__attribute__((packed))Header_T808;

//BMS电源信息 8B
typedef struct
{
	uint8_t SN[4];    //SN[0]-SN[4]
    uint8_t Date[3]; //yy mm dd
    uint8_t code_reserve;
}__attribute__((packed))BMSCode;

//设备信息 8B
typedef struct
{
	uint8_t  high_pack_cnt;
    uint8_t  low_pack_cnt;
    uint8_t  dev_reserve;
    uint8_t  factory;
    uint8_t  softwarew_ver;
    uint8_t  protocol_ver;
    uint8_t  hardware_ver;
    uint8_t  real_pack_cnt;
}__attribute__((packed))BMSDev;

//电压统计信息 8B
typedef struct
{
    uint16_t max_vol;
    uint16_t min_vol;
    uint16_t total_max_vol;
    uint16_t total_vol;
}__attribute__((packed))BMSVolStat;

//温度统计信息 8B
typedef struct
{
    uint8_t single_temp1;
    uint8_t single_temp2;
    uint8_t single_temp3;
    uint8_t single_temp4;
    uint8_t max_temp;
    uint8_t min_temp;
    uint8_t averge_temp;
    uint8_t div_temp;
}__attribute__((packed))BMSTempStat;

//电流极值位置信息 8B
typedef struct
{
    uint16_t current;
    uint16_t max_current;
    uint8_t  max_temp_pos;
    uint8_t  min_temp_pos;
    uint8_t  max_vol_pos;
    uint8_t  min_vol_pos;
}__attribute__((packed))BMSCurrentExtPos;

//均衡,硬件及电流状态 8B
typedef struct
{
	uint8_t	balence_op_1;
	uint8_t	balence_op_2;
	uint8_t	balence_op_3;
	uint8_t	balence_op_4;
	uint8_t	input_status;
	uint8_t	mos_status;
	uint8_t	current_status;
	uint8_t	switch_status;
}__attribute__((packed))BMSBalence;

//故障信息1 8B
typedef struct
{
    uint32_t vol_check_exce;
    uint8_t  temp_check_exce;
    uint8_t  connection_err;
    uint8_t  fault1_reserve;
    uint8_t  fault1_life;
}__attribute__((packed))BMSFault1;

//故障信息2 8B
typedef struct
{
	uint32_t fault2_reserve1;
	uint16_t warn_protect_status;
	uint8_t  fault2_reserve2;
	uint8_t  fault2_life;
}__attribute__((packed))BMSFault2;


//容量信息 8B
typedef struct
{
    uint16_t remaining_capacity;
    uint16_t rated_capacity;
    uint16_t capacity_reserve;
    uint8_t SOC;
    uint8_t SOH;
}__attribute__((packed))BMSCapacity;

//累计充放电容量 充放电次数 16B
typedef struct
{
	uint32_t discharge_capacity;
	uint32_t charge_capacity;
	uint32_t discharge_count;
	uint32_t charge_count;
}__attribute__((packed))BMSCumulative;

//电压阈值数据 8B
typedef struct
{
	uint16_t total_over_vol_threshold;
	uint16_t total_under_vol_threshold;
	uint16_t singlel_over_vol_threshold;
	uint16_t single_under_vol_threshold;
}__attribute__((packed))BMSVolThreshold;

//电压恢复值数据 8B
typedef struct
{
	uint16_t total_over_vol_recovery;
	uint16_t total_under_vol_recovery;
	uint16_t single_over_vol_recovery;
	uint16_t single_under_vol_recovery;
}__attribute__((packed))BMSVolRecovery;

//温度阈值 恢复值 4B 4B
typedef struct
{
	uint8_t temp_charge_high_threshold;
	uint8_t temp_charge_low_threshold;
	uint8_t temp_discharge_high_threshold;
	uint8_t temp_discharge_low_threshold;
}__attribute__((packed))BMSTempThreshold;

typedef struct
{
	uint8_t temp_charge_high_recovery;
	uint8_t temp_charge_low_recovery;
	uint8_t temp_discharge_high_recovery;
	uint8_t temp_discharge_low_recovery;
}__attribute__((packed))BMSTempRecovery;

//电流过流阈值 恢复值 4B 4B
typedef struct
{
	uint16_t curr_charge_over_threshold;
	uint16_t curr_discharge_over_threshold;
}__attribute__((packed))BMSCurrentThreshold;

typedef struct
{
	uint16_t curr_charge_over_recovery;
	uint16_t curr_discharge_over_recovery;
}__attribute__((packed))BMSCurentRecovery;

//BMS电压均衡开启电压、开启压差和关闭压差数据 8B
typedef struct
{
	uint16_t balence_open_vol;
	uint16_t balence_open_dp;
	uint16_t balence_close_dp;
	uint16_t balence_reserve;
}__attribute__((packed))BMSBalenceVol;

//BMS保护延时值数据 8B
typedef struct
{
	uint8_t over_vol_delay;
	uint8_t under_vol_delay;
	uint8_t over_current_delay;
	uint8_t single_high_temp_delay;
	uint8_t single_low_temp_delay;
	uint8_t mos_high_temp_delay;
	uint8_t mos_high_temp_threshold;
	uint8_t mos_high_temp_recovery;
}__attribute__((packed))BMSProtectDelay;

//单体电压 8B
typedef struct
{
   uint16_t bat1_vol;
   uint16_t bat2_vol;
   uint16_t bat3_vol;
   uint16_t bat4_vol;
}__attribute__((packed))BMSSingleVol;


typedef struct
{
	BMSCode code; //8B
	BMSDev  dev;
	BMSVolStat vol_stat;
	BMSTempStat temp_stat;
	BMSCurrentExtPos extpos;
	BMSBalence balence;
	BMSFault1 fault1;
	BMSFault2 fault2;
	BMSCapacity capacity;
	BMSCumulative cumulative;
	BMSVolThreshold vol_threshold;
	BMSVolRecovery vol_recovery;
	BMSTempThreshold temp_threshold;
	BMSTempRecovery temp_recovery;
    BMSCurrentThreshold current_threshold;
	BMSCurentRecovery current_recovery;
	BMSBalenceVol balence_vol;
	BMSProtectDelay delay;
	uint8_t single_vol[0];
}__attribute__((packed))BMSInfo;



#define T808_HEAD_LEN 12 //head
#define BODY_INDEX 13

#define T808_REGISTER_MSGID 0x0100
#define T808_REGISTER_ACK_MSGID 0x8100
#define T808_HEARTBEAT_MSGID 0x0002
#define T808_UNREGISTER_MSGID 0x0003
#define T808_AUTH_MSGID 0x0102
#define T808_ADJUSTTIME_MSGID 0x0109
#define T808_GPS_MSGID 0x0200
#define T808_BATCHGPS_MSGID 0x0704
#define T808_BMS_MSGID 0x0F08
#define T808_ICCID_MSGID 0x0120
#define T808_CMDRSP_MSGID 0x6006

#define BMS_ABNORMAL_VOL_ACQUISITION 101
#define BMS_ABNORMAL_TEMP_ACQUISITION 102
#define BMS_HIGH_AFE_COMMUNICATION_FAULT 103
#define BMS_LOW_AFE_COMMUNICATION_FAULT 104
#define BMS_FAULT2_ALARM_BASE 105

#define BMS_RELAY_0 "RELAY,0#"   //BMS供电
#define BMS_RELAY_1 "RELAY,1#"   //BMS断电


enum
{
	E_ABNORMAL_VOL_ACQUISITION = 0,
	E_ABNORMAL_TEMP_ACQUISITION,
	E_COMMUNICATION_FAULT,
	E_FAULT2_BIT_BASE = 8
}BMS_STATUS_BIT;

#define T808_WATERLEVEL_CMD 0x41
#define T808_LBS_CMD 0xa9

class T808Parser
{
public:
    T808Parser()
    {
		m_encrpy = 0;
		m_package = 0;
		m_bodylen = 0;
		m_packageIdx = 0;
		m_packageNum = 0;
		m_packagelen = 0;
    }
    virtual ~T808Parser()
    {
    }

    int MsgParser(const std::string &str, std::string &strRsp, bool bImeiPrefix);
	int HeaderParser(const std::string &str, bool bCheckPkg=false);
	int RegisterProtocol(const std::string &str, RegisterInfo &regist);
    int GpsProtocol(const std::string &str, SSMap &mpParam);
    int AuthProtocol(const std::string &str);
    int BatchGpsProtocol(const std::string &str);
	int BmsProtocol(const std::string &str);
	int IccidProtocol(const std::string &str, std::string &strPbMsg);
	int CmdRspProtocol(const std::string &str, std::string &strPbMsg);
    int LbsProtocol(uint32_t dateTime, const std::string &str);
    
    std::string GeneralMsgAck(uint8_t result);
    std::string RegisterMsgAck(uint8_t result, std::string &auCode);
    std::string HeadOnlyMsgAck();
    std::string AdjustTimeMsgAck();
	std::string TransPhonenoToImei(const char *phone_no, unsigned length);

	uint16_t GetMsgId()
	{
		return m_tHeader.msg_id;
	}

	uint16_t GetMsgLen()
	{
		return m_packagelen;
	}

	std::string GetImei()
	{
		return m_strImei;
	}

	uint64_t GetDevId()
	{
		return m_devId;
	}

	void SetDevId(uint64_t devId)
	{
		m_devId = devId;
	}

	static std::string CommandReqMsg(const std::string &imei, const std::string &content);
    static int Transform0x7e(uint8_t* buf, uint32_t* packageSize, uint32_t* otherSize);
    static bool Transfer0x7e(uint8_t* src_buf , uint32_t src_len, uint8_t* dst_buf, uint32_t* p_dst_len);

private:
	void sendBmsAlarm(int alarm_type, const std::string &status);

private:
	uint64_t m_devId;
    std::string m_strImei;

	Header_T808 m_tHeader;
    uint8_t  m_encrpy;
    uint16_t m_bodylen;
	uint8_t  m_package;

	uint16_t m_packageNum;
	uint16_t m_packageIdx;
	uint16_t m_packagelen;
};

#endif


