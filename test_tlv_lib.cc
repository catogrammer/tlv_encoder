#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "tlv_lib/tlv_lib.h"

#include <bitset>



typedef std::uint_fast16_t tl_t; // tag_lenght_type

void print_tlv_struct(const tlv_t& tlv)
{
    std::cout << std::hex << tlv._tag << std::endl;
    std::cout << std::hex << tlv._len << std::endl;
    std::cout << tlv._value << std::endl;
}

// make as lambda and think about types !!

bool octet_overflow(tl_t tag) // make as lambda
{
    if ((tag & 0x7) > 30)
        return true;
    return false;
}

bool tag_is_complex(tl_t tag)
{
    return (tag & 0x20);
}

int bit_extracted(tl_t number, size_t k, size_t p)  //chaeck this func
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
}

// 0 - is lower bit
int bit_is_set(tl_t num, size_t pose)
{
    return ((num >> pose)  & 0x01);
}

bool len_is_long(tl_t len) // get higher bit
{
   return (len & 0x80);
}

int get_cnt_byte_of_len(tl_t len) // make as lambda // get second  bit 1-7
{
    return (len & 0x7F);
}

// i - 
tl_t merge_bytes(tl_t high_b, tl_t low_b, size_t i)
{
    return ((high_b << 8*i) | low_b);
}

void print_str_as_hex(std::basic_string<char16_t> in)
{
    for (auto &&i : in)
    {
        std::cout << std::hex << (int)i << ' ';
    }
    std::cout << std::endl;
}

void func_decode(std::basic_string<char16_t> data)
{
    size_t pos = 0;
    size_t offset = 2;
    tl_t tag = 0;
    std::uint_fast64_t len = 0;
    std::basic_string<char16_t> value;

    tag = data[pos];
    if (octet_overflow(tag))
    {
        std::cout << "Curr tag : " << std::hex << data[pos] << std::endl;
        std::cout << " Tag isn't simple " << std::endl;
        tag = ((data[pos] << 8) | data[pos+1]); // higher | lover
        pos++;
        offset +=1;
    }
    len = data[++pos];
    if (len_is_long(len))
    {
        std::cout << "Curr len : " << std::hex << len << std::endl;
        std::cout << " Len isn't short " << std::endl;
        int cnt_bytes = get_cnt_byte_of_len(len);

        if (cnt_bytes > 8)
            std::cout << "Too much lenth octets!" << std::endl;

        len = 0;
        for (int i = cnt_bytes; i < 0; i--)
        {
            len = merge_bytes(len, data[++pos], i);
        }
        offset += cnt_bytes;
    }

    value = data.substr(pos, len);

    std::cout << /*std::hex << */"tag : " << tag 
              << " len : "  << len << " == value len : "
              << value.length() << std::endl;

    std::basic_string<char16_t> right, left;

    right = data.substr(len + offset, data.length() -(len + offset));
    left = data.substr(offset, len);

    // std::cout << "Right : " << right << std::endl;
    // std::cout << "Left : " << left << std::endl;
    std::cout << "Right : ";
    print_str_as_hex(right);
    std::cout << "Left : ";
    print_str_as_hex(left);

    if (left.length() > 1 && tag_is_complex(tag))
        func_decode(left);
    
    if (!right.length() > 1 && tag_is_complex(tag))
        func_decode(right);
}

std::basic_string<char16_t> str_to_hex_arr(std::string in)
{
    unsigned char* buff = new unsigned char[int(in.length()/2)];
    std::basic_string<char16_t> out;
    for (int i = 0; i < in.length(); i+=2)
    {
        // std::cout << in.substr(i, 2) << " = " << std::stoul(in.substr(i, 2), nullptr, 16) << " | ";
        out += std::stoul(in.substr(i, 2), nullptr, 16);
    }
    std::cout << std::endl;
    return out;
}

int main(int argc, char const *argv[])
{
    std::string str = "6F2F840E325041592E5359532E4444463031A51DBF0C1A61184F07A0000000031010500A564953412044454249548701019000";
    std::string str2 = "6f2f840e325041592e5359532e4444463031a51dbf0c1a61184f07a0000000031010500a564953412044454249548701019000";
    auto hex_str = str_to_hex_arr(str2);
    print_str_as_hex(hex_str);


    std::cout << "Start encode" << std::endl;
    func_decode(hex_str);

    // std::vector<tlv_t> tlv_box;
    // func_decode(str, tlv_box);
    // for (auto &&i : tlv_box)
    // {
    //     print_tlv_struct(i);
    // }
    

    return 0;
}
