#ifndef  _GOOCAR_COMMON_ERROR_CODE_H_
#define  _GOOCAR_COMMON_ERROR_CODE_H_

#include <string>

enum error_code_e
{
    ERR_OK = 0,
    //////////// System error /////////////////
    // ϵͳ�ڲ�������Ҫ�����޸�
    ERR_SYSTEM_ERR_BEGIN = 1000,
    ERR_SYSTEM_ERROR,           // ������ǵ�˵����һ�㲻����ʹ��
    ERR_CONNECT_FAIL,           // ����Ŀ�����ʧ��
    ERR_CALL_CGI_FAIL,
    ERR_CALL_PROXY_FAIL,
    ERR_CALL_SERVANT_FAIL,
    ERR_LOG_USER_OPERATE,       //�û�������־���ʧ��
    ERR_PROTOBUF_SERIALIZE_ERR, //���л�Protobuf���ݳ���
    ERR_PROTOBUF_PARSE_ERR,     // ����Protobuf���ݳ���
    ERR_PASS_OTHER_SHARD_DEVICE,  // ������������Ƭ���豸
    ERR_TIMEOUT,               // ���ó�ʱ
    ERR_ICE_EXCEPTION,         // ICE�����쳣
    ERR_CGI_PROCESSOR_NOT_EXIST, //processor������
    ERR_CGI_CONFIG_ERR,          //CGI���ô���
    
    //ICE PARALL ģ�����
    ERR_ICE_PARALL_PARAM_EMPTY = 1100,
    ERR_ICE_PARALL_MAP_FUNC_ERROR,
    ERR_ICE_PARALL_ENGINE_BUSY,
    ERR_ICE_PARALL_MAP_FUNC_TIME_OUT,
    ERR_ICE_PARALL_MAP_FUNC_RETURN_ERROR,
    ERR_ICE_PARALL_REDUCE_ERROR,
    ERR_ICE_PARALL_THREAD_POOL_NOT_INIT,
    ERR_ICE_PARALL_THREAD_LOCK_MUTEX_ERROR,
    ERR_SERVANT_PARALL_BAD_ANY_CAST = 1120,
    ERR_SERVANT_PARALL_ICE_CALL_EXCEPTION,
    ERR_SERVANT_PARALL_CALL_TYPE_UNKNOWN,
    ERR_SERVANT_PARALL_CALL_PARAM_VALUE_EMPTY,
    ERR_SERVANT_PARALL_MAP_REQUEST_EMPTY,
    ERR_PART_GNS_ICE_FAIL = 1140,     // ��ȡ���GNS��ICE���ʱ�����ֻ�ȡ��ʧ��
    ERR_ALL_GNS_ICE_FAIL,               // ��ȡ���GNS��ICE���ʱ��ȫ����ȡʧ��

    //����ϵͳģ�����
    ERR_CALL_SYS_MOD_BEGIN = 1200,
    ERR_CALL_GUD_FAIL,
    ERR_CALL_REDIS_FAIL,
    ERR_CALL_MYSQL_FAIL,
    ERR_CALL_GTC_FAIL,
    ERR_CALL_GMC_FAIL,
    ERR_CALL_GFS_FAIL,
    ERR_CALL_GMQ_FAIL,
    ERR_CALL_GSE_FAIL,
    ERR_CALL_GGB_FAIL,
    ERR_CALL_ACC_FAIL,
    ERR_CALL_DB_PROXY_FAIL,
	ERR_CALL_GOOMEPAY_FAIL,
	ERR_CALL_GOOMEIM_FAIL,
    
    //�����ⲿģ�����
    ERR_CALL_OTHER_MOD_BEGIN = 1300,
    ERR_CALL_GTREE_FAIL,
    ERR_CALL_GIMEI_FAIL,
    ERR_CALL_GLNAME_FAIL,
    ERR_CALL_GENID_FAIL,
    ERR_CALL_GALARM_SERVANT_FAIL,
    ERR_CALL_TBO_SERVANT_FAIL,
    ERR_CALL_BUS_PROXY_FAIL,
    ERR_CALL_GCID_FAIL,
    ERR_CALL_GIMAGE_FAIL,
    ERR_CALL_IOT_FAIL,  //����IOT_BO����������ʧ��
    ERR_CALL_IOT_ISPGW_FAIL,  
    ERR_CALL_IOT_CMPPGW_FAIL,
    ERR_CALL_IOT_CARDPOOL_FAIL,
    
