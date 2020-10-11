#ifndef __TLV_LIB_H__
#define __TLV_LIB_H__

#include <string>
#include <cstring>

#define BUFF_SIZE 256

struct tlv_t 
{
    std::uint_fast16_t _tag;
    std::uint_fast16_t _len;
    char _value[BUFF_SIZE];

    tlv_t(const std::string tag, const std::string len, const std::string val)
    {
        _tag = std::stoul(tag, nullptr, 16);
        _len = std::stoul(len, nullptr, 16);
        std::memcpy(&_value, val.c_str(), BUFF_SIZE*sizeof(char));
    }

    tlv_t(std::uint_fast16_t t, std::uint_fast16_t l, const std::string val)
    {
        _tag = t;
        _len = l;
        std::memcpy(&_value, val.c_str(), BUFF_SIZE*sizeof(char));
    }
};


void tlv_encode(const tlv_t& message, void* buf);
void tlv_decode(tlv_t& message, const void* buf);

#endif //__TLV_LIB_H__