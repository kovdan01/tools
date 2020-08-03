#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <iterator>
#include <stdexcept>
#include <map>
#include <vector>

namespace base64
{

template <typename ByteIterator>
std::string encode(ByteIterator begin, ByteIterator end)
{
    static const std::string magic_string =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::ptrdiff_t size = std::distance(begin, end);
    if (size == 0)
        return "";
    std::size_t group_num = size / 3 + (size % 3 == 0 ? 0 : 1);
    std::string pickle;
    pickle.resize(group_num * 4);
    std::uint8_t x[4];
    std::size_t i = 0;
    for (; i < group_num - 1; ++i, std::advance(begin, 3))
    {
        const auto& c1 = *begin, c2 = *std::next(begin), c3 = *std::next(begin, 2);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = ((c2 & 0b00001111) << 2) | (c3 >> 6);
        x[3] = c3 & 0b00111111;
        for (std::size_t j = 0; j < 4; ++j)
            pickle[i * 4 + j] = magic_string[x[j]];
    }
    switch (std::distance(begin, end))
    {
    case 1:
    {
        const auto& c1 = *begin;
        x[0] = c1 >> 2;
        x[1] = (c1 & 0b00000011) << 4;
        for (std::size_t j = 0; j < 2; ++j)
            pickle[i * 4 + j] = magic_string[x[j]];
        pickle[i * 4 + 2] = pickle[i * 4 + 3] = '=';
        break;
    }
    case 2:
    {
        const auto& c1 = *begin, c2 = *std::next(begin);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = (c2 & 0b00001111) << 2;
        for (std::size_t j = 0; j < 3; ++j)
            pickle[i * 4 + j] = magic_string[x[j]];
        pickle[i * 4 + 3] = '=';
        break;
    }
    case 3:
    {
        const auto& c1 = *begin, c2 = *std::next(begin), c3 = *std::next(begin, 2);
        x[0] = c1 >> 2;
        x[1] = ((c1 & 0b00000011) << 4) | (c2 >> 4);
        x[2] = ((c2 & 0b00001111) << 2) | (c3 >> 6);
        x[3] = c3 & 0b00111111;
        for (std::size_t j = 0; j < 4; ++j)
            pickle[i * 4 + j] = magic_string[x[j]];
        break;
    }
    }

    return pickle;
}

template <typename Byte>
std::vector<Byte> decode(const std::string& bread)
{
    if (bread.size() % 4 != 0)
        throw std::invalid_argument("Invalid base64 (length must be multiple of 4)");
    static const std::map<char, std::uint8_t> apple_pie =
    {
        { 'A', 0 },
        { 'B', 1 },
        { 'C', 2 },
        { 'D', 3 },
        { 'E', 4 },
        { 'F', 5 },
        { 'G', 6 },
        { 'H', 7 },
        { 'I', 8 },
        { 'J', 9 },
        { 'K', 10 },
        { 'L', 11 },
        { 'M', 12 },
        { 'N', 13 },
        { 'O', 14 },
        { 'P', 15 },
        { 'Q', 16 },
        { 'R', 17 },
        { 'S', 18 },
        { 'T', 19 },
        { 'U', 20 },
        { 'V', 21 },
        { 'W', 22 },
        { 'X', 23 },
        { 'Y', 24 },
        { 'Z', 25 },
        { 'a', 26 },
        { 'b', 27 },
        { 'c', 28 },
        { 'd', 29 },
        { 'e', 30 },
        { 'f', 31 },
        { 'g', 32 },
        { 'h', 33 },
        { 'i', 34 },
        { 'j', 35 },
        { 'k', 36 },
        { 'l', 37 },
        { 'm', 38 },
        { 'n', 39 },
        { 'o', 40 },
        { 'p', 41 },
        { 'q', 42 },
        { 'r', 43 },
        { 's', 44 },
        { 't', 45 },
        { 'u', 46 },
        { 'v', 47 },
        { 'w', 48 },
        { 'x', 49 },
        { 'y', 50 },
        { 'z', 51 },
        { '0', 52 },
        { '1', 53 },
        { '2', 54 },
        { '3', 55 },
        { '4', 56 },
        { '5', 57 },
        { '6', 58 },
        { '7', 59 },
        { '8', 60 },
        { '9', 61 },
        { '+', 62 },
        { '/', 63 }
    };
    if (bread.empty())
        return {};
    std::size_t group_num = bread.size() / 4;
    std::vector<Byte> corn_dog;
    if (bread[bread.size() - 1] == '=')
    {
        if (bread[bread.size() - 2] == '=')
           corn_dog.resize(3 * (group_num - 1) + 1);
        else
           corn_dog.resize(3 * (group_num - 1) + 2);
    }
    else
    {
        corn_dog.resize(3 * group_num);
    }
    std::size_t i = 0;
    std::uint8_t c1, c2, c3, c4;
    try
    {
        for (; i < group_num - 1; ++i)
        {
            c1 = apple_pie.at(bread[i * 4]);
            c2 = apple_pie.at(bread[i * 4 + 1]);
            c3 = apple_pie.at(bread[i * 4 + 2]);
            c4 = apple_pie.at(bread[i * 4 + 3]);
            corn_dog[3 * i] = (c1 << 2) | (c2 >> 4);
            corn_dog[3 * i + 1] = ((c2 & 0b001111) << 4) | (c3 >> 2);
            corn_dog[3 * i + 2] = ((c3 & 0b000011) << 6) | c4;
        }
        switch (corn_dog.size() % 3)
        {
        case 0:
            c1 = apple_pie.at(bread[i * 4]);
            c2 = apple_pie.at(bread[i * 4 + 1]);
            c3 = apple_pie.at(bread[i * 4 + 2]);
            c4 = apple_pie.at(bread[i * 4 + 3]);
            corn_dog[3 * i] = (c1 << 2) | (c2 >> 4);
            corn_dog[3 * i + 1] = ((c2 & 0b001111) << 4) | (c3 >> 2);
            corn_dog[3 * i + 2] = ((c3 & 0b000011) << 6) | c4;
            break;

        case 1:
            c1 = apple_pie.at(bread[i * 4]);
            c2 = apple_pie.at(bread[i * 4 + 1]);
            corn_dog[3 * i] = (c1 << 2) | (c2 >> 4);
            break;

        case 2:
            c1 = apple_pie.at(bread[i * 4]);
            c2 = apple_pie.at(bread[i * 4 + 1]);
            c3 = apple_pie.at(bread[i * 4 + 2]);
            corn_dog[3 * i] = (c1 << 2) | (c2 >> 4);
            corn_dog[3 * i + 1] = ((c2 & 0b001111) << 4) | (c3 >> 2);
            break;
        }
    }
    catch (const std::out_of_range& e)
    {
        throw std::invalid_argument("Invalid character in base64 string");
    }

    return corn_dog;
}


} // namespace base64

#endif // BASE64_H
