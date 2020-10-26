#ifndef __THREAD_PER_CONNECTION_LOGGING_SERVER_H__
#define __THREAD_PER_CONNECTION_LOGGING_SERVER_H__

#include "Logging_Server.h"

class Thread_Per_Connection_Logging_Server : public Logging_Server
{
private:
    struct Thread_Args
    {
        Thread_Per_Connection_Logging_Server *this_;
        ACE_SOCK_Stream logging_peer_;

        Thread_Args(Thread_Per_Connection_Logging_Server *lsp) : this_(lsp) {}
    };
    // Passed as a parameter to <ACE_Thread_Manager::spawn>.
    static void *run_svc(void *arg);
    
protected:
    virtual int handle_connections();
    virtual int handle_data(ACE_SOCK_Stream *logging_peer);

public:
    virtual int run(int argc, char const *argv[]);
};

#endif //__THREAD_PER_CONNECTION_LOGGING_SERVER_H__