//GPS定位漂移处理函数
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

	//扩展算法用，现在暂时用不到
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
        // 函数功能：根据rule中定义的标准，采用不同的方法进行校准
        // 输入参数：org_gps_record_vec为原始定位数据，filtered_gps_record_vec为修正定位数据,rule中包含修正算法相关参数（暂时无用）
        // 返回值：   false返回修正过程中发现定位数据不合法，true返回成功修正定位数据
        //================================================================
        bool CorrectorDispatch(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectBySpeed(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectByLngLat(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);
        bool CorrectByRoute(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule& rule);

        //================================================================
        // 函数功能：采用三次插值算法，对可疑漂移点进行过滤
        // 输入参数：org_gps_record_vec为原始定位数据，filtered_gps_record_vec为修正定位数据,rule中包含修正算法相关参数（暂时无用）
        // 返回值：   false返回修正过程中发现定位数据不合法，true返回成功修正定位数据
        //================================================================
        bool SimpleCorrector(GPS_RECORD_VEC & org_gps_record_vec, GPS_RECORD_VEC & filtered_gps_record_vec, Filter_Rule & rule);

		//================================================================
        // 函数功能：谷米自定义定位数据及飘逸点过滤
        // 输入参数：org_gps_record_vec为原始定位数据，filtered_gps_record_vec为修正定位数据,rule中包含修正算法相关参数（暂时无用）
        /*
        辅助定位过滤:
        	基站 wifi定位数据
        异常点过滤规则
		1.位置过滤
			a) 起始点（与前面间隔超过10分钟）之内的5个点，取前一、当前、后一三个点计算三点之间的距离，如果距离最大值超过最小值的5倍，则舍弃最短边两点之外的点。
			b) 起始点之后的点，用和前一点的距离除以时间间隔计算速度，如果速度值大于200，则舍弃当前点。

		2.速度修正
			a) 起始点（与前面间隔超过10分钟）速度如果大于100，修正速度值为60。
			b) 和前一点时间间隔小于5s，以加速度10计算名义速度，速度大于名义速度时，修正速度值为名义速度。
			c) 和前一点时间间隔大于5s，用和前一点的距离除以时间间隔，计算名义速度，速度大于名义速度时，修正速度值为名义速度。
        */
        // 返回值：   false返回修正过程中发现定位数据不合法，true返回成功修正定位数据
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

