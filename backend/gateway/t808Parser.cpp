#include <bitset>
#include <math.h>
#include "tostring.h"
#include "TimeTransformer.h"
#include "StringUtility.h"
#include "dev.pb.h"
#include "common.h"
#include "DispatchThread.h"
#include "GAsyncCurlThread.h"
#include "t808Parser.h"

#define Name(X) #X

extern ReceiveDataList g_oReceiveList;

static uint8_t calc_xor(uint8_t* buf, uint32_t len)
{
    uint8_t c_xor = 0;
    uint8_t* current = buf;
    for(uint32_t i = 0 ; i < len; i++, current++)
    {
        c_xor ^= *current;
    }
    return c_xor;
}

void OutBMSInfo(const BMSInfo &oInfo, std::map<std::string, std::string> &mpParam)
{
#define GET_BIN_FILEDS(X,F,P) {key=Name(F);val=Goome::Binary2HexFormatString((const char*)X.F,sizeof(X.F));P[key]=val;}
#define GET_FILEDS(X,F,P) {key=Name(F);val=ToString((uint32_t)X.F);P[key]=val;}
    std::string key, val;

    GET_BIN_FILEDS(oInfo.code,SN,mpParam)
    GET_BIN_FILEDS(oInfo.code,Date,mpParam)
    GET_FILEDS(oInfo.code,code_reserve,mpParam)

    GET_FILEDS(oInfo.dev,high_pack_cnt,mpParam)
    GET_FILEDS(oInfo.dev,low_pack_cnt,mpParam)
    GET_FILEDS(oInfo.dev,dev_reserve,mpParam)
    GET_FILEDS(oInfo.dev,factory,mpParam)
    GET_FILEDS(oInfo.dev,softwarew_ver,mpParam)
    GET_FILEDS(oInfo.dev,protocol_ver,mpParam)
    GET_FILEDS(oInfo.dev,hardware_ver,mpParam)
    GET_FILEDS(oInfo.dev,real_pack_cnt,mpParam)

    GET_FILEDS(oInfo.vol_stat,max_vol,mpParam)
    GET_FILEDS(oInfo.vol_stat,min_vol,mpParam)
    GET_FILEDS(oInfo.vol_stat,total_max_vol,mpParam)
    GET_FILEDS(oInfo.vol_stat,total_vol,mpParam)

    GET_FILEDS(oInfo.temp_stat,single_temp1,mpParam)
    GET_FILEDS(oInfo.temp_stat,single_temp2,mpParam)
    GET_FILEDS(oInfo.temp_stat,single_temp3,mpParam)
    GET_FILEDS(oInfo.temp_stat,single_temp4,mpParam)
    GET_FILEDS(oInfo.temp_stat,max_temp,mpParam)
    GET_FILEDS(oInfo.temp_stat,min_temp,mpParam)
    GET_FILEDS(oInfo.temp_stat,averge_temp,mpParam)
    GET_FILEDS(oInfo.temp_stat,div_temp,mpParam)

    GET_FILEDS(oInfo.extpos,current,mpParam)
    GET_FILEDS(oInfo.extpos,max_current,mpParam)
    GET_FILEDS(oInfo.extpos,max_temp_pos,mpParam)
    GET_FILEDS(oInfo.extpos,min_temp_pos,mpParam)
    GET_FILEDS(oInfo.extpos,max_vol_pos,mpParam)
    GET_FILEDS(oInfo.extpos,min_vol_pos,mpParam)

    GET_FILEDS(oInfo.balence,balence_op_1,mpParam)
    GET_FILEDS(oInfo.balence,balence_op_2,mpParam)
    GET_FILEDS(oInfo.balence,balence_op_3,mpParam)
    GET_FILEDS(oInfo.balence,balence_op_4,mpParam)
    GET_FILEDS(oInfo.balence,input_status,mpParam)
    GET_FILEDS(oInfo.balence,mos_status,mpParam)
    GET_FILEDS(oInfo.balence,current_status,mpParam)
    GET_FILEDS(oInfo.balence,switch_status,mpParam)

    GET_FILEDS(oInfo.fault1,vol_check_exce,mpParam)
    GET_FILEDS(oInfo.fault1,temp_check_exce,mpParam)
    GET_FILEDS(oInfo.fault1,connection_err,mpParam)
    GET_FILEDS(oInfo.fault1,fault1_reserve,mpParam)
    GET_FILEDS(oInfo.fault1,fault1_life,mpParam)

    GET_FILEDS(oInfo.fault2,fault2_reserve1,mpParam)
    GET_FILEDS(oInfo.fault2,warn_protect_status,mpParam)
    GET_FILEDS(oInfo.fault2,fault2_reserve2,mpParam)
    GET_FILEDS(oInfo.fault2,fault2_life,mpParam)

    GET_FILEDS(oInfo.capacity,remaining_capacity,mpParam)
    GET_FILEDS(oInfo.capacity,rated_capacity,mpParam)
    GET_FILEDS(oInfo.capacity,capacity_reserve,mpParam)
    GET_FILEDS(oInfo.capacity,SOC,mpParam)
    GET_FILEDS(oInfo.capacity,SOH,mpParam)

    GET_FILEDS(oInfo.cumulative,discharge_capacity,mpParam)
    GET_FILEDS(oInfo.cumulative,charge_capacity,mpParam)
    GET_FILEDS(oInfo.cumulative,discharge_count,mpParam)
    GET_FILEDS(oInfo.cumulative,charge_count,mpParam)

    GET_FILEDS(oInfo.vol_threshold,total_over_vol_threshold,mpParam)
    GET_FILEDS(oInfo.vol_threshold,total_under_vol_threshold,mpParam)
    GET_FILEDS(oInfo.vol_threshold,singlel_over_vol_threshold,mpParam)
    GET_FILEDS(oInfo.vol_threshold,single_under_vol_threshold,mpParam)

    GET_FILEDS(oInfo.vol_recovery,total_over_vol_recovery,mpParam)
    GET_FILEDS(oInfo.vol_recovery,total_under_vol_recovery,mpParam)
    GET_FILEDS(oInfo.vol_recovery,single_over_vol_recovery,mpParam)
    GET_FILEDS(oInfo.vol_recovery,single_under_vol_recovery,mpParam)

    GET_FILEDS(oInfo.temp_threshold,temp_charge_high_threshold,mpParam)
    GET_FILEDS(oInfo.temp_threshold,temp_charge_low_threshold,mpParam)
    GET_FILEDS(oInfo.temp_threshold,temp_discharge_high_threshold,mpParam)
    GET_FILEDS(oInfo.temp_threshold,temp_discharge_low_threshold,mpParam)

    GET_FILEDS(oInfo.temp_recovery,temp_charge_high_recovery,mpParam)
    GET_FILEDS(oInfo.temp_recovery,temp_charge_low_recovery,mpParam)
    GET_FILEDS(oInfo.temp_recovery,temp_discharge_high_recovery,mpParam)
    GET_FILEDS(oInfo.temp_recovery,temp_discharge_low_recovery,mpParam)

    GET_FILEDS(oInfo.current_threshold,curr_charge_over_threshold,mpParam)
    GET_FILEDS(oInfo.current_threshold,curr_discharge_over_threshold,mpParam)
    
    GET_FILEDS(oInfo.current_recovery,curr_charge_over_recovery,mpParam)
    GET_FILEDS(oInfo.current_recovery,curr_discharge_over_recovery,mpParam)

    GET_FILEDS(oInfo.balence_vol,balence_open_vol,mpParam)
    GET_FILEDS(oInfo.balence_vol,balence_open_dp,mpParam)
    GET_FILEDS(oInfo.balence_vol,balence_close_dp,mpParam)
    GET_FILEDS(oInfo.balence_vol,balence_reserve,mpParam)

    GET_FILEDS(oInfo.delay,over_vol_delay,mpParam)
    GET_FILEDS(oInfo.delay,under_vol_delay,mpParam)
    GET_FILEDS(oInfo.delay,over_current_delay,mpParam)
    GET_FILEDS(oInfo.delay,single_high_temp_delay,mpParam)
    GET_FILEDS(oInfo.delay,single_low_temp_delay,mpParam)
    GET_FILEDS(oInfo.delay,mos_high_temp_delay,mpParam)
    GET_FILEDS(oInfo.delay,mos_high_temp_threshold,mpParam)
    GET_FILEDS(oInfo.delay,mos_high_temp_recovery,mpParam)
}

