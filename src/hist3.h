
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <limits>
#include "datatable.h"

#pragma once

namespace nucmath
{

class Hist3
{
public:
    Hist3();
    Hist3(double startPosX, double startPosY, double startPosZ,
          double binWidth, double binHeight, double binDepth);

    void init(double startPosX, double startPosY, double startPosZ,
              double binWidth, double binHeight, double binDepth);

    bool add(double x, double y, double z, double c=1.0, bool expand=true);

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


 //   std::tuple<double, double, double, double> data(size_t bin) const;
//    double data(size_t binX, size_t binY) const;

    std::map<std::tuple<double, double, double>, double>::iterator begin() { return histmap.begin(); }
    std::map<std::tuple<double, double, double>, double>::iterator end() { return histmap.end(); }

    bool isChanged(bool leaveChanged);

    std::tuple<double, double, double, double> binToData(std::map<std::tuple<double, double, double>, double>::iterator it) const;

//    std::pair<double, double> getRangeX() const;
//    std::pair<double, double> getRangeY() const;
    std::tuple<double, double, double, double, double, double> getRange() const;

    double getBinWidth() const { return binWidth;}
    double getBinHeight() const { return binHeight;}
    double getBinDepth() const { return binDepth;}

//    enum FileFormat {List, Array};

//    bool save(const std::string& path, FileFormat fileFormat) const;

private:

    //std::vector<std::vector<std::vector<double>>> field;     //! Histogram data
    double binWidth, binHeight, binDepth;       //! The width of a bin.
    std::map<std::tuple<double, double, double>, double> histmap;


    /**
     * @brief The x/y-coordinate of the first bin.
     *
     * Its necessary to avoid big m_data size, if the histogram is starting with big values.
     */
    double startPosX, startPosY, startPosZ;
    double endPosX, endPosY, endPosZ;

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
