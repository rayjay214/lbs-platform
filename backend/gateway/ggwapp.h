#ifndef _GWAPP_H_
#define _GWAPP_H_
/*
 * ggwapp.h
 *
 */

#include <Ice/Service.h>
#include "GWReaderThread.h"
#include "DispatchThread.h"
#include "ReceiveThread.h"
#include "GAsyncCurlThread.h"


class GGWApp : public Ice::Service
{
public:
static GGWApp * Instance();

	GGWApp() 
	{
	};
	~GGWApp() {};

	virtual void interrupt();
	Ice::CommunicatorPtr GetCommunicatorPtr();
	
protected:
	virtual bool start(int argc, char* argv[], int& status);

private:
	Ice::ObjectAdapterPtr m_pAdapter;
	Ice::CommunicatorPtr m_ic;

	std::vector<int>	_listerfds;
	GWReaderPtr _readerThread;
	CDispatchThreadPtr _dispatchThread;
	CReceiveThreadPtr _receiveThread;
    GAsyncCurlThreadPtr _asyncCurlThread;
};
#endif  