void OutBMSSingleVol(const BMSSingleVol &oInfo, size_t idx, std::map<std::string, std::string> &mpParam)
{
    std::string prefix = "sigvol_" + ToString(idx) + "_";
    std::string key, val;

    key = prefix + "bat1_vol";
    val = ToString(oInfo.bat1_vol);
    mpParam[key] = val;

    key = prefix + "bat2_vol";
    val = ToString(oInfo.bat2_vol);
    mpParam[key] = val;

    key = prefix + "bat3_vol";
    val = ToString(oInfo.bat3_vol);
    mpParam[key] = val;

    key = prefix + "bat4_vol";
    val = ToString(oInfo.bat4_vol);
    mpParam[key] = val;
}

int T808Parser::MsgParser(const std::string &str, std::string &strRsp, bool bImeiPrefix)
{
    //Header_T808 tHeader;

    std::string strMsg(str);
    if (bImeiPrefix)
    {
        strMsg = strMsg.substr(GPP_IMEI_LEN);
    }

    int nRet = HeaderParser(strMsg, true);
    if (nRet != 0)
    {
        MYLOG_DEBUG(g_logger, "HeaderParser failed! nRet=%d", nRet);
        return nRet;
    }

    int offset = 1 + sizeof(Header_T808);
    if (m_package)
    {
        offset += 4;
    }
    std::string strBody = strMsg.substr(offset, m_bodylen);

    MYLOG_DEBUG(g_logger, "m_devId=%lu, m_strImei=%s, msg_id=%hu, offset=%d",
        m_devId, m_strImei.c_str(), m_tHeader.msg_id, offset);
    DebugBinaryStr(strBody.data(), strBody.length());
    switch(m_tHeader.msg_id)
    {
        case T808_REGISTER_MSGID:
        {
            std::string strAuthCode = "FOTA:";
            RegisterInfo oRegister;
            nRet = RegisterProtocol(strBody, oRegister);
            if (nRet == 0)
            {
                strAuthCode += ToString(m_devId);
                strRsp = RegisterMsgAck(0, strAuthCode);
            }
            else
            {
                strRsp = RegisterMsgAck(4, strAuthCode);
            }
            break;
        }
        case T808_UNREGISTER_MSGID:
        {
            strRsp = GeneralMsgAck(0);
            break;
        }
        case T808_AUTH_MSGID:
        {
            nRet = AuthProtocol(strBody);
            if (nRet == 0)
            {
                strRsp = GeneralMsgAck(0);
            }
            else
            {
                strRsp = GeneralMsgAck(1);
            }
            break;
        }
        case T808_HEARTBEAT_MSGID:
        {
            strRsp = GeneralMsgAck(0);
            std::map<std::string, std::string> mpParam;
            uint32_t uNow = time(NULL);

            mpParam["heart_time"] = ToString(uNow);
            mpParam["sys_time"] = ToString(uNow);
            saveRedisDevRunInfo(m_devId, mpParam);
            break;
        }
        case T808_GPS_MSGID:
        {
            SSMap mpParam;
            nRet = GpsProtocol(strBody, mpParam);
            if (nRet == 0)
            {
                saveRedisDevRunInfo(m_devId, mpParam);

                strRsp = GeneralMsgAck(0);
                MYLOG_DEBUG(g_logger, "GpsProtocol ssuccess. nRet=%d, m_strImei=%s, m_devId=%lu",
                    nRet, m_strImei.c_str(), m_devId);
            }
            else
            {
                strRsp = GeneralMsgAck(2);
                MYLOG_DEBUG(g_logger, "GpsProtocol failed! nRet=%d, m_strImei=%s, m_devId=%lu",
                    nRet, m_strImei.c_str(), m_devId);
            }
            break;
        }
        case T808_ADJUSTTIME_MSGID:
        {
            strRsp = AdjustTimeMsgAck();
            break;
        }
        case T808_BATCHGPS_MSGID:
        {
            nRet = BatchGpsProtocol(strBody);
            if (nRet == 0)
            {
                strRsp = GeneralMsgAck(0);
            }
            else
            {
                strRsp = GeneralMsgAck(2);
            }
            break;
        }
        case T808_BMS_MSGID:
        {
            nRet = BmsProtocol(strBody);
            if (nRet == 0)
            {
                strRsp = GeneralMsgAck(0);
            }
            else
            {
                strRsp = GeneralMsgAck(2);
            }
            break;
        }
        case T808_ICCID_MSGID:
        {
            std::string strPbMsg;
            nRet = IccidProtocol(strBody, strPbMsg);
            if (nRet == 0)
            {
                //TODO: 写kafka
                g_oReceiveList.AddData(strPbMsg);
                strRsp = GeneralMsgAck(0);
            }
            else
            {
                strRsp = GeneralMsgAck(2);
            }
            break;
        }
        case T808_CMDRSP_MSGID:
        {
            std::string strPbMsg;
            nRet = CmdRspProtocol(strBody, strPbMsg);
            if (nRet == 0)
            {
                //TODO: 写kafka
                g_oReceiveList.AddData(strPbMsg);
            }
            break;
        }
        default:
        {
            strRsp = GeneralMsgAck(3);
            break;
        }
    }

    return 0;
}

