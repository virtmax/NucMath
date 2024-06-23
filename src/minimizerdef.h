
#pragma once

#include "vector.h"

#include <functional>
#include <vector>


namespace nucmath
{

typedef std::function<double(const std::vector<double>&)> FUNC2MIN;
typedef std::function<double(const std::vector<double>&, const nucmath::Vector<double>& input)> MODELFUNC;

enum class OPTIMIZER_RETURN_TYPE
{
    Unknown,
    MaxIterations,
    ToleranceReached
};

}
