#include "bufsd/utils.h"

std::string bufsd::make_buffer_string(const std::vector<unsigned char> &buffer)
{
    std::stringstream ss;

    for (unsigned char val : buffer)
    {
        ss << std::hex << std::setfill('0') << std::setw(2) << (int)val;
    }

    return ss.str();
}

std::vector<unsigned char> bufsd::hex_string_to_byte_vector(const std::string &hex_string)
{
    std::string clean;
    clean.reserve(hex_string.size());

    for (char c : hex_string)
    {
        if (std::isspace(static_cast<unsigned char>(c)))
            continue;

        if (!std::isxdigit(static_cast<unsigned char>(c)))
            throw std::invalid_argument("invalid character in hex string");

        clean.push_back(c);
    }

    if (clean.size() % 2 != 0)
        throw std::invalid_argument("hex string must have even length");

    std::vector<unsigned char> bytes;
    bytes.reserve(clean.size() / 2);

    for (size_t i = 0; i < clean.size(); i += 2)
    {
        std::string byte_str = clean.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byte_str, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}