int T808Parser::HeaderParser(const std::string &str, bool bCheckPkg)
{
    std::string strHex = Goome::Binary2HexFormatString(str.data(), str.length());
    if (str.size() < T808_HEAD_LEN + 1)
    {
        MYLOG_WARN(g_logger, "Invalid msg length! length=%zu, hex=%s", str.size(), strHex.c_str());
        return -1;
    }

    //Header_T808 tHeader;
    int offset = 1; //stx 0x7e
    m_tHeader.msg_id = ntoh16(*(short *)&str[offset]);
    offset += sizeof(m_tHeader.msg_id);

    m_tHeader.attribute = ntoh16(*(short *)&str[offset]);
    offset += sizeof(m_tHeader.attribute);

    memcpy(m_tHeader.phone_no, &str[offset], sizeof(m_tHeader.phone_no));
    offset += sizeof(m_tHeader.phone_no);

    m_tHeader.seq = ntoh16(*(short *)&str[offset]);
    offset += sizeof(m_tHeader.seq);

    /* check pktlen */
    m_package = m_tHeader.attribute >> 13 & 0x1;
    if (m_package)
    {
        m_packageNum = ntoh16(*(short *)&str[offset]);
        offset += 2; //分包选项

        m_packageIdx = ntoh16(*(short *)&str[offset]);
        offset += 2;
    }

    m_bodylen = m_tHeader.attribute & 0x03FF;
    m_packagelen = offset + m_bodylen + 2;
    if (m_packagelen != str.size())
    {
        MYLOG_WARN(g_logger, "Invalid body length! offset=%d, bodylen=%hu, packagelen=%hu, length=%zu, hex=%s",
            offset, m_bodylen, m_packagelen, str.size(), strHex.c_str());
    	//return -2;
    }

    m_encrpy = m_tHeader.attribute >> 10 & 0x3;
    m_strImei = TransPhonenoToImei((const char *)m_tHeader.phone_no, sizeof(m_tHeader.phone_no));

    return 0;
}

int T808Parser::RegisterProtocol(const std::string &str, RegisterInfo &regist)
{
    int offset = 0;
    size_t msgSize = str.size();

    regist.province = ntoh16(*(uint16_t*)&str[offset]);
    offset += sizeof(regist.province);

    regist.city = ntoh16(*(uint16_t*)&str[offset]);
    offset += sizeof(regist.city);

    memcpy(regist.vendor, &str[offset], sizeof(regist.vendor));
    offset += sizeof(regist.vendor);

    //2013 version, 20byte termType, 从第38字节开始是车辆标识
    if(msgSize > 44)   //2013 version, 20byte termType, 从第38字节开始是车辆标识
    {
        memcpy(regist.termType, &str[offset], 20);
        offset += 20;

        memcpy(regist.termId, &str[offset], 7);
        offset += 7;

        regist.color = *(uint8_t*)&str[offset];
        offset += 1;

        uint32_t licenselen = (m_bodylen - offset) > 32 ? 32:(m_bodylen - offset);
        strncpy(regist.license, (char*)&str[offset], licenselen);
    }
    else    //2011 version, 8byte termType
    {
        memcpy(regist.termType, &str[offset], 8);
        offset += 8;

        memcpy(regist.termId, &str[offset], 7);
        offset += 7;

        regist.color = *(uint8_t*)&str[offset];
        offset += 1;

        uint32_t licenselen = (m_bodylen - offset) > 32 ? 32:(m_bodylen - offset);
        strncpy(regist.license, (char*)&str[offset], licenselen);
    }
    return 0;
}


