/*
	GNS API RAII Library Code
*/


#ifndef GNS_API_RAII_H
#define GNS_API_RAII_H

#include "gns_api.h"

namespace GNS_API
{

class GNS_API_RAII
{
public:
	
	GNS_API_RAII(const CALLER& caller, const CALLEE& callee, const int method_id):m_caller(caller), m_callee(callee)
	{
		gettimeofday(&m_report.beg_time, NULL);
		m_report.method_id = method_id;
		m_report.return_code = 0;
	}

	~GNS_API_RAII()
	{
		if((m_caller.caller_id != "") && (m_callee.callee_id != ""))
		{
			gettimeofday(&m_report.end_time, NULL);
			GNS_API_report(m_caller, m_callee, m_report);
		}
	}

	void set_return_code(int ret)
	{
		m_report.return_code = ret;
	}

private:
	
	CALLER m_caller;
	CALLEE m_callee;
	REPORT m_report;
};


}
#endif

