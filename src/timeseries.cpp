#include "timeseries.h"

nucmath::TimeSeries::TimeSeries()
{
    lastTimeValue = 0;
    firstTimeValue = 0.0;
}

void nucmath::TimeSeries::setTimeGranularity(double timeBinWidth)
{
    hist.setBinWidth(timeBinWidth);
    entriesPerBin.setBinWidth(timeBinWidth);
}

void nucmath::TimeSeries::add(double time, double value)
{
    if(firstTimeValue < 0.0)
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
    hist.add(time, value);

    entriesPerBin.add(time, 1.0);

    lastTimeValue = time;
}

size_t nucmath::TimeSeries::bin(double t)
{
    if(t >= hist.getLowestEdge() && t < hist.getLowestEdge() + hist.nBins() * hist.getBinWidth())
        return static_cast<size_t>(floor((t - hist.getLowestEdge()) / hist.getBinWidth()));
    else
        throw std::invalid_argument("TimeSeries::bin(double x): x is outside the data range.");
}

std::pair<double, double> nucmath::TimeSeries::data(size_t bin) const
{
    if(bin < entriesPerBin.nBins())
    {
        if(entriesPerBin.data(bin).second > 0.0)
        {
            const double y = hist.data(bin).second / (entriesPerBin.data(bin).second);
            return std::pair<double, double>(hist.data(bin).first, y);
        }
        else
            return std::pair<double, double>(hist.data(bin).first, 0);
    }
    else
        throw std::out_of_range("nucmath::TimeSeries::data(std::size_t bin): bin=" + std::to_string(bin));
}

bool nucmath::TimeSeries::isChanged(bool leaveChanged)
{
    return hist.isChanged(leaveChanged);
}

double nucmath::TimeSeries::max()
{
    double maxVal = std::numeric_limits<double>::lowest();
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
    for(size_t i = 0; i < hist.nBins(); i++)
    {
        sum += hist.data(i).second / (entriesPerBin.data(i).second);
    }

    return sum / hist.nBins();
}

bool nucmath::TimeSeries::save(const std::string& path) const
{
    std::ofstream stream;
    stream.open(path);

    if(stream.is_open())
    {
        for(size_t i = 0; i < nBins(); i++)
        {
            const auto& [x, y] = data(i);
            stream << nucmath::to_string(x, 10) << "\t" << y << std::endl;
        }
    }
    else
    {
        std::cout << "Hist::save: can't open " << path << std::endl;
        return false;
    }

    stream.close();

    return true;
}
