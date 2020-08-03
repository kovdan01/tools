#include "base64.h"
#include <iostream>

namespace base64
{
    static const std::string positions_to_symbols{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"}; //NOLINT(cert-err58-cpp)
    static const byte symbols_to_positions[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
                                                 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255, 255, 255, 255, 255,
                                                255,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
                                                 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
                                                255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
                                                 41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                                                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

    void decode_helper(std::uint32_t & buff, const std::string & str, std::size_t index)
    {
        buff <<= 6;
        byte sym = static_cast<byte>(str[index]);
        if (symbols_to_positions[sym] == 255)
            throw std::invalid_argument("Input parameter is not a correct base64 string.\n");
        buff |= symbols_to_positions[sym];
    }

    std::vector<byte> decode(const std::string & str)
    {
        std::size_t n = str.size();
        if (n % 4 != 0)
            throw std::invalid_argument("Input parameter is not a correct base64.\n");
        std::uint32_t buff;
        std::vector<byte> ans;
        ans.reserve((1 + n / 4) * 3);
        for (std::size_t i = 4; i <= n - 4; i += 4)
        {
            buff = 0;
            for (std::size_t j = i - 4; j < i; ++j)
                decode_helper(buff, str, j);
            ans.emplace_back((buff & 0b111111110000000000000000) >> 16);
            ans.emplace_back((buff & 0b1111111100000000) >> 8);
            ans.emplace_back((buff & 0b11111111));
        }
        buff = 0;
        std::size_t current = n - 4;
        if (n && str[n - 2] == '=' && str[n - 1] == '=')
        {
            for (std::size_t j = current; j < n - 2; ++j)
                decode_helper(buff, str, j);
            ans.emplace_back((buff & 0b111111110000) >> 4);
        }
        else if (n && str[n - 1] == '=')
        {
            for (std::size_t j = current; j < n - 1; ++j)
                decode_helper(buff, str, j);
            ans.emplace_back((buff & 0b111111110000000000) >> 10);
            ans.emplace_back((buff & 0b1111111100) >> 2);
        }
        else
        {
            for (std::size_t j = current; j < n; ++j)
                decode_helper(buff, str, j);
            ans.emplace_back((buff & 0b111111110000000000000000) >> 16);
            ans.emplace_back((buff & 0b1111111100000000) >> 8);
            ans.emplace_back((buff & 0b11111111));
        }
        return ans;
    }

    std::string encode(const std::vector<byte> & bytes)
    {
        std::size_t n = bytes.size();
        std::uint32_t buff;
        std::string ans;
        ans.reserve((1 + n / 3) * 4);
        for (std::size_t i = 3; i <= n; i += 3)
        {
            buff = 0;
            for (std::size_t j = i - 3; j < i; ++j)
            {
                buff <<= 8;
                buff |= bytes[j];
            }
            ans += positions_to_symbols[(buff & 0b111111000000000000000000) >> 18];
            ans += positions_to_symbols[(buff & 0b111111000000000000) >> 12];
            ans += positions_to_symbols[(buff & 0b111111000000) >> 6];
            ans += positions_to_symbols[(buff & 0b111111)];
        }
        std::size_t offset = n % 3, current = n - offset;
        if (offset)
        {
            buff = 0;
            if (offset == 1)
            {
                buff |= bytes[current];
                buff <<= 4;
                ans += (positions_to_symbols[(buff & 0b111111000000) >> 6]);
                ans += (positions_to_symbols[(buff & 0b111111)]);
                ans += "==";
            }
            else
            {
                buff |= bytes[current];
                buff <<= 8;
                buff |= bytes[current + 1];
                buff <<= 2;
                ans += positions_to_symbols[(buff & 0b111111000000000000) >> 12];
                ans += positions_to_symbols[(buff & 0b111111000000) >> 6];
                ans += positions_to_symbols[(buff & 0b111111)];
                ans += '=';
            }
        }
        return ans;
    }
}
