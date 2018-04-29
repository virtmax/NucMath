



#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include "datatable.h"

#pragma once

namespace nucmath
{

class Hist2
{
public:
    Hist2();
    Hist2(double startPosX, double startPosY, double binWidthX, double binWidthY, size_t nBinsX = 100, size_t nBinsY = 100);

    void init(double startPosX, double startPosY, double binWidthX, double binWidthY, size_t nBinsX = 100, size_t nBinsY = 100);

    bool add(double x, double y, double z=1.0, bool expand=true);

    void clear();

    /**
     * @brief Return the number of bins of the histogram.
     *
     * @return Number of bins.
     */
    size_t nBins() const;

    /**
     * @brief Set internal name. Used for error messages.
     * @param name
     */
    void setName(const std::string &name);


    /**
     * @brief Get internal name.
     */
    std::string getName() const { return name; }

    std::tuple<double, double, double> data(size_t bin);

    bool isChanged(bool leaveChanged);

private:
    std::vector<std::vector<double>> field;     //! Histogram data
    double binWidthX, binWidthY;                //! The width of on bin.


    /**
     * @brief The x/y-coordinate of the first bin.
     *
     * Its necessary to avoid big m_data size, if the histogram is starting with big values.
     */
    double startPosX, startPosY;

    bool initialized;

    /**
     * @brief name of the histrogram.
     */
    std::string name;

    /**
     * @brief status about the changing of the data stored in the histogram
     */
    bool changed;
};

}