int T808Parser::GpsProtocol(const std::string &str, SSMap &mpParam)
{
    const uint32_t k_GpsDataLen = 28;
    size_t msgSize = str.size();
    /* check size */
    if (msgSize < k_GpsDataLen)
    {
        MYLOG_DEBUG(g_logger, "msg size error! msgSize=%zu, k_GpsDataLen=%u", msgSize, k_GpsDataLen);
        return -1;
    }

    YunJi::GpsMsg oPbGps;
    oPbGps.set_id(m_devId);
    oPbGps.set_imei(m_strImei);
    mpParam["devid"] = ToString(m_devId);

    uint32_t uNow = time(NULL);
    oPbGps.set_systime(uNow);
    mpParam["sys_time"] = ToString(uNow);

    oPbGps.set_seq(m_tHeader.seq);

    int offset = 0;
    long lVal = 0;
    lVal = ntoh32(*(int *)&str[offset]);
    mpParam["alarmcode"] = ToString(lVal);
    offset += 4;

    int status = ntoh32(*(int *)&str[offset]);
    mpParam["status"] = ToString(status);
    offset += 4;

    int acc_status = GETBIT(status, 0);
    int gps_status = GETBIT(status, 1) >> 1;
    mpParam["acc_status"] = ToString(acc_status);
    mpParam["gps_status"] = ToString(gps_status);

    lVal = ntoh32(*(int *)&str[offset]);	// 带正负符号，已表明南纬/北纬
    lVal = (GETBIT(status, 2) != 0) ? -lVal : lVal;
    oPbGps.set_lat(lVal);
    //mpParam["latitude"] = ToString(lVal);
    offset += 4;

    lVal = ntoh32(*(int *)&str[offset]);	// 带正负符号，已表明东经/西经
    lVal = (GETBIT(status, 3) != 0) ? -lVal : lVal;
    oPbGps.set_lng(lVal);
    //mpParam["longitude"] = ToString(lVal);
    offset += 4;

    lVal = ntoh16(*(short *)&str[offset]);  //海拔
    //mpParam["alt"] = ToString(lVal);
    mpParam["altitude"] = ToString(lVal);
    offset += 2;

    lVal = ntoh16(*(short *)&str[offset]);

    //设备上报速度单位是0.1km/h 需除以10
    lVal = round(double(lVal) * 0.1);
    oPbGps.set_speed(lVal);
    //mpParam["speed"] = ToString(lVal);
    offset += 2;

    lVal = ntoh16(*(short *)&str[offset]);
    oPbGps.set_route(lVal);
    //mpParam["course"] = ToString(lVal);
    offset += 2;

	lVal = Goome::BCD2Time(&str[offset], 6);
    oPbGps.set_datetime(lVal);
    //mpParam["gps_time"] = ToString(lVal);
    offset += 6;

    if (msgSize > k_GpsDataLen)
	{
        //解析扩展字段
        std::string cmd;
        int iLoop = msgSize - k_GpsDataLen;
        uint8_t uCmd = 0;
        uint8_t uLenVal = 0;
    	while (iLoop > 2)
    	{
            uCmd = *(uint8_t*)&str[offset];
    		uLenVal = *(uint8_t*)&str[offset+1];
    		if (uLenVal + 2 <= iLoop)
    		{
                if (uCmd == T808_WATERLEVEL_CMD)
                {
                    cmd = "water_level";
                }
                else if (uCmd == T808_LBS_CMD)
                {
                    cmd = "lbs_info";
                }
                else
                {
                    cmd = "cmd_" + Goome::Binary2HexFormatString((const char *)&str[offset], 1);
                }

                if (uLenVal > 0)
                {
                    if (uCmd == T808_LBS_CMD)
                    {
                        std::string strVal((const char *)&str[offset+2], uLenVal);
                        mpParam[cmd] = strVal;
                        LbsProtocol(oPbGps.datetime(), strVal);
                    }
                    else
                    {
                        mpParam[cmd] = Goome::Binary2HexFormatString((const char *)&str[offset+2], uLenVal);
                    }
                }
                else
                {
                    mpParam[cmd] = "";
                }

    			iLoop -= (uLenVal + 2);
				offset += uLenVal+2;
    		}
    		else
    		{
    			break;
    		}
    	}
	}

    if (gps_status)
    {
        //更新redis位置信息
        mpParam["latitude"] = ToString(oPbGps.lat());
        mpParam["longitude"] = ToString(oPbGps.lng());
        mpParam["speed"] = ToString(oPbGps.speed());
        mpParam["course"] = ToString(oPbGps.route());
        mpParam["gps_time"] = ToString(oPbGps.datetime());

        //发送gps到消息队列
        YunJi::UpDevMsg oPbMsg;
        oPbMsg.set_msgtype(YunJi::kGpsMsg);
        *(oPbMsg.mutable_gps()) = oPbGps;

        std::string strPbRes;
        oPbMsg.SerializeToString(&strPbRes);

        g_oReceiveList.AddData(strPbRes);
    }
    return 0;
}

int T808Parser::AuthProtocol(const std::string &str)
{
    std::string strAuthCode(str);
    std::string strPlatAuthCode = "FOTA:" + ToString(m_devId);
    if (strAuthCode != strPlatAuthCode)
    {
        return -1;
    }

    return 0;
}

