#pragma once

#include <vector>
#include <iostream>

namespace nucmath
{

struct InterpolationParameter
{
    double x_min;
    double p[4];
};

class InterpolatedData
{
public:
    InterpolatedData() { last_indx = 0; };
    ~InterpolatedData(){};

    double operator[](double x) const
    {
        if(interpolationData.size() == 0)
        {
            std::cout << "InterpolatedData::operator[]: interpolationData.size() == 0" << std::endl;
            return 0.0;
        }
        if(x < x_min || x > x_max)
        {
            std::cout << "InterpolatedData::operator[]: x außerhalb des Interpolationsbereichs." << std::endl;
            return 0.0;
        }

        // Nach dem richtigen Spline-in der liste Suchen. Binäre Suche.
        size_t imin = 0, imax = interpolationData.size()-1;

        while(imin <= imax)
        {
            size_t indx =  imin + ((imax - imin) / 2);  // binäre Suche

            const InterpolationParameter &intpDi = interpolationData.at(indx);

            // den Intervall überprüfen
            if(intpDi.x_min <= x
                    && (indx+1 < interpolationData.size()) ? (x <= interpolationData.at(indx+1).x_min) : false)
            {
                x = x - intpDi.x_min;
                return intpDi.p[0]*x*x*x + intpDi.p[1]*x*x + intpDi.p[2]*x + intpDi.p[3];
            }
            else
            {
                // Suchgrenzen verschieben
                if(x < intpDi.x_min)
                {
                    imax = indx-1;
                }
                else
                {
                    imin = indx+1;
                }
            }


        }

        return 0;
    }

    double x_min, x_max;


    std::vector<InterpolationParameter> interpolationData;

private:
    size_t last_indx;
};

}
