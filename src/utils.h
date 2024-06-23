


#include <cmath>
#include <limits>
#include <sstream>

#pragma once

namespace nucmath
{


// very naive version for floating point comparison,
// TODO: have to be improved
bool isEqual(float v1, float v2);
bool isEqual(double v1, double v2);

bool s2d(const std::string& str, double& val, const std::locale& loc = std::locale::classic());

}
