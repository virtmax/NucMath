
#include "hist.h"

#pragma once

namespace nucmath
{


class TimeSeries : public nucmath::Hist
{
public:
    TimeSeries();

    void setTimeGranularity(double timeBinWidth);
    void add(double time, double value);

    std::pair<double, double> data(size_t bin) const;

    double max();

    double min();

    double mean();

private:

    double lastTimeValue;
	double firstTimeValue;

    Hist entriesPerBin;
};

}
