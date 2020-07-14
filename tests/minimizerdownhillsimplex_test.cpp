#include "../catch.hpp"

#include "minimizerdownhillsimplex.h"
#include "functions.h"
#include "vector.h"
#include "minimizer.h"

TEST_CASE("Downhill simplex optimization")
{
    FUNC2MIN rosenbrock = [](const std::vector<double> &p)
    {
        double x = p[0];
        double y = p[1];
        double a = 1;
        double b = 100;
        double res = (a-x)*(a-x) + b*(y-x*x)*(y-x*x);
        return res;
    };

    SECTION( "rosenbrock function test" )
    {

        for(int i = 0; i < 10; i++)
        {
            const double x = nucmath::getRandom(-5, 5);
            const double y = nucmath::getRandom(-5, 5);
            std::vector<std::array<double, 3> > initial_p = { { x, -5, 5 }, { y, -5, 5 } };
            std::vector<double> result;
            std::vector<double> result_sigma;
            double tolerance = 0.000000001;
            size_t max_interations = 10000;
            size_t number_of_seed_points = 10;

            nucmath::downhill_simplex_optimization(rosenbrock,
                                                      initial_p,
                                                      result,
                                                      tolerance,
                                                      max_interations,
                                                      number_of_seed_points);

            REQUIRE(result[0] == Approx(1).epsilon(0.01));
            REQUIRE(result[1] == Approx(1).epsilon(0.01));
        }
    }
}
