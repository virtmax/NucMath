
#include <functional>
#include <vector>
#include "vector.h"

#pragma once

typedef std::function<double(const std::vector<double> &)> FUNC2MIN;
typedef std::function<double(const std::vector<double> &, const nucmath::Vector<double> &input)> MODELFUNC;
