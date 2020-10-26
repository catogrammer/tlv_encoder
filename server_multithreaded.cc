// #include "Reactive_Logging_Server_Ex.h"
#include "ace/Signal.h"
#include "ace/Thread_Manager.h"

#include "Thread_Per_Connection_Logging_Server.h"

static void sigterm_handler(int /* signum */) { /* No-op. */ }

int main(int argc, char const *argv[])
{
    // Register to receive the <SIGTERM> signal.
    ACE_Sig_Action sa(sigterm_handler, SIGTERM);
    Thread_Per_Connection_Logging_Server server;
    
    if (server.run(argc, argv) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "server.run()"), 1);
    
    // Cooperative thread cancelation.
    ACE_Thread_Manager::instance()->cancel_all();
    // Barrier synchronization, wait no more than a minute.

    ACE_Time_Value timeout(60);
    return ACE_Thread_Manager::instance()->wait(/*&timeout*/);
}