#include "timeseries.h"

nucmath::TimeSeries::TimeSeries() : nucmath::Hist()
{
    lastTimeValue = 0;
    firstTimeValue = 0.0;
}

void nucmath::TimeSeries::setTimeGranularity(double timeBinWidth)
{
    setBinWidth(timeBinWidth);
    entriesPerBin.setBinWidth(timeBinWidth);
}

void nucmath::TimeSeries::add(double time, double value)
{
    if (firstTimeValue < 0.0)
    {
        firstTimeValue = time;
    }
    else
    {
        /*
        double timeStep = lastTimeValue - time;
        if (timeStep > getBinWidth())
        {
            setBinWidth(timeStep);
        }
        */
    }
    Hist::add(time, value);

    entriesPerBin.add(time, 1.0);

    lastTimeValue = time;
}

std::pair<double, double> nucmath::TimeSeries::data(size_t bin) const
{
    if(bin < entriesPerBin.nBins())
    {
        if(entriesPerBin.data(bin).second > 0.0)
            return std::pair<double, double>(Hist::data(bin).first, Hist::data(bin).second/(entriesPerBin.data(bin).second*entriesPerBin.getBinWidth()));
        else
            return std::pair<double, double>(Hist::data(bin).first, 0);
    }
    else
        throw std::out_of_range("nucmath::TimeSeries::data(std::size_t bin): bin="+std::to_string(bin));
}

double nucmath::TimeSeries::max()
{
    double maxVal = std::numeric_limits<double>::min();
    for(size_t i = 0; i < nBins(); i++)
    {
        if(data(i).second > maxVal)
        {
            maxVal = data(i).second;
        }
    }

    return maxVal;
}

double nucmath::TimeSeries::min()
{
    double minVal = std::numeric_limits<double>::max();
    for(size_t i = 0; i < nBins(); i++)
    {
        if(data(i).second < minVal)
        {
            minVal = data(i).second;
        }
    }

    return minVal;
}

double nucmath::TimeSeries::mean()
{
    double sum = 0.0;
    for(size_t i = 0; i < nBins(); i++)
    {
        sum += Hist::data(i).second/(entriesPerBin.data(i).second);
    }

    return sum/nBins();
}
