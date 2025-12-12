#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

namespace bufsd
{
    std::string make_buffer_string(const std::vector<unsigned char> &buffer);

    std::vector<unsigned char> hex_string_to_byte_vector(const std::string &hex_string);
}
