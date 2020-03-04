//GPS��λƯ�ƴ�����
#ifndef _DRIFT_FILTER_H_
#define _DRIFT_FILTER_H_

#include <time.h>
#include <vector>
#include <stdint.h>
#include <string>


namespace DriftCorrector
{

    struct GpsRecord
    {
    public:
        uint64_t uid;
        uint64_t reportTime;
        uint64_t sysTime;
        uint32_t longitude;
        uint32_t latitude;
        int speed;
        uint32_t direction;
        uint32_t sequenceNo;
        uint8_t protocolNo[4];
        union
        {
            uint8_t status[16];
            uint8_t driftFlg;
        } u;
    } __attribute__((packed));

	//��չ�㷨�ã�������ʱ�ò���
    typedef struct
    {
        std::string criterion;
        uint32_t    maxReportInterval;
        int         countConstraint1;
        int         countConstraint2;
        uint32_t    ctConstraint1;
        uint32_t    ctConstraint2;
        uint32_t    ctConstraint3;
    } Filter_Rule;

    typedef std::vector<GpsRecord> GPS_RECORD_VEC;

	class DupPointFilter
	{
	public:
		DupPointFilter(const GpsRecord &basePoint, const GpsRecord &checkPoint):
			m_basePoint(basePoint), m_checkPoint(checkPoint), m_bDrift(false)
		{
		}
		virtual ~DupPointFilter()
		{
		}
		void checkDrift(int maxSpeed);
		bool isDrift()
		{
			return m_bDrift;
		}

		int getCalcSpeed()
		{
			return m_nCalcSpeed;
		}

		std::string getDriftInfo()
		{
			return m_strDriftInfo;
		}
	private:
		const GpsRecord &m_basePoint;
		const GpsRecord &m_checkPoint;
		bool m_bDrift;
		int m_nCalcSpeed;

		std::string m_strDriftInfo;
	};

	class TriPointFilter
	{
	public:
		TriPointFilter(const GpsRecord &basePoint, const GpsRecord &checkPoint, const GpsRecord &assistPoint):
			m_basePoint(basePoint), m_checkPoint(checkPoint), m_assistPoint(assistPoint), m_bDrift(false)
		{
		}
		virtual ~TriPointFilter()
		{
		}
		void checkDrift(int maxSpeed);
		bool isDrift()
		{
			return m_bDrift;
		}

		int getCalcSpeed()
		{
			return m_nCalcSpeed;
		}

		std::string getDriftInfo()
		{
			return m_strDriftInfo;
		}
	private:
		const GpsRecord &m_basePoint;
		const GpsRecord &m_checkPoint;
		const GpsRecord &m_assistPoint;
		bool m_bDrift;
		int m_nCalcSpeed;

		std::string m_strDriftInfo;
	};

    class CDriftCorrector
    {
    public:
        CDriftCorrector();
        ~CDriftCorrector();

        //================================================================
        // �������ܣ�����rule�ж���ı�׼�����ò�ͬ�ķ�������У׼
        // ���������org_gps_record_vecΪԭʼ��λ���ݣ�filtered_gps_record_vecΪ������λ����,rule�а��������㷨��ز�������ʱ���ã�
        // ����ֵ��   false�������������з��ֶ�λ���ݲ��Ϸ���true���سɹ�������λ����
        //================================================================
        bool CorrectorDispatch(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectBySpeed(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectByLngLat(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectByRoute(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);

        //================================================================
        // �������ܣ��������β�ֵ�㷨���Կ���Ư�Ƶ���й���
        // ���������org_gps_record_vecΪԭʼ��λ���ݣ�filtered_gps_record_vecΪ������λ����,rule�а��������㷨��ز�������ʱ���ã�
        // ����ֵ��   false�������������з��ֶ�λ���ݲ��Ϸ���true���سɹ�������λ����
        //================================================================
        bool SimpleCorrector(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule & rule);

		//================================================================
        // �������ܣ������Զ��嶨λ���ݼ�Ʈ�ݵ����
        // ���������org_gps_record_vecΪԭʼ��λ���ݣ�filtered_gps_record_vecΪ������λ����,rule�а��������㷨��ز�������ʱ���ã�
        /*
        ������λ����:
        	��վ wifi��λ����
        �쳣����˹���
		1.λ�ù���
			a) ��ʼ�㣨��ǰ��������10���ӣ�֮�ڵ�5���㣬ȡǰһ����ǰ����һ�������������֮��ľ��룬����������ֵ������Сֵ��5������������̱�����֮��ĵ㡣
			b) ��ʼ��֮��ĵ㣬�ú�ǰһ��ľ������ʱ���������ٶȣ�����ٶ�ֵ����200����������ǰ�㡣

		2.�ٶ�����
			a) ��ʼ�㣨��ǰ��������10���ӣ��ٶ��������100�������ٶ�ֵΪ60��
			b) ��ǰһ��ʱ����С��5s���Լ��ٶ�10���������ٶȣ��ٶȴ��������ٶ�ʱ�������ٶ�ֵΪ�����ٶȡ�
			c) ��ǰһ��ʱ��������5s���ú�ǰһ��ľ������ʱ���������������ٶȣ��ٶȴ��������ٶ�ʱ�������ٶ�ֵΪ�����ٶȡ�
        */
        // ����ֵ��   false�������������з��ֶ�λ���ݲ��Ϸ���true���سɹ�������λ����
        //================================================================
		bool CorrectByGoome(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule & rule);

		void getDebugMsg(std::vector<std::string> &vecDebugMsg)
		{
			vecDebugMsg.swap(m_vecDebugMsg);
			return;
		}

	private:
		std::string DebugString(const std::string &msg, const GpsRecord &gps, const std::string &str="");
		
	private:
		std::vector<std::string> m_vecDebugMsg;
    };
}

#endif  //_DRIFT_FILTER_H_

