#include "ace/Auto_Ptr.h"
#include "ace/FILE_IO.h"
#include "ace/Log_Msg.h"
#include "ace/Sched_Params.h"
#include "ace/Signal.h"
#include "ace/Thread_Manager.h"

#include "RT_Thread_Per_Connection_Logging_Server.h"

int RT_Thread_Per_Connection_Logging_Server::open(u_short port)
{
	ACE_Sched_Params fifo_sched_params(ACE_SCHED_FIFO,
		ACE_Sched_Params::priority_min(ACE_SCHED_FIFO),
		ACE_SCOPE_PROCESS);

	if (ACE_OS::sched_params (fifo_sched_params) != 0) {
		if (errno == EPERM || errno == ENOTSUP)
			ACE_DEBUG((LM_DEBUG, "Warning: user's not superuser, so "
					   "we're running in time-shared class\n"));
		else
			ACE_ERROR_RETURN((LM_ERROR, "%p\n", "ACE_OS::sched_params()"), -1);
	}
  // Initialize the parent classes.
  return Thread_Per_Connection_Logging_Server::open(port);
}


int RT_Thread_Per_Connection_Logging_Server::handle_data(ACE_SOCK_Stream *client)
{
	int prio = ACE_Sched_Params::next_priority(ACE_SCHED_FIFO,
		ACE_Sched_Params::priority_min(ACE_SCHED_FIFO),
		ACE_SCOPE_THREAD);
	ACE_OS::thr_setprio(prio);

  return Thread_Per_Connection_Logging_Server::handle_data (client);
}