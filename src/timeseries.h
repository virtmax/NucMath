

#pragma once

#include "hist.h"

namespace nucmath
{


class TimeSeries
{
public:
    TimeSeries();

    void setTimeGranularity(double timeBinWidth);
    void add(double time, double value);

    size_t bin(double t);
    std::pair<double, double> data(size_t bin) const;

    size_t nBins() const { return hist.nBins(); }
    bool isChanged(bool leaveChanged = false);

    double max();

    double min();

    double mean();

    bool save(const std::string& path) const;

private:
    double lastTimeValue;
    double firstTimeValue;

    Hist entriesPerBin;
    Hist hist;
};

}
