#ifndef BASE64_H
#define BASE64_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace base64
{
    using byte = unsigned char;

    std::string encode(const std::vector<byte> & bytes);
    std::vector<byte> decode(const std::string & str);
}

#endif // BASE64_H
