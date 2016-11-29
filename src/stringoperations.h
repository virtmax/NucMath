
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#pragma once

namespace nucmath
{ 


	std::vector<std::string> tokenize(const std::string& str, std::vector<char> delims);


    template <typename T>
    std::string to_string(const T a_value, const int n = 3)
    {
        std::ostringstream out;
        out << std::fixed << std::setprecision(n) << a_value;
        return out.str();
    }
}
