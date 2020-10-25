#include "Reactive_Logging_Server_Ex.h"


int main(int argc, char const *argv[])
{
    Reactive_Logging_Server_Ex server;
    if (server.run(argc, argv) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", " server.run()"), 1);

    return 0 ;
}