
#pragma once

#include <algorithm>
#include <limits>
#include <sstream>
#include <vector>


namespace nucmath
{

/**
 *  Definition of a row of the DataTable class
 */
class TableRow
{
public:
    TableRow(size_t columns, double defaultValue = 0.0);
    TableRow(const std::vector<double>& row);
    TableRow(const TableRow& row);

    void reset();

    void setNumOfColumns(size_t columns, double defaultValue = 0.0);
    size_t getNumOfColumns() const { return nColumns; }

    /**
     * @brief Return the row values as a string.
     * @return
     */
    std::string str() const;

    /**
     * @brief multiply all elements inside the row with a @param factor.
     * @param factor
     * @return
     */
    TableRow operator*(double factor) const;
    TableRow& operator+=(double shift);
    const double& operator[](size_t column) const;
    double& operator[](size_t column);

    double getMax();
    double getMin();


private:
    size_t nColumns;

    double min;
    double max;

    bool changed;

    friend class DataTable;
    std::vector<double> data;

    void updateStatistics();
};

}
