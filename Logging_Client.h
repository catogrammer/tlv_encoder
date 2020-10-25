#include "ace/CDR_Stream.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Record.h"


class Logging_Client {
public:
    int send (const ACE_Log_Record &log_record);    // Send <log_record> to the server.
    ACE_SOCK_Stream &peer() { return logging_peer_; }  // Accessor method.
    ~Logging_Client() { logging_peer_. close( ); }  // Close the connection to the server.

private :
    ACE_SOCK_Stream logging_peer_; // Connected to server.

};

int Logging_Client::send(const ACE_Log_Record &log_record) {
    const size_t max_payload_size = 
        4   // type O
        + 8 // timestump
        + 4 // process id 
        + 4 // data length
        + ACE_Log_Record::MAXLOGMSGLEN // data
        + ACE_CDR::MAX_ALIGNMENT; // padding;

    ACE_OutputCDR payload(max_payload_size) ;

    payload << log_record;
    ACE_CDR::ULong length = payload.total_length();
    ACE_OutputCDR header (ACE_CDR::MAX_ALIGNMENT + 8);
    header << ACE_OutputCDR::from_boolean(ACE_CDR_BYTE_ORDER);
    header << ACE_CDR::ULong(length);
   
    iovec iov [2];
    iov[0].iov_base = header.begin()->rd_ptr();
    iov[0].iov_len = 8;
    iov[1].iov_base = payload.begin()->rd_ptr();
    iov[1].iov_len = length;

    return logging_peer_.sendv_n(iov, 2);
}