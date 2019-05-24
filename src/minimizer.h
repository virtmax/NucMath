


#pragma once


#include <vector>
#include <iostream>
#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <string>

#include "vector.h"
#include "minimizerrandom.h"
#include "minimizerdownhillsimplex.h"

namespace nucmath
{


class Minimizer
{
public:
    Minimizer();
    ~Minimizer();

    enum MinimizerMethode {Random = 0, DownhillSimplex };

    void setData(const std::vector<double> &inputs, const std::vector<double> &y);
    void setData(const std::vector<nucmath::Vector<double>> &inputs, const std::vector<double> &y);
    void setWeights(const std::vector<double> &weights);

    void setNumberOfSeedPoints(size_t number_of_seed_points);

   // TODO: void setDataUncertainty()
    void setModelFunction(MODELFUNC &modelFunction);
    void setInitialPointsAndConstrains(const std::vector< std::array<double,3> > &initial_p);

    void setParameterNames(std::vector<std::string> names);

    double findFit(size_t interations, double tolerance);


    std::string getFormatedInitialPoints();
    std::string getFormatedConstrains();
    std::string getFormatedInitialPointsAndConstrains();
    std::string getFormatedFitResults();

    std::vector<double> getResult() { return result; }
    std::vector<double> getResultUncertainty() { return result_sigma; }

private:

    std::vector<nucmath::Vector<double>> inputs;
    std::vector<double> y;    // data
    std::vector<double> weights;

    size_t number_of_seed_points;

    MinimizerMethode minMode;

    FUNC2MIN func2min;

    std::mt19937 rand_gen;

    std::vector< std::array<double,3> > initial_points;
    std::vector<std::string> param_names;
    std::vector<double> result;
    std::vector<double> result_sigma;
};

}


