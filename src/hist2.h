

#pragma once

#include "datatable.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>


namespace nucmath
{

class Hist2
{
public:
    Hist2();
    Hist2(double startPosX, double startPosY, double binWidth, double binHeight, size_t nBinsX = 100, size_t nBinsY = 100);

    void init(double startPosX, double startPosY, double binWidth, double binHeight, size_t nBinsX = 100, size_t nBinsY = 100);

    bool add(double x, double y, double z = 1.0, bool expand = true);

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
    void setName(const std::string& name);


    /**
     * @brief Get internal name.
     */
    std::string getName() const { return name; }

    /**
    * @brief Get the mean value of all row bins for a given column.
    * @return
    */
    double meanOverColumn(size_t column) const;

    /**
    * @brief Get the mean value of all column bins for a given row.
    * @return
    */
    double meanOverRow(size_t row) const;

    std::tuple<double, double, double> max() const;
    std::tuple<double, double, double> max(size_t column) const;

    double& dataref(size_t binX, size_t binY);
    const double& dataref(size_t binX, size_t binY) const;
    double& dataref(size_t bin);
    const double& dataref(size_t bin) const;

    std::vector<std::vector<double>>::iterator begin() { return field.begin(); }
    std::vector<std::vector<double>>::iterator end() { return field.end(); }

    std::tuple<double, double, double> data(size_t bin) const;
    std::tuple<double, double, double> data(size_t binX, size_t binY) const;

    bool isChanged(bool leaveChanged);

    std::pair<double, double> getRangeX() const;
    std::pair<double, double> getRangeY() const;
    std::tuple<double, double, double, double> getRange() const;

    double getBinWidth() const { return binWidth; }
    double getBinHeight() const { return binHeight; }

    size_t nBinsX() const;
    size_t nBinsY() const;

    enum FileFormat
    {
        List,
        Array
    };

    bool save(const std::string& path, FileFormat fileFormat) const;

private:
    std::vector<std::vector<double>> field;   //! Histogram data
    double binWidth, binHeight;               //! The width of on bin.


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
