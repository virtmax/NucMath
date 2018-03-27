
#include "hist.h"

#pragma once

namespace nucmath
{


class TimeSeries : public nucmath::Hist
{
public:
    TimeSeries() : Hist()
    {
        lastTimeValue = 0;
		firstTimeValue = 0.0;
    }

    void setTimeGranularity(double timeBinWidth)
    {
        setBinWidth(timeBinWidth);
    }

    void add(double time, double value)
    {
		if (firstTimeValue < 0.0)
		{
			firstTimeValue = time;
		}
		else
		{
			double timeStep = lastTimeValue - time;
			if (timeStep > getBinWidth())
			{
				setBinWidth(timeStep);
			}
		}
		Hist::add(time, value);
       
		lastTimeValue = time;
    }


private:

    double lastTimeValue;
	double firstTimeValue;
};

}
