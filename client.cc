#include "ace/OS.h"
#include "ace/CDR_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Record.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"
#include "ace/streams.h"
#include "Logging_Client.h"
#include <string>


int main(int argc, char const *argv[])
{
    u_short logger_port = argc > 1 ? atoi (argv[1]) : 0;
    const char *logger_host = argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;
    int result;
    ACE_INET_Addr server_addr;

    if (logger_port != 0)
        result = server_addr.set (logger_port, logger_host);
    else
        result = server_addr.set ("ace_logger", logger_host);

    if (result == -1)
        ACE_ERROR_RETURN((LM_ERROR, "lookup %s, %p\n", 
                          logger_port == 0 ? "ace_logger" : argv[1],
                          logger_host),
                         1);
    
    ACE_SOCK_Connector connector;
    Logging_Client logging_client;

    if (connector.connect(logging_client.peer(), server_addr) < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect()"), 1); // Limit the number of characters read on each record.
    
    cin.width(ACE_Log_Record::MAXLOGMSGLEN);

    for (;;) {
        std::string user_input;
        getline (cin, user_input, '\n');
        if (!cin | cin.eof()) 
            break;

        ACE_Time_Value now(ACE_OS::gettimeofday());
        ACE_Log_Record log_record(LM_INFO, now, ACE_OS::getpid());
        log_record.msg_data(user_input.c_str());

        if (logging_client.send(log_record) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "logging client.send()"), 1);
    }

    return 0; // Logging_Client destructor closes TCP connection.
}




