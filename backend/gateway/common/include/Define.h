/********************************************************************
	created:	2013/09/11
	created:	11:9:2013   15:03
	filename: 	Define.h
	author:		LiuHeMing
	
	purpose:	��������
*********************************************************************/
#ifndef Define_H__
#define Define_H__
#include <stdlib.h>
#include <string>

const char CONST_CONFIG[128] = "../etc/config.ini";
const char SQL_INSERT_STDDATA_BAIDU[128] = "insert into t_data_std_baidu values(%3.2f,%3.2f,%3.12f,%3.12f)";
const char SQL_INSERT_GEODATA_BAIDU[128] = "insert into t_data_geo_baidu values('%s','%s',%3.12f,%3.12f)";

const char SQL_INSERT_STDDATA_GOOGLE[128] = "insert into t_data_std_google values(%3.2f,%3.2f,%3.12f,%3.12f)";
const char SQL_INSERT_GEODATA_GOOGLE[128] = "insert into t_data_geo_google values('%s','%s',%3.12f,%3.12f)";

const char SQL_SELECT_STDDATA_BAIDU[128] = "select dx,dy from t_data_std_baidu where x=%3.2f and y=%3.2f;";
const char SQL_SELECT_STDDATA_GOOGLE[128] = "select dx,dy from t_data_std_google where x=%3.2f and y=%3.2f;";

const int    CONST_GEOCHAR_LEN = 20;
const int	 CONST_MAXLEN  = 255;
const int    CONST_X_MAX = 6600;//7200--13800
const int	 CONST_Y_MAX =4500 ;//1000--5499
const int	CONST_X_OFFSET = 7200;
const int	CONST_Y_OFFSET = 1000;
const int	CONST_RATE = 100;		//���100����תΪ����
const double	CONST_FRATE = 100.0f;		//���100����תΪ����
const double	CONST_XRATE = 1000.0f;		//����10�õ�����ֵ
const int	CONST_MNG_NUM = 1;
const int	MAX_MEMORY_MNG_NUM = 1;
const int	CONST_SELECT_DATA_LEN = 2;
const char  CONST_BAIDU_MEM_FILENAME[128] = "/home/common/common/dataload/bin/baidu_stddata0.01.shm";
const char  CONST_GOOGLE_MEM_FILENAME[128] = "/home/common/common/dataload/bin/google_stddata0.01.shm";
const unsigned char CONST_UCKEY = 'M';
const char  CONST_MEM_PATH[128] = "/home/common/common/dataload/bin/";

#define IPCKEYBAIDU		0x4d020170
#define IPCKEYGOOGLE	0x4d020171

enum	E_ID_DATAMNG
{
	E_ID_DATA_GOOGLE =0,
	E_ID_DATA_BAIDU = 1,
};

enum E_MAPTYPE
{
	E_MAPTYPE_GOOGLE = 0,
	E_MAPTYPE_BAIDU = 1,
};

typedef struct ST_CONF 
{
	std::string m_strIP;
	int    m_nPort;
	std::string m_strUserName;
	std::string m_strPassWord;
	std::string m_strDBName;

	std::string m_strDataFileBaidu;
	std::string m_strDataFileGoogle;
	int	m_nGeoMod;			//geohash����ľ���
	int	m_nLoadStdData;			//���ر�׼���ݿ��أ�0�أ���0��
	int	m_nLoadGeoData;			//����geo���ݿ��أ�0�أ���0��

	std::string m_strShmFilePath;		//�����ڴ��Ŀ���ļ���
	std::string m_strShmFileBaidu;		//�����ڴ��BaiduĿ���ļ�
	std::string m_strShmFileGoogle;		//�����ڴ��GoogleĿ���ļ�
}
T_CONF;

typedef struct ST_DATA
{
	double x;
	double y;
	double datax;
	double datay;
}
T_DATA;

typedef struct ST_DPOS
{
	double dx;
	double dy;
}T_DPOS;

typedef struct ST_Data
{
	ST_DPOS m_szData[CONST_X_MAX][CONST_Y_MAX];
}T_Data;
#endif // Define_H__