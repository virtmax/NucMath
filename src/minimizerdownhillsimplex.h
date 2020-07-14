


#include <random>


#pragma once

#include "minimizerdef.h"
#include "regression.h"
#include "optpoint.h"

namespace nucmath
{

OPTIMIZER_RETURN_TYPE downhill_simplex_optimization(FUNC2MIN opt_func,
                                     const std::vector< std::array<double,3> > &initial_p,
                                     std::vector<double> &result_p,
                                     double tolerance,
                                     size_t max_interations,
                                     size_t number_of_seed_points);

}