int T808Parser::BatchGpsProtocol(const std::string &str)
{
    const uint32_t k_PreDataLen = 5;
    const uint32_t k_GpsDataLen = 28; //5 + n*28
    size_t msgSize = str.size();;
    /* check size */
    if (msgSize < k_PreDataLen)
    {
        return -1;
    }

    uint16_t offset = 0;
    uint16_t gps_num = ntoh16(*(short *)&str[offset]);
    offset += 2;

    uint8_t gps_type = *(uint8_t*)&str[offset];
    offset += 1;

    int nRet = 0;
    std::string strGpsData;
    std::map<std::string, std::string> mpParam;

    do{
        strGpsData.clear();
        mpParam.clear();

        uint16_t gps_len = ntoh16(*(short *)&str[offset]);
        offset += 2;

        if (offset + gps_len > msgSize)
        {
            break;
        }

        if (gps_len >= k_GpsDataLen)
        {
            strGpsData = str.substr(offset, gps_len);
            nRet = GpsProtocol(strGpsData, mpParam);
            if (nRet == 0)
            {
                if (gps_type == 0)
                {
                    //mpParam["sys_time"] = ToString(time(NULL));
                    saveRedisDevRunInfo(m_devId, mpParam);
                }

                //strGpsRecord = ToGpsRecordStr(PROTOCOL_T808, mpParam);
                //g_oReceiveList.AddData(strGpsRecord);
            }
        }

        gps_num--;
        offset += gps_len;
    }while(gps_num > 0);

    return 0;
}

int T808Parser::BmsProtocol(const std::string &str)
{
    std::string strBmsInfo = Goome::Binary2HexFormatString(str.data(), str.length());
    std::map<std::string, std::string> mpParam;
    if (str.size() < sizeof(BMSInfo))
    {
        MYLOG_WARN(g_logger, "invalid BMSInfo. m_strImei=%s, m_devId=%lu, strBmsInfo=%s",
            m_strImei.c_str(), m_devId, strBmsInfo.c_str());
        mpParam["bms"] = strBmsInfo;
    }
    else
    {
        const BMSInfo *pBms = (const BMSInfo *)str.data();
        OutBMSInfo(*pBms, mpParam);

        BMSSingleVol oSingleVol;
        size_t sigVolLen = str.size() - sizeof(BMSInfo);
        size_t sigVolNum = sigVolLen / sizeof(BMSSingleVol);
        for(size_t idx = 0; idx < sigVolNum && idx < 8; ++idx)
        {
            memcpy(&oSingleVol, pBms->single_vol+idx*sizeof(BMSSingleVol), sizeof(BMSSingleVol));
            OutBMSSingleVol(oSingleVol, idx, mpParam);
        }

        //BMS状态
        std::string alarm_status;
        std::bitset<32> bms_status;
        if (pBms->fault1.vol_check_exce != 0)
        {
            bms_status.set(E_ABNORMAL_VOL_ACQUISITION);
            alarm_status = ToString(pBms->fault1.vol_check_exce);
            sendBmsAlarm(BMS_ABNORMAL_VOL_ACQUISITION, alarm_status);
        }

        if (pBms->fault1.temp_check_exce != 0)
        {
            bms_status.set(E_ABNORMAL_TEMP_ACQUISITION);
            alarm_status = ToString((int)pBms->fault1.temp_check_exce);
            sendBmsAlarm(BMS_ABNORMAL_TEMP_ACQUISITION, alarm_status);
        }

        if (pBms->fault1.connection_err != 0)
        {
            bms_status.set(E_COMMUNICATION_FAULT);
            if (pBms->fault1.connection_err & 0x1)
            {
                sendBmsAlarm(BMS_HIGH_AFE_COMMUNICATION_FAULT, "");
            }
            else if (pBms->fault1.connection_err & 0x2)
            {
                sendBmsAlarm(BMS_LOW_AFE_COMMUNICATION_FAULT, "");
            }
        }

        std::bitset<16> bs_fault2(pBms->fault2.warn_protect_status);
        for (size_t idx=0; idx < bs_fault2.size(); ++idx)
        {
            if (bs_fault2.test(idx))
            {
                bms_status.set(E_FAULT2_BIT_BASE + idx);
                sendBmsAlarm(BMS_FAULT2_ALARM_BASE + idx, "");
            }
        }

        mpParam["bms_status"] = ToString(bms_status.to_ullong());
    }

    saveRedisBmsInfo(m_devId, mpParam);
    //
    return 0;
}


int T808Parser::IccidProtocol(const std::string &str, std::string &strPbMsg)
{
    //std::string strIccid = Goome::Binary2HexFormatString(str.data(), str.length());
    std::string strIccid(str);
    MYLOG_WARN(g_logger, "m_strImei=%s, m_devId=%lu, strIccid=%s",
            m_strImei.c_str(), m_devId, strIccid.c_str());

    //TODO:
    //saveRedisIccidInfo(m_devId, strIccid);

    YunJi::IccidMsg oPbIccid;
    oPbIccid.set_id(m_devId);
    oPbIccid.set_imei(m_strImei);
    oPbIccid.set_iccid(strIccid);

    YunJi::UpDevMsg oPbMsg;
    oPbMsg.set_msgtype(YunJi::kIccidMsg);
    *(oPbMsg.mutable_iccid()) = oPbIccid;

    oPbMsg.SerializeToString(&strPbMsg);
    return 0;
}

int T808Parser::CmdRspProtocol(const std::string &str, std::string &strPbMsg)
{
    std::string strCmdRsp(str);
    MYLOG_WARN(g_logger, "m_strImei=%s, m_devId=%lu, strCmdRsp=%s",
            m_strImei.c_str(), m_devId, strCmdRsp.c_str());

    YunJi::CommandRsp oPbCmdRsp;
    oPbCmdRsp.set_id(m_devId);
    oPbCmdRsp.set_imei(m_strImei);

    uint32_t uNow = time(NULL);
    oPbCmdRsp.set_rsptime(uNow);
    oPbCmdRsp.set_rsp(strCmdRsp);

    YunJi::UpDevMsg oPbMsg;
    oPbMsg.set_msgtype(YunJi::kCommandRsp);
    *(oPbMsg.mutable_cmdrsp()) = oPbCmdRsp;

    oPbMsg.SerializeToString(&strPbMsg);
    return 0;
}

