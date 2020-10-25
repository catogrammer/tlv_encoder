#include <iostream>
#include <bitset>
#include <locale>
#include <codecvt>


int bit_extracted(std::uint_fast16_t number, size_t k, size_t p)  //chaeck this func
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
}

void print_str_as_hex(std::u16string in)
{
    for (auto &&i : in)
    {
        std::cout << std::hex << i << ' ';
    }
    std::cout << std::endl;
}

std::u16string to_u16string(uint_fast16_t const& i) {
    std::cout << "i : " << std::hex << i << std::endl;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.from_bytes(std::to_string(i));
}

int main(int argc, char const *argv[])
{
    // std::cout << "Tag is" << sizeof(tag) << " = " << sizeof(*value) << std::endl;
    std::string s = "0xBF0C";
    std::string s1 = "6F";
    std::string s2 = "2f";

    // std::uint_fast16_t tag = std::stoul(s2, nullptr, 16);
    // std::cout << tag << std::hex << "   " << (tag & 0x20) << std::endl;

    std::uint_fast16_t tag1 = 0, tag2 = 0;
    std::u16string str_16_t;
    tag1 = std::stoul(s, nullptr, 16);
    tag2 = std::stoul(s2, nullptr, 16);


    // std::uint_fast16_t tag2 = std::stoul(s2, nullptr, 16);
    // std::cout << tag1 << "   " << tag2 << std::endl;
    // std::cout << std::bitset<8>(tag1) << std::endl;

    std::cout << std::bitset<16>(tag1) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 1) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 2) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 3) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 4) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 5) << std::endl;
    // std::cout << std::bitset<8>(tag1 >> 6) << std::endl;
    std::uint_fast16_t tmp = ((tag1 >> 8) & 0x20);
    std::cout << std::bitset<16>(tmp) << std::endl;

    // int len = 0;
    // len = (len << 8) | 10;
    // std::cout << std::bitset<16>(len) << std::endl;
    // len = (len << 8) | 2;
    // std::cout << std::bitset<24>(len) << " " << len << std::endl;

    // std::cout << std::bitset<24>((len << 8) | 10) << std::endl;

    return 0;
}
