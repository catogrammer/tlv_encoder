#include <cstring>
#include <cstdint>
#include "tlv_lib.h"

void tlv_encode(const tlv_t& message, void* buf)  //void* ?
{
    std::memcpy(buf, &(message.tag), sizeof(std::uint_fast8_t));
    std::memcpy(buf + sizeof(std::uint_fast8_t), &(message.length), 
                sizeof(std::uint_fast16_t));
    std::memcpy(buf + sizeof(std::uint_fast8_t) + sizeof(std::uint_fast16_t),
                &(message.value), message.length);
}

void tlv_decode(tlv_t& message, const void* buf)
{
   std::memcpy(&(message.tag), buf, sizeof(message.tag));
   std::memcpy(&(message.length), buf + sizeof(message.tag),
               sizeof(message.length));
   std::memcpy(&(message.length),
               buf + sizeof(message.tag) + sizeof(message.length),
               message.length);
}