/*!

    @brief Histogram class

    @author 

  */

#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include "datatable.h"

#pragma once

namespace nucmath
{

//#define DEBUG_HIST_MSG_OUTPUT 1

class Hist
{
public:
    Hist();
    virtual ~Hist();

    /**
     * @brief Initialize the histogram
     * @param startPosition The value of the first histogram bin.
     * @param binWidth
     * @param numberOfBins
     */
    void init(double startPosition, double binWidth, size_t numberOfBins = 100);



    /**
     * @brief Add an entry to the histogram over direct access to the bin.
     *
     *
     * @param x The bin (or x-position) on which the value should be added.
     * @param y The value that should be added to the histogram.
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
    void create(DataTable &datatable, size_t column, size_t columnData, double xWidth, double binWidth);


    /**
     * @brief Set internal name. Used for error messages.
     * @param name
     */
    void setName(const std::string &name);


    void fold(Hist& folded,double resolution_sigma);


	double getStartX() const;
	double getBinWidth() const;

    /**
     * @brief Return the number of bins in the histogram.
     *
     * @return Number of bins.
     */
	size_t numberOfBins() const;

    Hist& operator=(const Hist & hist2d);

    void getThresholdBorders(double threshold, size_t& left_border, size_t& right_border) const;

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
     * @brief Get the bin with the biggest value in the histogram.
     *        If the are many bins with the same value, the first one counts.
     * @return
     */
    size_t max_bin();


    size_t mean_bin();


    double mean_x() const;

    double standardDeviation() const;


    /**
     * @brief Get status about the changing of the data stored in the histogram.
     * @return true, if the the histagram has changed since last isChanged-call, e.g. after the add-method was calling
     *         false, otherwise
     */
    bool isChanged();

    void truncateZeroBins();

	void clear();


	std::vector<int>& data();
	const std::vector<int>& data() const;
    std::pair<double,double> data(std::size_t bin) const;

private:

    std::vector<int> m_data;     //! Histogram data
    double m_binWidth;              //! The width of on bin.
 //   size_t startBin;

    /**
     * @brief The smallest value on the x-axis (the x-coordinate of the first bin).
     *
     * Its necessary to avoid big m_data size, if the histogram is starting with big values.
     */
    double m_startX;

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