    // �����ӵ�ϵͳ�ڲ��������ͣ�ֻ����ӵ������棬�����Դ��������˳��Ҳ�������ɾ��������������ǹ̶��ģ�

    //////////// Data error /////////////////
    // ϵͳ�������д���
    ERR_DATA_ERR_BEGIN = 2000,
    ERR_JSON_FORMAT,
    ERR_DATA_SIZE,
    ERR_EID_NOT_EXIST,      // ָ���Ŀͻ�ID������
    ERR_IMEI_NOT_EXIST,
    ERR_HTTP_QUERY_FAIL,    // HTTP����ʧ��
    ERR_METHOD_NOT_SUPPORTED,
    // �����ӵ�ϵͳ�����ݴ������ͣ�ֻ����ӵ������棬�����Դ��������˳��Ҳ�������ɾ��������������ǹ̶��ģ�

    //////////// User error /////////////////
    // ���û��ύ�Ĳ������Ϲ淶����Ĵ���
    ERR_USER_ERR_BEGIN = 3000,
    ERR_INVALIDATE_PARAM,   // �����д���
    ERR_LACK_PARAM,         // ȱ�ٲ���
    ERR_PARSE_USER_TICKET,  // �����û����ʧ��
    ERR_NOT_YET_LOGIN,      // �û���δ��½���޲���Ȩ��
    ERR_OP_RATE_LIMIT,      // ����Ƶ������
    ERR_METHOD_NOT_EXIST,   // ���÷���������
    ERR_PASSWORD,           // �û��������
    ERR_USER_NOT_EXIST,
    ERR_DATA_NOT_FOUND,     // δ���ҵ���������������
    ERR_QUERY_NUMBER_LIMIT, // ��ѯ��������
    ERR_CHECK_FAIL,         // ���ݼ�鲻ͨ��
    ERR_NOT_ENOUGH_PERMISSION, // ����Ȩ�޲���
    ERR_INVALID_OPERATION,   // �Ƿ�����
    ERR_CHECK_SIGN_FAIL,     // SIGNУ��ʧ��
    ERR_LOGIN_NAME_ALREADY_EXIST,  //ע���¼���Ѵ���
    ERR_RESULT_SET_TOO_LARGE,  //�����̫��
    ERR_WL_CARD_OUT_TIME,		//�û�����������
    ERR_WL_MSISDN_EMPTY,	//������ר����Ϊ�գ������·�ָ��
    ERR_SIGN_HAS_EXPIRED,       // SIGN�ѹ���
    ERR_NOT_ENOUGH_HISTORY_PERMISSION, // ��ѯ��ʷ�켣Ȩ�޲���
    ERR_PLACE_FOR_DEPT,       //�ø��ڵ㲻�ܲ��벿��
    ERR_CANT_MODIFY_DEPT,     //�������Բ��ܱ��޸�
    ERR_DEPT_ONLY_HAS_DEPT,   //��������ֻ���ǲ���

