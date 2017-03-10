

#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include "interpolationdata.h"
#include "functions.h"


namespace nucmath
{

class InterpolationSteffen
{
public:
    InterpolationSteffen();
    ~InterpolationSteffen();

    InterpolatedData interpolate(std::vector<double> x, std::vector<double> y);
    InterpolatedData interpolate(std::vector<float> y, double stepWidth);
};

}