int T808Parser::LbsProtocol(uint32_t dateTime, const std::string &str)
{
    std::string strLbsInfo(str);
    MYLOG_DEBUG(g_logger, "m_strImei=%s, m_devId=%lu, dateTime=%u, strLbsInfo=%s",
            m_strImei.c_str(), m_devId, dateTime, strLbsInfo.c_str());

    if (strLbsInfo.empty())
    {
        return 0;
    }

    //mcc;mnc;lac1,ci1,rssi1;lac2,ci2,rssi2;
    SVec vecLbs;
    Goome::SplitString(strLbsInfo, ";", vecLbs);
    if (vecLbs.size() < 3)
    {
        MYLOG_WARN(g_logger, "LbsInfo error! strLbsInfo=%s, vecLbs size=%zu", 
            strLbsInfo.c_str(), vecLbs.size());
        return -1;
    }
    
    YunJi::LbsMsg *pbLbs = new YunJi::LbsMsg;
    if (pbLbs)
    {
        pbLbs->set_id(m_devId);
        pbLbs->set_imei(m_strImei);
        pbLbs->set_seq(m_tHeader.seq);
        pbLbs->set_datetime(dateTime);
        pbLbs->set_systime(time(NULL));
        pbLbs->set_mcc(vecLbs[0]);
        pbLbs->set_mnc(vecLbs[1]);

        bool bSetArea = false;
        int rssi = 0;
        SVec vecArea;
        for (size_t idx = 2; idx < vecLbs.size(); ++idx)
        {
            vecArea.clear();
            const std::string &strAreaInfo = vecLbs[idx];
            Goome::SplitString(strAreaInfo, ",", vecArea);

            if (vecArea.size() < 3)
            {
                MYLOG_WARN(g_logger, "LbsAreaInfo error! strLbsInfo=%s, strAreaInfo=%s, vecLbs size=%zu, vecArea size=%zu", 
                    strLbsInfo.c_str(), strAreaInfo.c_str(), vecLbs.size(), vecArea.size());
                continue;
            }

            YunJi::LbsMsg_LbsInfo *pbAreaInfo = pbLbs->add_lbsinfo();
            if (pbAreaInfo)
            {
                bSetArea = true;
                pbAreaInfo->set_lac(vecArea[0]);
                pbAreaInfo->set_ci(vecArea[1]);

                rssi = Goome::HexStr2long(vecArea[2].c_str());
                pbAreaInfo->set_rssi(rssi);
            }
        }

        if (!bSetArea)
        {
            delete pbLbs;
            return -2;
        }

        MYLOG_DEBUG(g_logger, "Lbs info: %s", pbLbs->DebugString().c_str());

        Job oJob;
        oJob.type = "lbs";
        oJob.pLbsMsg = pbLbs;
        GAsyncCurlThread::AddJob2Queue(oJob);
    }
    
    return 0;
}


//通用应答
std::string T808Parser::GeneralMsgAck(uint8_t result)
{
    //uint8_t rsp[128] = {0};
    uint16_t bodylen = 5;
    uint16_t pkglen = sizeof(Header_T808) + bodylen + 3;

    uint8_t *pRsp = (uint8_t*)malloc(pkglen*3);
    size_t rspLen = 0;

    *(uint8_t*)pRsp = 0x7e;
	rspLen += 1;

    Header_T808 tHeader;
    tHeader.msg_id = hton16(0x8001);
    tHeader.attribute = hton16(bodylen);
    memcpy(tHeader.phone_no, m_tHeader.phone_no, sizeof(m_tHeader.phone_no));
    tHeader.seq = hton16(m_tHeader.seq);
    memcpy(pRsp + rspLen, &tHeader, sizeof(tHeader));
    rspLen += sizeof(tHeader);

    *(uint16_t*)(pRsp + rspLen) = hton16(m_tHeader.seq);
	rspLen += 2;

    *(uint16_t*)(pRsp + rspLen) = hton16(m_tHeader.msg_id);
	rspLen += 2;

    *(uint8_t*)(pRsp + rspLen)= result;
	rspLen += 1;

    uint8_t myxor = calc_xor((uint8_t*)(pRsp+1), pkglen-3);
    *(uint8_t*)(pRsp + rspLen)= myxor;
    rspLen += 1;

    *(uint8_t*)(pRsp + rspLen) = 0x7e;
    rspLen += 1;

    uint8_t *pDst = pRsp + rspLen;
    uint32_t dst_len = 2 * pkglen;

    std::string strRsp;
    if (rspLen == pkglen && Transfer0x7e((uint8_t*)pRsp, rspLen, pDst, &dst_len))
    {
        strRsp.assign((const char*)pDst, dst_len);
	}
	else
	{
		MYLOG_WARN(g_logger, "build failed! imei=%s, rspLen=%zu, pkglen=%hu, dst_len=%u",
            m_strImei.c_str(), rspLen, pkglen, dst_len);
	}

    free(pRsp);
    return strRsp;
}


//注册应答
std::string T808Parser::RegisterMsgAck(uint8_t result, std::string &auCode)
{
    //uint8_t rsp[128] = {0};
    uint16_t bodylen = auCode.length() + 3;
    uint16_t pkglen = sizeof(Header_T808) + bodylen + 3;

    uint8_t *pRsp = (uint8_t*)malloc(pkglen*3);
    size_t rspLen = 0;

    *(uint8_t*)pRsp = 0x7e;
	rspLen += 1;

    Header_T808 tHeader;
    tHeader.msg_id = hton16(0x8100);
    tHeader.attribute = hton16(bodylen);
    memcpy(tHeader.phone_no, m_tHeader.phone_no, sizeof(m_tHeader.phone_no));
    tHeader.seq = hton16(m_tHeader.seq);
    memcpy(pRsp + rspLen, &tHeader, sizeof(tHeader));
    rspLen += sizeof(tHeader);

    *(uint16_t*)(pRsp + rspLen) = hton16(m_tHeader.seq);
	rspLen += 2;

    *(uint8_t*)(pRsp + rspLen)= result;
	rspLen += 1;

	memcpy(pRsp+rspLen, auCode.data(), auCode.length());
    rspLen += auCode.length();

    uint8_t myxor = calc_xor((uint8_t*)(pRsp+1), pkglen-3);
    *(uint8_t*)(pRsp + rspLen)= myxor;
    rspLen += 1;

    *(uint8_t*)(pRsp + rspLen) = 0x7e;
    rspLen += 1;

    uint8_t *pDst = pRsp + rspLen;
    uint32_t dst_len = 2 * pkglen;

    std::string strRsp;
    if (rspLen == pkglen && Transfer0x7e((uint8_t*)pRsp, rspLen, pDst, &dst_len))
    {
        strRsp.assign((const char*)pDst, dst_len);
	}
	else
	{
		MYLOG_WARN(g_logger, "build failed! imei=%s, rspLen=%zu, pkglen=%hu, dst_len=%u",
            m_strImei.c_str(), rspLen, pkglen, dst_len);
	}

    free(pRsp);
    return strRsp;
}

