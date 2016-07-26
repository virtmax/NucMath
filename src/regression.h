


#ifndef REGRESSION_H
#define REGRESSION_H

#include <iostream>
#include <vector>
#include <array>

namespace nucmath
{

	class Regression
	{
	public:
		Regression();
		~Regression();

		// ax + b
		void linear(const std::vector<std::array<double, 2>> &points, double &a, double&b, double &R2);

		// ax^2 + bx + c
		void quadratic(const std::vector<std::array<double, 2>> &points, double &a, double &b, double &c);

	};

}


#endif // REGRESSION_H
