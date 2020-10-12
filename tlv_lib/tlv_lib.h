#ifndef __TLV_LIB_H__
#define __TLV_LIB_H__

#include <string>
#include <cstring>

#define BUFF_SIZE 256

struct tlv_t 
{
    char16_t _tag;
    char16_t _len;
    std::u16string _value;

    tlv_t(char16_t t, char16_t l, const std::u16string& val)
    {
        _tag = t;
        _len = l;
        _value = val;
    }
};


void tlv_encode(const tlv_t& message, void* buf);
void tlv_decode(tlv_t& message, const void* buf);

#endif //__TLV_LIB_H__