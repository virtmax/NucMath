#include "utils.h"


bool nucmath::isEqual(float v1, float v2)
{
    return (std::abs(v1 - v2) < std::numeric_limits<float>::epsilon()) ? true : false;
}

bool nucmath::isEqual(double v1, double v2)
{
    return (std::abs(v1 - v2) < std::numeric_limits<double>::epsilon()) ? true : false;
}