    // �����ӵ��û������������ͣ�ֻ����ӵ������棬�����Դ��������˳��Ҳ�������ɾ��������������ǹ̶��ģ�
    ERR_CHARGE_MODULE_BEGIN = 4000, // ��ֵ��ģ�� + ���ų�ֵ������
    ERR_CHARGE_REPEAT_COMMIT,        // 5minute not repeat summit order
    //ERR_CHARGE_INVALIDATE_EID,      // ��ʹ��ERR_INVALIDATE_PARAM���
    // ERR_CHARGE_WITHOUT_PERMISSION,  // ��ʹ��ERR_NOT_ENOUGH_PERMISSION���
    // ERR_CHARGE_NOT_DEALER,         // ��ʹ��ERR_NOT_ENOUGH_PERMISSION���
    // ERR_CHARGE_NOT_CHILD,        // ��ʹ��ERR_NOT_ENOUGH_PERMISSION���
    // ERR_CHARGE_INVALIDATE_ORDERID,      // ��ʹ��ERR_DATA_NOT_FOUND���
    ERR_CHARGE_ORDER_STATUS_ERROR,       //����״̬���󣬲���ȡ��
    //ERR_CHARGE_LACK_REMARK,          // ��ʹ��ERR_LACK_PARAM���
    //ERR_CHARGE_PAYMONEY_ERROR,       //��ʹ��ERR_INVALIDATE_PARAM���
    ERR_CHARGE_WIOUT_CHARGECARD,     // �ͻ������޳�ֵ��
    ERR_CHARGE_OPE_DOMAIN_ERROR,      // ������referer ����
    ERR_CHARGE_WRONG_PWD,       // �������(�ۺ�ӿ�)
    ERR_CHARGE_WITHOUT_PWD,     // δ��������(�ۺ�ӿ�)
    ERR_CHARGE_EXIST_LIFELONG_IMIE,      // ��ֵ���������豸
    ERR_CHARGE_CARD_NOT_SUFFICIENT,      // ��ֵ������
    ERR_CHARGE_TRANSFER_PARTIAL_SUCCESS,  // ��ֵ��ת�Ʋ��ֳɹ�
    ERR_CHARGE_WL_FORBIDDEN,            // ���׿���ֹ��ֵ

	//UserGroup Errorcode
	ERR_USERGROUP_BEGIN = 4400,
	ERR_USERGROUP_CREATE_GROUP,
	ERR_USERGROUP_NAME_EXIST,
	ERR_USERGROUP_NOT_EMPTY,
	ERR_USERGROUP_CLEAR_DEFAULT_GROUP,
	ERR_USERGROUP_GET_GROUP_INFO,
	ERR_USERGROUP_GET_GROUP_USER,
	ERR_USERGROUP_GET_USER_INFO,
	ERR_USERGROUP_MODIFY_GROUP_INFO,
	ERR_USERGROUP_MOVE_GROUP_USER,
	ERR_USERGROUP_CLEAR_GROUP_USER,
	ERR_USERGROUP_DELETE_GROUP,

	//RentIncident
	ERR_RENT_BEGIN = 4500,
	ERR_RENT_DEVICENUM_NOT_ENOUGH,
	ERR_RENT_INCIDENTNUM_NOT_ENOUGH,
	ERR_RENT_EXIST,
	ERR_RENT_NOT_EXIST,

    //ShortMessage
	ERR_SHORT_MESSAGE_BEGIN = 4600,
	ERR_SHORT_MESSAGE_NOT_ENOUGH,

	//Account
	ERR_ACCOUNT_BEGIN = 4700,
	ERR_ACCOUNT_NAME_EXIST,
	ERR_ACCOUNT_NOT_EXIST,

	//GLname & GIMEI & GCID
    ERR_GLNAME_NAME_NOT_FOUND = 4800,	//GLNAME�Ҳ�����Ӧ�ĵ�¼��
	ERR_GIMEI_IMEI_NOT_FOUND, 	//GIMEI���Ҳ�����Ӧ��imei��
	ERR_GCID_CID_NOT_FOUND,	//GCID���Ҳ�����Ӧ��CID��Ϣ

	//Search
	ERR_SEARCH_BEGIN = 4900,
	ERR_SEARCH_CACHE_ABNORMAL = 4901,
	ERR_SEARCH_FILE_ABNORMAL = 4902,
	
    //Delete
    ERR_DEL_ERROR = 5000,
    ERR_DEL_SUBEID_EXIST,        // �������¼��ͻ�
    ERR_DEL_SUBDEV_EXIST,        // ���������豸
    ERR_DEL_SUBACCOUNT_EXIST,    // �������˻�
    ERR_DEL_CARD_EXIST,          // �г�ֵ��

