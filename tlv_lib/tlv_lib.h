#ifndef __TLV_LIB_H__
#define __TLV_LIB_H__

#include <string>
#include <cstring>

#define BUFF_SIZE 256

struct tlv_t 
{
    std::uint_fast16_t _tag;
    std::uint_fast16_t _len;
    std::basic_string<char16_t> _value;

    tlv_t(std::uint_fast16_t t, std::uint_fast16_t l, 
          const std::basic_string<char16_t>& val)
    {
        _tag = t;
        _len = l;
        _value = val;
        // std::memcpy(&_value, val.c_str(), val.length()*sizeof(char16_t));
    }
};


void tlv_encode(const tlv_t& message, void* buf);
void tlv_decode(tlv_t& message, const void* buf);

#endif //__TLV_LIB_H__