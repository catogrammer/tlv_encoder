#include <iostream>
#include <string>
#include <utility>
// #include <cstring>
#include <vector>
#include "tlv_lib/tlv_lib.h"

#include <bitset>

typedef std::uint_fast16_t tl_t; // tag_lenght_type


void print_str_as_hex(std::basic_string<char16_t> in)
{
    for (auto &&i : in)
    {
        std::cout << std::hex << (int)i << ' ';
    }
    std::cout << std::endl;
}

void print_tlv_struct(const tlv_t& tlv)
{
    std::cout << "Tag : " << std::hex << tlv._tag << std::endl;
    std::cout << "Len : " << std::hex << tlv._len << std::endl;
    std::cout << "Value : "; print_str_as_hex(tlv._value);
}

void print_tlv_struct(const tlv_t& tlv, std::string offset)
{
    std::cout << offset  << "Tag : " << std::hex << tlv._tag << std::endl;
    std::cout << offset  << "Len : " << std::hex << tlv._len << std::endl;
    std::cout <<  offset << "Value : "; print_str_as_hex(tlv._value);
}

void print_tlv_box(const std::vector<std::pair<size_t, tlv_t>>& box)
{
    for (auto &&i : box)
    {
        std::string offset = "";
        for (size_t j = 0; j < i.first - 1; j++)
            offset += "    ";
        print_tlv_struct(i.second, offset);
    }
}
// make as lambda and think about types !!

bool octet_overflow(tl_t tag) // make as lambda
{
    return (tag & 0x1F) > 30 ? true : false;
}

bool tag_is_complex(tl_t tag)
{
    return (tag & 0x20);
}

bool len_is_long(tl_t len) // get higher bit
{
   return (len & 0x80);
}

int get_cnt_byte_of_len(tl_t len) // make as lambda // get second  bit 1-7
{
    return (len & 0x7F);
}

// i - count of octet moves
tl_t merge_bytes(tl_t high_b, tl_t low_b, size_t i)
{
    return ((high_b << 8*i) | low_b);
}

/**
 * Function for decode tlv string
 */
void tlvs_decode(std::basic_string<char16_t> data,
                 std::vector<std::pair<size_t, tlv_t>>& box,
                 size_t order)
{
    size_t pos = 0;
    size_t offset = 2;
    tl_t tag = 0;
    std::uint_fast64_t len = 0;
    std::basic_string<char16_t> value;
    bool complex_tag = false;

    tag = data[pos];
    complex_tag = tag_is_complex(tag);
    if (octet_overflow(tag))
    {
        tag = merge_bytes(data[pos], data[pos+1], 1);
        pos++;
        offset +=1;
    }
    len = data[++pos];
    if (len_is_long(len))
    {
        int cnt_bytes = get_cnt_byte_of_len(len);

        if (cnt_bytes > 8)
            std::cout << "Too much length octets!" << std::endl;

        len = 0;
        for (int i = cnt_bytes; i < 0; i--)
        {
            len = merge_bytes(len, data[++pos], i);
        }
        offset += cnt_bytes;
    }

    value = data.substr(pos + 1, len);

    std::basic_string<char16_t> right, left;

    right = data.substr(len + offset, data.length() -(len + offset));
    left = data.substr(offset, len);

    if(complex_tag)
        order++;
    
    box.emplace_back(std::make_pair(order, tlv_t(tag, len, value)));

    if (left.length() > 1 && complex_tag)
        tlvs_decode(left, box, order);
    
    if (right.length() > 1)
        tlvs_decode(right, box, order);
}

void tlvs_encode(std::basic_string<char16_t>& data,
                 const std::vector<std::pair<size_t, tlv_t>>& box)
{
    
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
    std::string str1 = "6F1A840E315041592E5359532E4444463031A5088801025F2D02656E";
    std::string str2 = "6f2f840e325041592e5359532e4444463031a51dbf0c1a61184f07a0000000031010500a564953412044454249548701019000";
    auto hex_str = str_to_hex_arr(str2);
    print_str_as_hex(hex_str);


    std::cout << "Start encode" << std::endl;
    std::vector<std::pair<size_t, tlv_t>> tlv_box;
    // tlvs_decode(str, tlv_box);
    tlvs_decode(hex_str, tlv_box, 0);


    print_tlv_box(tlv_box);
    

    return 0;
}
