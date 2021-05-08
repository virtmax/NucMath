#include "../catch.hpp"

#include "minimizerdownhillsimplex.h"
#include "functions.h"
#include "vector.h"
#include "minimizer.h"

TEST_CASE("Minimizer ")
{


    std::vector<double> result;
    std::vector<double> result_sigma;
    std::vector<double> xv = {-3, -1.4, 1, 2, 3, 4, 5};
    std::vector<double> yv = {-3, -0.5, 1.3, 4, -1.2, 3.9, 1};
    // 0.554442 x - 0.0538699

    std::vector<std::array<double, 3> > initial_p = {  { 0, -10, 10 },
                                                        { 0, -10, 10 }  };

    nucmath::MODELFUNC lineModel = [&](const std::vector<double> &p, const nucmath::Vector<double> &x)
    {
        return p[0] + p[1]*x(0);
    };

    nucmath::Minimizer minimizer;
    minimizer.setData(xv,yv);
    minimizer.setNumberOfSeedPoints(10);
    minimizer.setInitialPointsAndConstrains(initial_p);
    minimizer.setParameterNames({"a0","a1"});
    //std::cout << minimizer.getFormatedInitialPoints();
    //std::cout << minimizer.getFormatedConstrains();

    minimizer.setModelFunction(lineModel);

    //std::cout << "Stop creterion: " << minimizer.getReturnType() << std::endl;
    //std::cout << minimizer.getFormatedFitResults() << std::endl;

    for(int i = 0; i < 10; i++)
    {
        minimizer.findFit(1000, 0.0000001);
        result = minimizer.getResult();
        result_sigma = minimizer.getResultUncertainty();

        REQUIRE(result[0] == Approx(-0.053869).epsilon(0.1));
        REQUIRE(result[1] == Approx(0.554442).epsilon(0.1));

        REQUIRE(result_sigma[0] == Approx(0.970171).epsilon(0.1));
        REQUIRE(result_sigma[1] == Approx(0.316051).epsilon(0.1));
    }
}
