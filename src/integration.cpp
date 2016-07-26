
#include "integration.h"



// mu = zentrum
double nucmath::normalDistIntegral(double mu, double sigma, double from, double to)
{
	return 0.5*(std::erf((mu - from) / (std::sqrt(2)*sigma)) - std::erf((mu - to) / (std::sqrt(2)*sigma)));
}


