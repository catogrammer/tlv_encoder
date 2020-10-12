#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "tlv_lib/tlv_lib.h"

typedef char16_t tl_t; // tag_lenght_type


void print_str_as_hex(std::u16string in)
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
size_t cnt_octet_in(tl_t tag)
{
    size_t cnt = 0;
    while (tag >> 8)
    {
        cnt++;
        tag = tag >> 8;
    }
    return cnt;
}

bool octet_overflow(const tl_t& tag) // make as lambda
{
    return ((tag >> cnt_octet_in(tag)*8) & 0x1F) > 30 ? true : false;
}

bool tag_is_complex(const tl_t& tag)
{
    return ((tag >> cnt_octet_in(tag)*8) & 0x20);
}

bool len_is_long(const tl_t& len) // get higher bit
{
   return (len & 0x80);
}

int get_cnt_byte_of_len(const tl_t& len) // make as lambda // get second  bit 1-7
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
void tlvs_decode(std::u16string data,
                 std::vector<std::pair<size_t, tlv_t>>& box,
                 size_t order)
{
    size_t pos = 0;
    size_t offset = 2;  //for check, when len and tag more than one octet
    tl_t tag = 0;
    std::uint_fast64_t len = 0;
    std::u16string value, right, left;

    tag = data[pos];
    if (octet_overflow(tag))
    {
        tag = merge_bytes(data[pos], data[pos+1], 1); // I think that max tag len equals 2 octets
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
    right = data.substr(len + offset, data.length() -(len + offset));
    left = data.substr(offset, len);

    if(tag_is_complex(tag))
        order++;
    
    box.emplace_back(std::make_pair(order, tlv_t(tag, len, value)));

    if (left.length() > 1 && tag_is_complex(tag))
        tlvs_decode(left, box, order);
    
    if (right.length() > 1)
        tlvs_decode(right, box, order);
}

tl_t* split_tag(const tl_t& tag)
{
    tl_t* tag_arr = new tl_t[2];

    tag_arr[0] = (tag >> 8);
    tag_arr[1] = (tag & 0xFF);

    return tag_arr;
}

void tlvs_encode(std::u16string& data,
                 const std::vector<std::pair<size_t, tlv_t>>& box)
{
    for (size_t i = 0; i < box.size(); i++)
    {
        std::u16string tmp;
        tl_t tmp_tag = box[i].second._tag;

        if (octet_overflow(tmp_tag))
        {
            tl_t* tag_arr = split_tag(tmp_tag);
            tmp = ((tmp + tag_arr[0]) + tag_arr[1]) + box[i].second._len;
            delete[] tag_arr;
        }
        else
            tmp = (tmp + tmp_tag) + box[i].second._len;

        if (tag_is_complex(tmp_tag))
            data += tmp;
        else
            data += tmp + box[i].second._value;
    }
}

std::u16string str_to_hex_arr(std::string in)
{
    unsigned char* buff = new unsigned char[int(in.length()/2)];
    std::u16string out;
    for (int i = 0; i < in.length(); i+=2)
    {
        out += std::stoul(in.substr(i, 2), nullptr, 16);
    }
    std::cout << std::endl;
    return out;
}

int main(int argc, char const *argv[])
{
    std::string str1 = "6F1A840E315041592E5359532E4444463031A5088801025F2D02656E";
    std::string str2 = "6f2f840e325041592e5359532e4444463031a51dbf0c1a61184f07a0000000031010500a564953412044454249548701019000";
    std::u16string hex_str;
    std::vector<std::pair<size_t, tlv_t>> tlv_box;
    
    hex_str = str_to_hex_arr(str1);
    tlvs_decode(hex_str, tlv_box, 0);
    print_tlv_box(tlv_box);

    std::u16string enc_str;
    tlvs_encode(enc_str, tlv_box);
    std::cout << "Original : ";
    print_str_as_hex(hex_str);
    std::cout << "Encoded  : ";
    print_str_as_hex(enc_str);
    

    return 0;
}