    //Location
    ERR_LOCATION_ERROR = 6000,
    ERR_LOCATION_TOO_MANY_DEVICE = 6001,

	//APPPAY
	ERR_APPPAY_ERROR = 6100,
	ERR_APPPAY_MONEY_NOT_ENOUGH = 6101,
	ERR_APPPAY_CARD_NOT_NOUGH,
	ERR_APPPAY_NOT_COMPLETED,
	ERR_APPPAY_BIND_CARD_ERROR,
	ERR_APPPAY_NOT_TOP_DEALER,
	ERR_APPPAY_PLATEFEE_INVALID,
	ERR_APPPAY_FORBID_RENNEW_DEV,
	ERR_APPPAY_RENNEW_FEE_ERROR,
	ERR_APPPAY_RENNEW_FEE_NOT_MATCH,
	
	
    
    //����ϵͳ������
    ERR_BUS_BEGIN = 10000,
    ERR_BUS_EID_NOT_EXIST,
    ERR_BUS_UID_NOT_EXIST,
	ERR_BUS_CITYCODE_NOT_EXIST,
	ERR_BUS_AD_ID_NOT_EXIST,
    ERR_BUS_GET_SHARDID_FAIL,
    ERR_BUS_GET_KDTREE_FAIL,
    ERR_BUS_STATION_NOT_IN_ORDER,
    ERR_BUS_TRACE_IDX_NOT_CORRECT,
    ERR_BUS_TOO_MANY_MASTERLINE_BY_CUSTOMER_ID,
    ERR_BUS_TOO_MANY_SUBLINE_BY_CUSTOMER_ID_AND_DIRECTION,
    ERR_BUS_EMPTY_MASTER_INFO_BEFORE_SET_SUBLINE_INFO,
    ERR_BUS_SUBLINEID_EMPTY_AFTER_INSERT_T_BUS_SUBLINE,
    ERR_BUS_QUERY_CITYCODE_BY_CUSTOMER_ID,
    ERR_BUS_GET_LINE_KD_TREE_DB,
    ERR_BUS_GET_NEAREST_PINT_FROM_KD_TREE,
    ERR_BUS_GET_NEAREST_IDX_POINT_FROM_KD_TREE,
    ERR_BUS_STATION_INFO_VEC_SIZE_NOT_EQUAL,
    ERR_BUS_EXIST_JW_STATION_TOO_SMALL,
	
	
	//����ϵͳ������
	ERR_FINANCE_BEGIN = 11000,
	ERR_CAR_NOT_EXIST,                        // ����������
	ERR_CAR_PLATE_ALREADY_EXIST,              // ���ƺ��ظ�
	ERR_DEVICE_ALREADY_INSTALLED,             // �豸��װ��
	ERR_DEVICE_NOT_INSTALLED,                 // �豸δװ��
	ERR_NOT_BELONG_TO_CUSTOMER,               // �豸���������ڿͻ���������
	ERR_CAR_HAS_DEVICES_INSIDE,               // ���������豸
	ERR_EID_OF_CAR_DEV_NOT_MATCH,             // �豸�ͳ����������ͻ���һ��
	ERR_IDCARD_EXIST,                         // ���֤���ظ�
	
    
	ERR_END
};



