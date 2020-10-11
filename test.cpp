#include <iostream>
#include <bitset>


int bit_extracted(std::uint_fast16_t number, size_t k, size_t p)  //chaeck this func
{ 
    return (((1 << k) - 1) & (number >> (p - 1))); 
}

int main(int argc, char const *argv[])
{
    // std::cout << "Tag is" << sizeof(tag) << " = " << sizeof(*value) << std::endl;
    std::string s = "0xBF0C";
    std::string s1 = "BF";
    std::string s2 = "84";

    // std::uint_fast16_t tag = std::stoul(s2, nullptr, 16);
    // std::cout << tag << std::hex << "   " << (tag & 0x20) << std::endl;

    std::uint_fast16_t tag1 = 0;
    tag1 = std::stoul(s2, nullptr, 16);
    // std::uint_fast16_t tag2 = std::stoul(s2, nullptr, 16);
    // std::cout << tag1 << "   " << tag2 << std::endl;
    // // std::cout << std::hex << ((tag1 << 8) | tag2) << std::endl;
    // std::cout << std::bitset<8>(tag1) << std::endl;

    std::cout << std::bitset<8>(tag1) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 1) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 2) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 3) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 4) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 5) << std::endl;
    std::cout << std::bitset<8>(tag1 >> 6) << std::endl;
    std::cout << std::bitset<8>(tag1 & 0x7) << std::endl;

    // int len = 0;
    // len = (len << 8) | 10;
    // std::cout << std::bitset<16>(len) << std::endl;
    // len = (len << 8) | 2;
    // std::cout << std::bitset<24>(len) << " " << len << std::endl;

    // std::cout << std::bitset<24>((len << 8) | 10) << std::endl;



    return 0;
}
