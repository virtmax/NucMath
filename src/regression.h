


#pragma once

#include "utils.h"

#include <array>
#include <iostream>
#include <vector>

namespace nucmath
{
class Regression
{
public:
    Regression();
    ~Regression();

    class LinearRegressionResult
    {
    public:
        LinearRegressionResult() : a0(0.0), a1(0.0), r2(0.0), r2_red(0.0), sqr(0.0) {}

        double a0;       // intercept
        double a1;       // slope
        double r2;       // coefficient of determination
        double r2_red;   // reduced r2

        double sqr;   // sum of squared residuals

        bool operator<(const LinearRegressionResult& o) const { return (r2_red > o.r2_red); }
    };

    // Model: y = a0 + a1*x
    LinearRegressionResult linear(const std::vector<std::array<double, 2>>& points);


    class QuadraticRegressionResult
    {
    public:
        QuadraticRegressionResult() : a0(0.0), a1(0.0), a2(0.0), x0(0.0), c(0.0), y0(0.0), r2(0.0) {}

        // standard form coefficients: a0 + a1*x + a2*x^2
        double a0;
        double a1;
        double a2;

        // vertex form coefficients: c*(x-x0) + y0
        double x0;
        double c;
        double y0;

        double r2;   // coefficient of determination
    };

    // Model: y = a0 + a1*x + a2*x^2
    nucmath::Regression::QuadraticRegressionResult quadratic(const std::vector<std::array<double, 2>>& points);
};

}
