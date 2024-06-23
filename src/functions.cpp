
#include "functions.h"


// x = ableseort, mu = zentrum position, sigma = standardabweichung
double nucmath::normal_dist_pdf(double mu, double sigma, double x)
{
    // 1/sqrt(2*Pi) = 0.398942280401432677

    return (0.398942280401432677 / sigma) * std::exp(-0.5 * (((mu - x) * (mu - x)) / (sigma * sigma)));
}

double nucmath::normal_dist_pdf(const std::array<double, 3>& p, double x)
{
    return p[0] * std::exp(-((p[1] - x) * (p[1] - x)) / (2.0 * (p[2] * p[2]))) / (SQRT_OF_PI2 * std::fabs(p[2]));
}

double nucmath::normal_dist2_pdf(const std::array<double, 6>& p, double x)
{
    return p[0] * std::exp(-((p[1] - x) * (p[1] - x)) / (2.0 * p[2] * p[2])) / (SQRT_OF_PI2 * p[2]) +
           p[3] * std::exp(-((p[4] - x) * (p[4] - x)) / (2.0 * p[5] * p[5])) / (SQRT_OF_PI2 * p[5]);
}

/*
    0 = A
    1 = µ1
    2 = sigma1
    3 = µ2
    4 = sigma2
    5 = rho (-1 < rho < 1)!
*/
double nucmath::normal_dist_2d_pdf(const std::array<double, 6>& p, double x1, double x2)
{
    const double t1 = (p[1] - x1) * (p[1] - x1) / (p[2] * p[2]);
    const double t2 = (p[3] - x2) * (p[3] - x2) / (p[4] * p[4]);
    const double t3 = 2 * p[5] * (p[1] - x1) * (p[3] - x2) / (p[2] * p[4]);
    const double rho = p[5];

    return p[0] * std::exp(-(t1 + t2 - t3) / (2 * (1 - rho * rho))) / (PI2 * p[2] * p[4] * sqrt(1 - rho * rho));
}


double nucmath::normal_dist_intersection_point(double mu1, double sig1, double A1, double mu2, double sig2, double A2)
{
    if(sig1 == sig2)   // TODO: abfrage instabil
    {
        return (mu1 + mu2) / 2.0;
    }
    else
    {
        const double sig1_2 = sig1 * sig1;
        const double sig2_2 = sig2 * sig2;
        const double llog = log((A1 * sig2) / (A2 * sig1));

        return (mu1 * sig2_2 - mu2 * sig1_2 + sig1 * sig2 * sqrt((mu1 - mu2) * (mu1 - mu2) - 2 * (sig2_2 - sig1_2) * llog)) / (sig2_2 - sig1_2);
    }
}


double nucmath::maxwell_boltzmann_dist_pdf(const std::array<double, 1>& p, double x)
{
    const double factor = PI2 / std::pow(PI * p[0], 1.5);
    return factor * std::sqrt(x) * std::exp(-x / p[0]);
}


double nucmath::watt_dist_pdf(const std::array<double, 2>& p, double x)
{
    const double Tw = p[0];
    const double Ew = p[1];

    return (2.0 / sqrt(PI * 4 * Ew * Tw)) * exp((-Ew - x) / Tw) * sinh(sqrt(4 * Ew * x / (Tw * Tw)));
}


std::mt19937 rnd1 = std::mt19937((unsigned int)time(0));
std::default_random_engine rnd2 = std::default_random_engine((unsigned int)time(0));

double nucmath::getRandom(double start, double end)
{
#ifdef __MINGW32__
    return start + static_cast<double>(rand()) / static_cast<float>(RAND_MAX / (end - start));
#else
    std::uniform_real_distribution<double> randUniform(0.0, 1.0);
    return start + randUniform(rnd2) * abs(end - start);
#endif
}


int nucmath::sign(double x)
{
    if(std::signbit(x))
        return -1;
    else
    {
        if(x < 10e-7)
            return 0;
        else
            return 1;
    }
}

double nucmath::sinc(double x)
{
    if(x > -1e-8 && x < 1e-8)
        return 1.0;

    return std::sin(PI * x) / (PI * x);
}

double nucmath::factorial(size_t x)
{
    if(x == 0)
        return 1.0;
    else
        return x * factorial(x - 1);
}

double nucmath::binomialCoefficient(size_t n, size_t k)
{
    if(k > n)
        return 0;

    if(2 * k > n)
        k = n - k;

    double res = 1;
    for(size_t i = 1; i <= k; i++)
        res *= (n + 1 - i) / static_cast<double>(i);

    return res;
}

double nucmath::binomialPMF(double p, size_t n, size_t k)
{
    if(k > n)
        return 0;

    double sum = 0.0;
    for(size_t i = 0; i <= k; i++)
        sum += binomialCoefficient(n, i) * std::pow(p, i) * std::pow(1.0 - p, n - i);

    return sum;
}

double nucmath::rad2deg(double angle_rad)
{
    return 57.2957795130823 * angle_rad;
}

double nucmath::deg2rad(double angle_deg)
{
    return 0.0174532925199439 * angle_deg;
}