//无消息体应答
std::string T808Parser::HeadOnlyMsgAck()
{
    //uint8_t rsp[128] = {0};
    uint16_t bodylen = 0;
    uint16_t pkglen = sizeof(Header_T808) + bodylen + 3;

    uint8_t *pRsp = (uint8_t*)malloc(pkglen*3);
    size_t rspLen = 0;

    *(uint8_t*)pRsp = 0x7e;
	rspLen += 1;

    Header_T808 tHeader;
    tHeader.msg_id = hton16(m_tHeader.msg_id);
    tHeader.attribute = hton16(bodylen);
    memcpy(tHeader.phone_no, m_tHeader.phone_no, sizeof(m_tHeader.phone_no));
    tHeader.seq = hton16(m_tHeader.seq);
    memcpy(pRsp + rspLen, &tHeader, sizeof(tHeader));
    rspLen += sizeof(tHeader);

    uint8_t myxor = calc_xor((uint8_t*)(pRsp+1), pkglen-3);
    *(uint8_t*)(pRsp + rspLen)= myxor;
    rspLen += 1;

    *(uint8_t*)(pRsp + rspLen) = 0x7e;
    rspLen += 1;

    uint8_t *pDst = pRsp + rspLen;
    uint32_t dst_len = 2 * pkglen;

    std::string strRsp;
    if (rspLen == pkglen && Transfer0x7e((uint8_t*)pRsp, rspLen, pDst, &dst_len))
    {
        strRsp.assign((const char*)pDst, dst_len);
	}
	else
	{
		MYLOG_WARN(g_logger, "build failed! imei=%s, rspLen=%zu, pkglen=%hu, dst_len=%u",
            m_strImei.c_str(), rspLen, pkglen, dst_len);
	}

    free(pRsp);
    return strRsp;
}

//时间校准应答
std::string T808Parser::AdjustTimeMsgAck()
{
    //uint8_t rsp[128] = {0};
    uint16_t bodylen = 7;
    uint16_t pkglen = sizeof(Header_T808) + bodylen + 3;

    uint8_t *pRsp = (uint8_t*)malloc(pkglen*3);
    size_t rspLen = 0;

    *(uint8_t*)pRsp = 0x7e;
	rspLen += 1;

    Header_T808 tHeader;
    tHeader.msg_id = hton16(0x8109);
    tHeader.attribute = hton16(bodylen);
    memcpy(tHeader.phone_no, m_tHeader.phone_no, sizeof(m_tHeader.phone_no));
    tHeader.seq = hton16(m_tHeader.seq);
    memcpy(pRsp + rspLen, &tHeader, sizeof(tHeader));
    rspLen += sizeof(tHeader);

    time_t now = time(NULL);
    struct tm localtm;
	localtime_r(&now, &localtm);
    *(uint16_t*)(pRsp + rspLen) = hton16(localtm.tm_year + 1900);
	rspLen += 2;

    *(uint8_t*)(pRsp + rspLen)= localtm.tm_mon + 1;
	rspLen += 1;

    *(uint8_t*)(pRsp + rspLen)= localtm.tm_mday;
	rspLen += 1;

    *(uint8_t*)(pRsp + rspLen)= localtm.tm_hour;
	rspLen += 1;

    *(uint8_t*)(pRsp + rspLen)= localtm.tm_min;
	rspLen += 1;

    *(uint8_t*)(pRsp + rspLen)= localtm.tm_sec;
	rspLen += 1;

    uint8_t myxor = calc_xor((uint8_t*)(pRsp+1), pkglen-3);
    *(uint8_t*)(pRsp + rspLen)= myxor;
    rspLen += 1;

    *(uint8_t*)(pRsp + rspLen) = 0x7e;
    rspLen += 1;

    uint8_t *pDst = pRsp + rspLen;
    uint32_t dst_len = 2 * pkglen;

    std::string strRsp;
    if (rspLen == pkglen && Transfer0x7e((uint8_t*)pRsp, rspLen, pDst, &dst_len))
    {
        strRsp.assign((const char*)pDst, dst_len);
	}
	else
	{
		MYLOG_WARN(g_logger, "build failed! imei=%s, rspLen=%zu, pkglen=%hu, dst_len=%u",
            m_strImei.c_str(), rspLen, pkglen, dst_len);
	}

    free(pRsp);
    return strRsp;
}

