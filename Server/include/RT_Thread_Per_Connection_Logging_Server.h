#ifndef __RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H__
#define __RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H__

#include "ace/Sched_Params.h"
#include "Thread_Per_Connection_Logging_Server.h"

class RT_Thread_Per_Connection_Logging_Server :
	public Thread_Per_Connection_Logging_Server
{
protected:
	virtual int open(u_short port);
	virtual int handle_data(ACE_SOCK_Stream * = 0);
};

#endif // _RT_THREAD_PER_CONNECTION_LOGGING_SERVER_H
