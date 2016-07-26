
#include <functional>
#include <vector>

#pragma once

typedef std::function<double(const std::vector<double> &)> FUNC2MIN;
typedef std::function<double(const std::vector<double> &, double x)> MODELFUNC;