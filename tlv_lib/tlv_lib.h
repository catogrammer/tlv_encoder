#ifndef __TLV_LIB_H__
#define __TLV_LIB_H__

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

    friend std::ostream& operator<<(std::ostream& os, const tlv_t& tlv);
};

std::string get_str_as_hex(std::u16string in);
void print_tlv_box(const std::vector<std::pair<size_t, tlv_t>>& box);
void tlvs_decode(std::u16string data,
                 std::vector<std::pair<size_t, tlv_t>>& box,
                 size_t order);
void tlvs_encode(std::u16string& data,
                 const std::vector<std::pair<size_t, tlv_t>>& box);

#endif //__TLV_LIB_H__