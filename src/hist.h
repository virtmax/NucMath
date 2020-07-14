/**

    @brief Histogram class

    @author 

*/

#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include "datatable.h"
#include "utils.h"
#include "functions.h"

#pragma once

namespace nucmath
{

class Hist
{
public:
    Hist();
    Hist(const Hist& hist);
    Hist(double lowerEdge, double binWidth, size_t nBins);
    ~Hist();


    /**
     * @brief Initialize the histogram
     * @param lowerEdge The value of the lowest value included in the histogram. Left edge of the first bin.
     * @param binWidth
     * @param numberOfBins
     */
    void init(double lowerEdge, double binWidth, size_t nBins);


    /**
     * @brief Add an entry to the histogram.
     *
     *
     * @param x The value that should be added.
     * @param y The amount of the value x that should be added to the histogram.
     * @param expand Have to be true, if the histogram should add new bins, if x is bigger than histogram size.
     */
    bool add(double x, double y=1.0, bool expand=true);


    /**
     * @brief Create a histogram from a data table
     * @param datatable
     * @param column
     * @param columnData
     * @param xWidth
     * @param binWidth
     */
    bool create(DataTable &datatable, size_t column, double binWidth);


    /**
     *  @brief: Create a Histogram from a histogram contained inside the DataTable container.
     *
     */
    bool create(DataTable &datatable, size_t xColumn, size_t yColumn, double xWidth, double binWidth);


    /**
     * @brief Set internal name. Used for error messages.
     * @param name
     */
    void setName(const std::string &name);


    /**
     * @brief Get internal name.
     */
    std::string getName() const { return name; }


    /**
     * @brief Fold the histogram with a normal distribution.
     * @param folded
     * @param sigmaResolution
     */
    void fold(Hist& folded, double sigmaResolution);


    /**
     * @brief Set new bin width.
     * @param width
     * @return
     */
    bool setBinWidth(double width);



	double getBinWidth() const;


    std::pair<double, double> getRangeOfInsertedData() const;

    double getLowestEdge() const;
    double getHighestEdge() const;

    /**
     * @brief Return the number of bins of the histogram.
     *
     * @return Number of bins.
     */
    size_t nBins() const;

    Hist& operator=(const Hist & hist);


    /**
     * @brief Return the first bin (left border) and the last bin (right border)
     *          that corresponds to a value that is greater equal to the threshold value.
     *
     * @param threshold
     * @param left_border
     * @param right_border
     */
    void getThresholdBorders(double threshold, size_t& left_border, size_t& right_border) const;


    bool sumUp(const Hist& hist);


    /**
     * @brief Get maximal value stored in a bin.
     * @return
     */
	double max();


    /**
     * @brief Get minimal value stored in a bin.
     * @return
     */
	double min();


    /**
     * @brief Get the sum of all values in the histogram.
     * @return
     */
    double sum() const;


    /**
     * @brief Get the mean value of all bins.
     * @return
     */
    double mean() const;
    double mean(double start, double end) const;

    /**
     * @brief Get the bin with the biggest value in the histogram.
     *        If the are many bins with the same value, the first one counts.
     * @return
     */
    size_t maxBin() const;


    size_t meanBin();

    double centerOfMass();


/*
    double mean_x() const;
*/

    double standardDeviation() const;
    double standardDeviation(double start, double end) const;


    /**
     * @brief Get status about the changing of the data stored in the histogram.
     * @return true, if the the histagram has changed since last isChanged-call, e.g. after the add-method was calling
     *         false, otherwise
     */
    bool isChanged(bool leaveChanged = false);

    /**
     * @brief Set changed status manually.
     * @see isChanged
     */
    void setChanged(bool changed = true);

    void truncateZeroBins();

	void clear();

    /**
    * @brief Get Cumulative Distribution Function
    *
    */
    nucmath::Hist getCFD();

    nucmath::Hist& normalizeArea();
    nucmath::Hist& normalizeToMax(double max);

    bool isSet(double x) const;
    const std::vector<bool>& getIsSetData() const;

    size_t bin(double x) const;

    void fill(double x);

    std::vector<double>& data();
    const std::vector<double>& data() const;
    std::pair<double, double> data(size_t bin) const;

    std::pair<double, double> dataFromX(double x) const;

    Hist operator* (const double factor) const;

    std::pair<double, double> getRange() const;

    std::vector<double>::const_iterator begin() const { return field.begin(); }
    std::vector<double>::iterator begin() { return field.begin(); }

    std::vector<double>::const_iterator end() const { return field.end(); }
    std::vector<double>::iterator end() { return field.end(); }

    const Hist& getCopy() const { return *this; }
    void setCopy(const Hist &hist);

    void load(const std::string& path);
    void save(const std::string& path) const;

    Hist getWithNewBinning(double binning);

    Hist getUnfolded(double sigma);

private:

    std::vector<double> field;     //! Histogram data
    double binWidth;             //! The width of on bin.

    std::vector<bool> isSetData;

    /**
     * @brief The smallest value on the x-axis (the x-coordinate of the first bin).
     *
     * Its necessary to avoid big m_data size, if the histogram is starting with big values.
     */
    double lowerEdge;

    double hRangeL;
    double hRangeR;

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
