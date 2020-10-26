#include "ace/ACE.h"
#include "ace/Handle_Set.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"

#include "Thread_Per_Connection_Logging_Server.h"
#include "Logging_Handler.h"

int Thread_Per_Connection_Logging_Server::handle_connections()
{
    Thread_Args *thread_args = new Thread_Args(this);
    if (acceptor().accept(thread_args->logging_peer_) == -1)
        return -1;
    
    if (ACE_Thread_Manager::instance()->spawn( run_svc, // Pointer-to-function entry point.
            static_cast<void*>(thread_args), THR_DETACHED | THR_SCOPE_SYSTEM)
         == -1)
        return -1;

    // delete thread_args;
    
    return 0;
}

void* Thread_Per_Connection_Logging_Server::run_svc (void *arg)
{
    Thread_Args *thread_args = static_cast<Thread_Args*>(arg) ;
    thread_args->this_->handle_data(&thread_args->logging_peer_);
    thread_args->logging_peer_.close();

    return 0; // Return value is ignored.
}

int Thread_Per_Connection_Logging_Server::handle_data(ACE_SOCK_Stream *logging_peer)
{
    ACE_FILE_IO log_file;
    make_log_file (log_file, logging_peer);  // Client's hostname is used as the logfile name.
    
    logging_peer->disable(ACE_NONBLOCK); // Place the connection into blocking mode.
    Logging_Handler logging_handler(log_file, *logging_peer);
    ACE_Thread_Manager *mgr = ACE_Thread_Manager::instance();
    ACE_thread_t me = ACE_OS::thr_self();

    // Keep handling log records until client closes connection
    // or this thread is asked to cancel itself.
    while (!mgr->testcancel(me) && logging_handler.log_record() != -1)
        continue;

    log_file.close();

    return 0;
}

int Thread_Per_Connection_Logging_Server::run(int argc, char const *argv[]) {
    if (open(argc > 1 ? atoi(argv[1]) : 0) == -1)
        return -1;

    for (;;) {
        if (wait_for_multiple_events() == -1)
            return -1;
        if (handle_connections() == -1)
            return -1;
    }

    ACE_NOTREACHED(return 0;)
}