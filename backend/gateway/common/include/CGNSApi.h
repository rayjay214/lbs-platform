#ifndef CGNSAPI_H_HEADER_INCLUDED_AFBFED84
#define CGNSAPI_H_HEADER_INCLUDED_AFBFED84

#include <string>
#include <sys/time.h>

#include "gns_api.h"

//##ModelId=504059BA031C
class CGNSApi
{
  std::string m_sCallerID;
  
  
  struct timezone	 m_tmzTmp;
  struct timeval m_tmvBegin;
  
  public:
    //##ModelId=504059E3010C
    void Init(
        // 主调方的服务名
        const std::string &in_sCallerID)
		{
			m_sCallerID = in_sCallerID;
		}
		;

    //##ModelId=50405A800026
    int GetNodeInfo(const std::string &in_sCalleeID, std::string &o_sIp, int &o_usPort)
	{
		GNS_API::CALLER objCALLER;
		GNS_API::CALLEE objCALLEE;
		
		objCALLER.caller_id = m_sCallerID;
		objCALLEE.callee_id = in_sCalleeID;
		
		int iRetcode_Get = GNS_API::GNS_API_get(objCALLER,objCALLEE);	
		
		if( 0 == iRetcode_Get)
		{
			o_sIp = objCALLEE.ip;
			o_usPort = objCALLEE.port;
			
			gettimeofday (&m_tmvBegin, & m_tmzTmp);
		}
		
		return iRetcode_Get;
	};
    int GetNodeInfos(const std::string &in_sCalleeID, std::string &o_sIp, int &o_usPort,std::vector<GNS_API::CALLEE>& callees)
	{
		GNS_API::CALLER objCALLER;
		GNS_API::CALLEE objCALLEE;
		
		objCALLER.caller_id = m_sCallerID;
		objCALLEE.callee_id = in_sCalleeID;
		
		int iRetcode_Get = GNS_API::GNS_API_gets(objCALLER,objCALLEE,callees);	
		
		if( 0 == iRetcode_Get)
		{
			o_sIp = objCALLEE.ip;
			o_usPort = objCALLEE.port;
			
			gettimeofday (&m_tmvBegin, & m_tmzTmp);
		}
		
		return iRetcode_Get;
	};
	
	

    //##ModelId=50405B9D03AC
	/*如果一次get多次调用，则用次函数，否则不必单独调用*/
    void StartTimer()
	{
		gettimeofday (&m_tmvBegin, & m_tmzTmp);
	};

    //##ModelId=504062F100F5
    int Report(const std::string &in_sCalleeID, const std::string &in_sIp, unsigned short in_usPort, int in_iMethodID, int in_iRetcode)
	{
	
		GNS_API::CALLER objCALLER;
		GNS_API::CALLEE objCALLEE;
		
		GNS_API::REPORT objREPORT;
		
		objCALLER.caller_id = m_sCallerID;
		
		objCALLEE.callee_id = in_sCalleeID;
		objCALLEE.ip = in_sIp;
		objCALLEE.port = in_usPort;
		
		objREPORT.method_id = in_iMethodID;	
		objREPORT.return_code = in_iRetcode;
		objREPORT.beg_time = m_tmvBegin;	
		gettimeofday (&objREPORT.end_time, & m_tmzTmp);
		
		int iRetcode_Report = GNS_API::GNS_API_report(objCALLER,objCALLEE,objREPORT);
	};

    //##ModelId=5040674101A8
    CGNSApi(
        // 主调方的服务名
        const std::string &in_sCallerID)
	{
		Init(in_sCallerID);
	};

    //##ModelId=5040674101C8
    virtual ~CGNSApi(){};

};



#endif /* CGNSAPI_H_HEADER_INCLUDED_AFBFED84 */
