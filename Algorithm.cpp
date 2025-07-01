#include "include/Algorithm.hpp"
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iostream>
#include <cctype>

namespace Algorithm {

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool isBase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64Decode(const std::string& input) {
    std::string base64_str = input;

    int mod = base64_str.size() % 4;
    if (mod != 0) {
        base64_str.append(4 - mod, '=');
    }

    int in_len = base64_str.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (base64_str[in_] != '=') && isBase64(base64_str[in_])) {
        char_array_4[i++] = base64_str[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

            char_array_3[0] = static_cast<unsigned char>(( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4));
            char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
            char_array_3[2] = static_cast<unsigned char>(((char_array_4[2] & 0x3) << 6) +   char_array_4[3]);

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

        char_array_3[0] = static_cast<unsigned char>(( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4));
        char_array_3[1] = static_cast<unsigned char>(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
        char_array_3[2] = static_cast<unsigned char>(((char_array_4[2] & 0x3) << 6) +   char_array_4[3]);

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

std::string xorDecode(const std::string& data, char key) {
    std::string output;
    output.reserve(data.size());
    for (auto c : data) {
        output.push_back(c ^ key);
    }
    return output;
}

std::string extractString(const std::string& source, 
                          const std::string& start, 
                          const std::string& end) {
    size_t pos1 = source.find(start);
    if (pos1 == std::string::npos) return "";
    pos1 += start.length();
    size_t pos2 = source.find(end, pos1);
    if (pos2 == std::string::npos) return "";
    return source.substr(pos1, pos2 - pos1);
}

std::string urlDecode(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '%') {
            if (i + 2 < input.size()) {
                std::string hex = input.substr(i + 1, 2);
                char decoded_char = static_cast<char>(std::stoi(hex, nullptr, 16));
                result += decoded_char;
                i += 2;
            } else {
                throw std::runtime_error("Invalid URL encoding");
            }
        } else if (input[i] == '+') {
            result += ' ';
        } else {
            result += input[i];
        }
    }
    return result;
}

}