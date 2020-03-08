#ifndef CGNSAUTOREPORTER_H_HEADER_INCLUDED_AFBFFC4E
#define CGNSAUTOREPORTER_H_HEADER_INCLUDED_AFBFFC4E


#include "CGNSApi.h"


//##ModelId=5040675A01C4
class CGNSAutoReporter
{
  public:
    //##ModelId=5040677C012A
    CGNSAutoReporter(CGNSApi *in_pobjCGNSApi, const std::string *in_psCalleeID,const std::string *in_psIp, const int * in_pusPort, const int *in_piMethodID, const int *in_piRetcode)
		:m_pCGNSApi(in_pobjCGNSApi),m_psCalleeID(in_psCalleeID),m_piMethodID(in_piMethodID),m_piRetcode(in_piRetcode),m_psIp(in_psIp),m_pusPort(in_pusPort)
	{
		if((NULL == m_pCGNSApi) || (NULL == m_psCalleeID) || (NULL == m_piMethodID) || (NULL == m_piRetcode) || (NULL == m_psIp) || (NULL == m_pusPort) )
			return;
		m_pCGNSApi->StartTimer();
		
	};

    //##ModelId=5040677C014A
    virtual ~CGNSAutoReporter()
	{
		if((NULL == m_pCGNSApi) || (NULL == m_psCalleeID) || (NULL == m_piMethodID) || (NULL == m_piRetcode) || (NULL == m_psIp) || (NULL == m_pusPort) )
			return;
		
		m_pCGNSApi->Report(*m_psCalleeID,*m_psIp,*m_pusPort,*m_piMethodID,*m_piRetcode);
	};

  private:
    //##ModelId=504067890304
    CGNSApi *m_pCGNSApi;

    //##ModelId=504067AB0379
    const int *m_piRetcode;

    //##ModelId=504067BC02A8
    const std::string *m_psCalleeID;

    //##ModelId=504067EE001B
    const int *m_piMethodID;
	
	const std::string *m_psIp;
	
	const int * m_pusPort;

};



#endif /* CGNSAUTOREPORTER_H_HEADER_INCLUDED_AFBFFC4E */
