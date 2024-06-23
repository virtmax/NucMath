

#pragma once

#include "functions.h"
#include "interpolationdata.h"

#include <cmath>
#include <iostream>
#include <vector>


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