inline std::string ToErrString(int errcode)
{
    std::string msg;
    // ע�⣺���¸�ö��ֵ���ֵ�˳�������ö�ٽṹ�ж����˳��һ�£��������
    switch(errcode)
    {
    case ERR_OK:
        msg = "OK";
        break;
        //////////// System error /////////////////
        // ϵͳ�ڲ�������Ҫ�����޸�
    case ERR_SYSTEM_ERROR:
        msg = "system error";
        break;
    case ERR_CALL_GUD_FAIL:
		msg = "call gud error";
        break;
    case ERR_CALL_REDIS_FAIL:
    	msg = "call redis error";
        break;
    case ERR_CALL_MYSQL_FAIL:
		msg = "call mysql error";
        break;
    case ERR_CALL_GTC_FAIL:
		msg = "call GTC error";
        break;
    case ERR_CALL_GMC_FAIL:
		msg = "call GMC error";
        break;
    case ERR_CALL_GFS_FAIL:
		msg = "call GFS error";
        break;
    case ERR_CALL_GMQ_FAIL:
		msg = "call GMQ error";
        break;
    case ERR_CALL_GSE_FAIL:
		msg = "call GSE error";
        break;
    case ERR_CALL_GGB_FAIL:
		msg = "call GGB error";
        break;
    case ERR_CALL_ACC_FAIL:
		msg = "call ACC error";
        break;
    
    //�����ⲿģ�����
    case ERR_CALL_GTREE_FAIL:
		msg = "call Gtree error";
        break;
    case ERR_CALL_GIMEI_FAIL:
		msg = "call GIMEI error";
        break;
    case ERR_CALL_GLNAME_FAIL:
		msg = "call Glname error";
        break;
    case ERR_CALL_GENID_FAIL:
		msg = "call GenId error";
        break;
    case ERR_CALL_GALARM_SERVANT_FAIL:
		msg = "call GAlarm error";
        break;
    case ERR_CALL_TBO_SERVANT_FAIL:
		msg = "call TboServant error";
        break;
    
        //////////// Data error /////////////////
        // ϵͳ�������д���
    case ERR_JSON_FORMAT:
        msg = "parse json data fail.";
        break;

        //////////// User error /////////////////
        // ���û��ύ�Ĳ������Ϲ淶����Ĵ���
    case ERR_INVALIDATE_PARAM:
        msg = "invalidate parameter";
        break;
    case ERR_LACK_PARAM:
        msg = "lack param";
        break;
    case ERR_PARSE_USER_TICKET:
        msg = "parse user ticket fail";
        break;
    case ERR_NOT_YET_LOGIN:
        msg = "please login first";
        break;
    case ERR_OP_RATE_LIMIT:
        msg = "operation too frequent";
        break;
    case ERR_METHOD_NOT_EXIST:
        msg = "call method not exist";
        break;
    case ERR_PASSWORD:
        msg = "password error";
        break;
    case ERR_USER_NOT_EXIST:
        msg = "user not exist,please check";
        break;
    case ERR_DATA_NOT_FOUND:
        msg = "data not found";
        break;
    case ERR_QUERY_NUMBER_LIMIT:
        msg = "over records number limit";
        break;
    case ERR_CHECK_FAIL:
        msg = "check data fail";
        break;
    case ERR_NOT_ENOUGH_PERMISSION:
        msg = "no permission";
        break;
    case ERR_INVALID_OPERATION:
		msg = "invalid operation";
		break;
    case ERR_CHECK_SIGN_FAIL:
		msg = "check sign fail";
		break;
    case ERR_LOGIN_NAME_ALREADY_EXIST:
		msg = "login_name exist";
		break;
    case ERR_RESULT_SET_TOO_LARGE:
		msg = "result is too large";
		break;
	case ERR_WL_CARD_OUT_TIME:
		msg = "wulian card is out of date";
		break;
	case ERR_WL_MSISDN_EMPTY:
		msg = "wulian card msisdn is empty";
		break;
    case ERR_NOT_ENOUGH_HISTORY_PERMISSION:
        msg = "no history permission";
        break;

	//charge error
    case ERR_CHARGE_REPEAT_COMMIT:
		msg = "repeat commit";
		break;
    case ERR_CHARGE_ORDER_STATUS_ERROR:
		msg= "order status error";
		break;
    case ERR_CHARGE_WIOUT_CHARGECARD:
		msg = "without charge card";
		break;
    case ERR_CHARGE_OPE_DOMAIN_ERROR:
		msg = "operation refer error";
		break;
    case ERR_CHARGE_WRONG_PWD:
		msg = "wrong passwd";
		break;
    case ERR_CHARGE_WITHOUT_PWD:
		msg = "without passwd";
		break;
    case ERR_CHARGE_EXIST_LIFELONG_IMIE:
		msg = "exist lifelong IMEI";
		break;
    case ERR_CHARGE_CARD_NOT_SUFFICIENT:
		msg = "charge card not enough";
		break;
    case ERR_CHARGE_TRANSFER_PARTIAL_SUCCESS:
		msg = "transfer partly success";
		break;

	//usergroup error
	case ERR_USERGROUP_CREATE_GROUP:
		msg = "create group failed";
		break;
	case ERR_USERGROUP_NAME_EXIST:
		msg = "usergroup name exist";
		break;
	case ERR_USERGROUP_NOT_EMPTY:
		msg = "usergroup not empty";
		break;
	case ERR_USERGROUP_CLEAR_DEFAULT_GROUP:
		msg = "cannot clear default group";
		break;
	case ERR_USERGROUP_GET_GROUP_INFO:
		msg = "get group info failed";
		break;
	case ERR_USERGROUP_GET_GROUP_USER:
		msg = "get group user failed";
		break;
	case ERR_USERGROUP_GET_USER_INFO:
		msg = "get user info failed";
		break;
	case ERR_USERGROUP_MODIFY_GROUP_INFO:
		msg = "modify group failed";
		break;
	case ERR_USERGROUP_MOVE_GROUP_USER:
		msg = "move group user failed";
		break;
	case ERR_USERGROUP_CLEAR_GROUP_USER:
		msg = "clear group user failed";
		break;
	case ERR_USERGROUP_DELETE_GROUP:
		msg = "delete group failed";
		break;
		
    case ERR_SHORT_MESSAGE_NOT_ENOUGH:
        msg = "please buy short message first";
        break;

	//account error
	case ERR_ACCOUNT_NAME_EXIST:
		msg = "account name exist";
		break;
	case ERR_ACCOUNT_NOT_EXIST:
		msg = "account not exist";
		break;

	//Glname error
	case ERR_GLNAME_NAME_NOT_FOUND:
		msg = "not found the name";
		break;
	case ERR_GIMEI_IMEI_NOT_FOUND:
		msg = "not found the IMEI";
		break;

	//search error
	case ERR_SEARCH_CACHE_ABNORMAL:
		msg = "search cache abnormal";
		break;
	case ERR_SEARCH_FILE_ABNORMAL:
		msg = "search file abnormal";
		break;
	
    //Delete
    case ERR_DEL_SUBEID_EXIST:
		msg = "sub eid exist";
		break;
    case ERR_DEL_SUBDEV_EXIST:
		msg = "sub device exist";
		break;
    case ERR_DEL_CARD_EXIST:
		msg = "card exist";
		break;
		
		
	//Bus error
	case ERR_BUS_CITYCODE_NOT_EXIST:
		msg = "citycode not exist";
		break;
	case ERR_BUS_TOO_MANY_SUBLINE_BY_CUSTOMER_ID_AND_DIRECTION:
        msg = "more than one subline find by on customer id and direction";
        break;
    case ERR_BUS_EMPTY_MASTER_INFO_BEFORE_SET_SUBLINE_INFO:
        msg = "empty master info before set subline info";
        break;
		

	//Finance error
	case ERR_CAR_NOT_EXIST:
		msg = "car not exist";
		break;
	case ERR_CAR_PLATE_ALREADY_EXIST:
        msg = "car plate already exist";
        break;
    case ERR_DEVICE_ALREADY_INSTALLED:
        msg = "device already installed";
        break; 
	case ERR_DEVICE_NOT_INSTALLED:
        msg = "device not installed";
        break; 
	case ERR_NOT_BELONG_TO_CUSTOMER:
        msg = "car or device not belong to customer";
        break; 
	case ERR_CAR_HAS_DEVICES_INSIDE:
		msg = "car has devices on it";
        break;
	case ERR_EID_OF_CAR_DEV_NOT_MATCH:
		msg = "eid of car and eid of device not match";
		break;
			
		
    default:
        msg = "unknow error";
        break;
    }
    return msg;
}



#endif // _GOOCAR_COMMON_ERROR_CODE_H_