//指令请求
std::string T808Parser::CommandReqMsg(const std::string &imei, const std::string &content)
{
    //uint8_t rsp[128] = {0};
    uint16_t bodylen = content.size() + 1; //1byte flag
    uint16_t pkglen = sizeof(Header_T808) + bodylen + 3;

    uint8_t *pRsp = (uint8_t*)malloc(pkglen*3);
    size_t rspLen = 0;

    *(uint8_t*)pRsp = 0x7e;
	rspLen += 1;

    std::string subImei = imei.substr(imei.size()-12, 12);
    std::string phone_no = Goome::Hex2Bin(subImei);

    Header_T808 tHeader;
    tHeader.msg_id = hton16(0x8300);
    tHeader.attribute = hton16(bodylen);
    memcpy(tHeader.phone_no, phone_no.data(), phone_no.length());
    tHeader.seq = hton16(generateSeqno());
    memcpy(pRsp + rspLen, &tHeader, sizeof(tHeader));
    rspLen += sizeof(tHeader);

    uint8_t flag = 0x0;
    *(uint8_t*)(pRsp + rspLen)= flag;
	rspLen += 1;

    memcpy(pRsp+rspLen, content.data(), content.length());
    rspLen += content.length();

    uint8_t myxor = calc_xor((uint8_t*)(pRsp+1), pkglen-3);
    *(uint8_t*)(pRsp + rspLen)= myxor;
    rspLen += 1;

    *(uint8_t*)(pRsp + rspLen) = 0x7e;
    rspLen += 1;

    uint8_t *pDst = pRsp + rspLen;
    uint32_t dst_len = 2 * pkglen;

    std::string strRsp;
    if (rspLen == pkglen && Transfer0x7e((uint8_t*)pRsp, rspLen, pDst, &dst_len))
    {
        strRsp.assign((const char*)pDst, dst_len);
	}
	else
	{
		MYLOG_WARN(g_logger, "build failed! imei=%s, rspLen=%zu, pkglen=%hu, dst_len=%u",
            imei.c_str(), rspLen, pkglen, dst_len);
	}

    free(pRsp);
    return strRsp;
}


bool T808Parser::Transfer0x7e(uint8_t* src_buf , uint32_t src_len, uint8_t* dst_buf, uint32_t* p_dst_len)
{
    uint8_t* src_current = src_buf;
    uint8_t* dst_current = dst_buf ;
    uint32_t dst_len = 0;
    *dst_current ++ = *src_current++;
    for(uint32_t i = 0; i < src_len-2 && (*p_dst_len-2 >= dst_len); ++i,++src_current, ++dst_current)
    {
        if(*src_current == 0x7e)
        {
            *dst_current = 0x7d;
            if(*p_dst_len - 2 > dst_len)
            {
                *(++dst_current) = 0x02;
                dst_len++;
            }
        }
        else if(*src_current == 0x7d)
        {
            *dst_current = *src_current;
            if(*p_dst_len -2 > dst_len)
            {
                *(++dst_current) = 0x01;
                dst_len++;
            }
        }
        else
        {
            *dst_current = *src_current;
        }
        dst_len ++;
    }

    if(*p_dst_len < dst_len)
    {
        return false;
    }
    *dst_current = *src_current;

    *p_dst_len = dst_len + 2;
    return true;
}

//只转义第一条数据，防止多次转义  如 7d010101 转义为 7d0101 后 如果再次转义还会 译成 7d01
int T808Parser::Transform0x7e(uint8_t* buf, uint32_t* packageSize, uint32_t* otherSize)
{
    if (*buf != 0x7e || *packageSize < 2)
    {
        return PACKET_INCOMPLETE;
    }
    uint32_t end = 1;
    for (; end < *packageSize; ++end)
    {
        if (*(buf+end) == 0x7e)
        {
            break;
        }
    }
    if(end < *packageSize)
    {
        //找到一个完整的包
        *otherSize = end + 1;
        *packageSize = end + 1;
    }
    else
    {
        //没找到完整的包，不进行任何转义
        return PACKET_INCOMPLETE;
    }

    uint8_t* current = buf + 1;
    uint32_t skipped = 0;

    for(uint32_t i = 1; i < *packageSize-1; i++,current++)//循环不包括最前的0x7e 和最后的0x7e
    {
        if (*current == 0x7d)
        {
            if ( i+1 >= *packageSize -1 )
            {
                return TRANSFORM0X7E_ERROR;
            }
            if(*(current+1) == 0x02)
            {
                *(current-skipped) = 0x7e;
            }
            else if(*(current+1) == 0x01)
            {
                *(current-skipped) = 0x7d;
            }
            else
            {
                return TRANSFORM0X7E_ERROR;
            }
            ++i;
            ++current;
            ++skipped;
        }
        else
        {
            *(current-skipped) = *current;
        }
    }
    *(current-skipped) = *current;
    *packageSize -= skipped;
    return 0;
}



std::string T808Parser::TransPhonenoToImei(const char *phone_no, unsigned length)
{
    std::string strImei = "000000000000000";
    std::string strPhoneno = Goome::Binary2HexFormatString((const char*)m_tHeader.phone_no, sizeof(m_tHeader.phone_no));

    if (strPhoneno.length() < IMEI_STR_LEN)
    {
        size_t pad_len = IMEI_STR_LEN - strPhoneno.length();
        strImei = strImei.substr(0, pad_len) + strPhoneno;
    }
    else
    {
        strImei = strPhoneno.substr(0, IMEI_STR_LEN);
    }

    return strImei;
}

void T808Parser::sendBmsAlarm(int alarm_type, const std::string &status)
{
    YunJi::AlarmMsg oPbAlarm;
    oPbAlarm.set_id(m_devId);
    oPbAlarm.set_imei(m_strImei);
    oPbAlarm.set_alarmtime(time(NULL));
    oPbAlarm.set_alarmtype(alarm_type);
    oPbAlarm.set_status(status);

    YunJi::UpDevMsg oPbMsg;
    oPbMsg.set_msgtype(YunJi::kAlarmMsg);
    *(oPbMsg.mutable_alarm()) = oPbAlarm;

    std::string strPbMsg;
    oPbMsg.SerializeToString(&strPbMsg);
    g_oReceiveList.AddData(strPbMsg);
}

