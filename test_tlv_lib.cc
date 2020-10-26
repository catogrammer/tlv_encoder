#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "tlv_lib.h"


#include <codecvt>
#include <locale>

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
    std::cout << get_str_as_hex(hex_str) << std::endl;
    std::cout << "Encoded  : ";
    std::cout << get_str_as_hex(enc_str) << std::endl;

    return 0;
}
