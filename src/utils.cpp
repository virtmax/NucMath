#include "utils.h"


bool nucmath::isEqual(float v1, float v2)
{
    return (std::abs(v1 - v2) < std::numeric_limits<float>::epsilon()) ? true : false;
}

bool nucmath::isEqual(double v1, double v2)
{
    return (std::abs(v1 - v2) < std::numeric_limits<double>::epsilon()) ? true : false;
}

bool nucmath::s2d(const std::string &str, double &val, const std::locale &loc)
{
  std::stringstream ss(str);
  ss.imbue(loc);
  ss >> val;
  return ss.eof() && !ss.fail();
}
