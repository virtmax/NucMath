


#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "utils.h"

namespace nucmath
{
	class Regression
	{
	public:
		Regression();
		~Regression();

        struct LinearRegressionResult
        {
            double a0;  // intercept
            double a1;  // slope
            double r2;  // coefficient of determination
        };

        // Model: y = a0 + a1*x
        LinearRegressionResult linear(const std::vector<std::array<double, 2>> &points);


        struct QuadraticRegressionResult
        {
            double a0;  // slope
            double a1;  // slope
            double a2;  // intercept
            double r2;  // coefficient of determination
        };

        // Model: y = a0 + a1*x + a2*x^2
        nucmath::Regression::QuadraticRegressionResult quadratic(const std::vector<std::array<double, 2>> &points);

	};

}

