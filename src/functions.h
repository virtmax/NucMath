
#pragma once

#include "constants.h"

#include <array>
#include <chrono>
#include <random>
#include <vector>


namespace nucmath
{
/*
    Model: A*exp(-(x-µ)^2 / (2*sigma^2)) / sqrt(2*Pi*sigma^2)

    p - Array with model parameters
    0 = A
    1 = µ
    2 = sigma

    x - parameter

*/
double normal_dist_pdf(const std::array<double, 3>& p, double x);

// x = ableseort, mu = zentrum position, sigma = standardabweichung
double normal_dist_pdf(double mu, double sigma, double x);

// Summe aus zwei normal_dist_pdf
double normal_dist2_pdf(const std::array<double, 6>& p, double x);

double normal_dist_2d_pdf(const std::array<double, 6>& p, double x1, double x2);

double normal_dist_intersection_point(double mu1, double sigma1, double A1, double mu2, double sigma2, double A2);


/*
        p - Array with model parameters
        0 - Energy (Temperature)

        x - variable
        */
double maxwell_boltzmann_dist_pdf(const std::array<double, 1>& p, double x);


/*
        p - Array with model parameters
        0 - Energy (Temperature)
        1 - Watt-Parameter

        x - variable
        */
double watt_dist_pdf(const std::array<double, 2>& p, double x);


// TODO: auf gcc kompilier nicht thread-sicher !
double getRandom(double start, double end);


int sign(double x);

double sinc(double x);

double factorial(size_t x);

double binomialCoefficient(size_t n, size_t k);

// binomial Probability mass function
double binomialPMF(double p, size_t n, size_t k);

double rad2deg(double angle_rad);
double deg2rad(double angle_deg);
}
