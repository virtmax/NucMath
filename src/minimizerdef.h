
#include <functional>
#include <vector>
#include "vector.h"

#pragma once

namespace nucmath
{

typedef std::function<double(const std::vector<double> &)> FUNC2MIN;
typedef std::function<double(const std::vector<double> &, const nucmath::Vector<double> &input)> MODELFUNC;

enum OPTIMIZER_RETURN_TYPE {Unknown, MaxIterations, ToleranceReached};

}
