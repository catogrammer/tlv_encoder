#ifndef __LOGGING_HANDLER__
#define __LOGGING_HANDLER__

#include <iostream>
#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"
#include "ace/Log_Record.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"
#include <string>
#include <utility>
#include <vector>
#include "tlv_lib.h"

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

std::u16string str_to_hex_arr(std::string in)
{
    unsigned char* buff = new unsigned char[int(in.length()/2)];
    std::u16string out;
    if (in.length()%2 == 0)
        for (int i = 0; i <= (in.length() - 2); i+=2)
        {
            try
            {
                out += std::stoul(in.substr(i, 2), nullptr, 16);
            }
            catch(const std::exception& e)
            {
                out.clear();
                std::cerr << e.what() << '\n';
            }  
        }
    std::cout << std::endl;
    return out;
}

int Logging_Handler::recv_log_record(ACE_Message_Block *&mblk)
{
    iovec response;
    const char* buff;
    int len;

    if (logging_peer_.recvv(&response) == -1)
        return -1;

    buff = static_cast<char*>(response.iov_base);
    len = response.iov_len;
    
    if (len == 0 || buff == 0x0)
        return -1;

    std::string input(buff, len);
    // ACE_DEBUG((LM_DEBUG, ACE_TEXT("buff :%s\nbuff len: %d\n"), input.c_str(), len));

    std::u16string hex_str;
    std::vector<std::pair<size_t, tlv_t>> tlv_box;

    hex_str = str_to_hex_arr(input);

    if (hex_str.empty())
        return -1;

    tlvs_decode(hex_str, tlv_box, 0);
    print_tlv_box(tlv_box);

    delete[] buff;

    return 0;
}

int Logging_Handler::write_log_record(ACE_Message_Block *mblk)
{
    if (log_file_.send_n(mblk) == -1)
        return -1;

    return mblk->total_length();
}

int Logging_Handler::log_record()
{
    ACE_Message_Block *mblk = 0;

    if (recv_log_record(mblk) == -1)
        return -1;
    else 
        return 0;
}

#endif // __LOGGING_HANDLER__