#ifndef __LOGGING_SERVER_H__
#define __LOGGING_SERVER_H__

#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

class Logging_Server {
public:
    // Template Method that runs logging server's event loop.
    virtual int run (int argc, char const *argv[]);
protected:
    // The following four methods are ' 'hooks' ' that can be
    // overridden by subclasses
    virtual int open(u_short port = 0);
    virtual int wait_for_multiple_events() { return 0; }
    virtual int handle_connections() = 0;
    virtual int handle_data(ACE_SOCK_Stream* = 0) = 0;
    // This helper method can be used by the hook methods.
    int make_log_file(ACE_FILE_IO&, ACE_SOCK_Stream* = 0) ;
    // Close the socket endpoint and shutdown ACE.
    virtual ~Logging_Server() { acceptor_.close(); }

    ACE_SOCK_Acceptor &acceptor() { return acceptor_; }
private :
    ACE_SOCK_Acceptor acceptor_;    // Socket acceptor endpoint
};


#endif //__LOGGING_SERVER_H__
