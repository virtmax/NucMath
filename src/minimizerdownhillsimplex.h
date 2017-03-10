


#include <random>


#pragma once

#include "regression.h"
#include "optpoint.h"

namespace nucmath
{

double downhill_simplex_optimization(FUNC2MIN opt_func,
                                     const std::vector< std::array<double,3> > &initial_p,
                                     std::vector<double> &result_p,
                                     std::vector<double> &result_sigma,
                                     double tolerance,
                                     size_t max_interations,
                                     size_t number_of_seed_points);

}


