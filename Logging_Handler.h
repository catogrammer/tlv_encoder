#include <iostream>
#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Log_Record.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"

class Logging_Handler
{
protected:
    ACE_FILE_IO &log_file_;         // Reference to a log file.
    ACE_SOCK_Stream logging_peer_;  // Connected to the client.
public:
    // Initialization and termination methods.
    Logging_Handler(ACE_FILE_IO &log_file) : log_file_(log_file) {}
    Logging_Handler(ACE_FILE_IO &log_file, ACE_HANDLE handle) : log_file_(log_file)
        { logging_peer_.set_handle(handle); }
    Logging_Handler(ACE_FILE_IO &log_file, const ACE_SOCK_Stream &logging_peer)
    : log_file_(log_file), logging_peer_(logging_peer) {}
   
    int close() { return logging_peer_.close(); }
    // Receive one log record from a connected client. Returns
    // length of record on success and <mblk> contains theACE_FILE_IO 
    // hostname, <mblk->cont()> contains the log record header
    // (the byte order and the length) and the data. Returns -1 on
    // failure or connection close.
    int recv_log_record(ACE_Message_Block *&mblk);
    // Write one record to the log file. The <mblk> contains the
    // hostname and the <mblk->cont> contains the log record.
    // Returns length of record written on success; -1 on failure,
    int write_log_record(ACE_Message_Block *mblk);
    // Log one record by calling <recv_log_record> and
    // <write_log_record>. Returns 0 on success and -1 on failure.
    int log_record();
    ACE_SOCK_Stream& peer() { return logging_peer_; } // Accessor.
};


int Logging_Handler::recv_log_record(ACE_Message_Block *&mblk)
{
    ACE_INET_Addr peer_addr;
    logging_peer_.get_remote_addr(peer_addr);
    mblk = new ACE_Message_Block(MAXHOSTNAMELEN + 1);
    peer_addr.get_host_name(mblk->wr_ptr(), MAXHOSTNAMELEN);
    mblk->wr_ptr(strlen(mblk->wr_ptr()) + 1); // Go past name.
    
    ACE_Message_Block *payload = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE);
    ACE_CDR::mb_align(payload);     // Align Message Block for a CDR stream.

    if (logging_peer_.recv_n(payload->wr_ptr(), 8) == 8) {
        payload->wr_ptr(8);

        ACE_InputCDR cdr(payload);  // Reflect addition of 8 bytes.
        ACE_CDR::Boolean byte_order;
        // Use helper method to disambiguate booleans from chars.
        cdr >> ACE_InputCDR::to_boolean(byte_order);
        cdr.reset_byte_order(byte_order);
        ACE_CDR::ULong length;
        cdr >> length;
        payload->size(length + 8);

        if (logging_peer_.recv_n(payload->wr_ptr(), length) > 0) {
            payload->wr_ptr(length);
            // Reflect additional bytes.
            mblk->cont(payload);
            return length; // Return length of the log record.
        }
    }

    payload->release();
    mblk->release();
    payload = mblk = 0;
    return -1;
}

int Logging_Handler::write_log_record(ACE_Message_Block *mblk)
{
    if (log_file_.send_n(mblk) == -1)
        return -1;

    if (ACE::debug())
    {
        ACE_InputCDR cdr(mblk->cont());
        ACE_CDR::Boolean byte_order;
        ACE_CDR::ULong length;
        cdr >> ACE_InputCDR::to_boolean(byte_order);
        cdr.reset_byte_order (byte_order);
        cdr >> length;
        ACE_Log_Record log_record;
        cdr >> log_record; // Extract the <ACE_log_record>.
        log_record.print(mblk->rd_ptr(), 1, std::cerr);
    }
    return mblk->total_length();
}

int Logging_Handler::log_record()
{
    ACE_Message_Block *mblk = 0;

    if (recv_log_record(mblk) == -1)
        return -1;
    else {
        int result = write_log_record(mblk);
        mblk->release();    // Free up the entire, contents.

        return result == -1 ? -1 : 0;
    }
}