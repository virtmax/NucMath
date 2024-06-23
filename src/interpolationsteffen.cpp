
#include "interpolationsteffen.h"


nucmath::InterpolatedData nucmath::InterpolationSteffen::interpolate(std::vector<double> x, std::vector<double> y)
{
    InterpolatedData intpData;

    if(x.size() != y.size())
    {
        std::cout << "InterpolationSteffen::interpolate : x.size() != y.size()" << std::endl;
    }

    double hi = 0.0, si = 0.0, pi = 0.0;
    double hi_p1 = 0.0, si_p1 = 0.0, pi_p1 = 0.0;
    double si_m1 = 1.0, hi_m1 = 1.0;
    double ysi = 0.0, ysi_p1 = 0.0;

    // Weil nur bis size()-1 interpoliert wird, am ende noch einen doppelten datenpunkt hinzufügen
    x.push_back(x.at(x.size() - 1) + (x.at(x.size() - 1) - x.at(x.size() - 2)));   // schritt wie zwischen den letzten beiden punkten
    y.push_back(y.at(y.size() - 1));

    for(size_t i = 0; i < x.size() - 1; ++i)
    {
        hi = x[i + 1] - x[i];
        si = (y[i + 1] - y[i]) / hi;
        pi = (si_m1 * hi + si * hi_m1) / (hi_m1 + hi);

        if(i < x.size() - 2)
        {
            hi_p1 = x[i + 2] - x[i + 1];
            si_p1 = (y[i + 2] - y[i + 1]) / hi_p1;
            pi_p1 = (si * hi_p1 + si_p1 * hi) / (hi + hi_p1);

            ysi = (nucmath::sign(si_m1) + nucmath::sign(si)) * std::min(fabs(si_m1), std::min(fabs(si), fabs(pi) / 2));
            ysi_p1 = (nucmath::sign(si) + nucmath::sign(si_p1)) * std::min(fabs(si), std::min(fabs(si_p1), fabs(pi_p1) / 2));
        }
        else if(i == 0)
        {
            ysi = 0;
            ysi_p1 = 0;
        }
        else
        {
            ysi = 0;
        }

        InterpolationParameter intp;
        intp.p[0] = (ysi + ysi_p1 - 2 * si) / (hi * hi);
        intp.p[1] = (3 * si - 2 * ysi - ysi_p1) / hi;
        intp.p[2] = ysi;
        intp.p[3] = y[i];
        intp.x_min = x[i];
        intpData.interpolationData.push_back(intp);

        si_m1 = si;
        hi_m1 = hi;
    }


    intpData.x_min = x.at(0);
    intpData.x_max = x.at(x.size() - 1);

    return intpData;
}

nucmath::InterpolatedData nucmath::InterpolationSteffen::interpolate(std::vector<float> y, double stepWidth)
{
    std::vector<double> x;
    std::vector<double> yy;
    for(size_t i = 0; i < y.size(); i++)
    {
        x.push_back(i * stepWidth);
        yy.push_back(y[i]);
    }

    return interpolate(x, yy);
}


nucmath::InterpolationSteffen::InterpolationSteffen()
{
}

nucmath::InterpolationSteffen::~InterpolationSteffen()
{
}
