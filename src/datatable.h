/*
    @brief Container for tables with double values


 */

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <regex>
#include <array>
#include <cmath>
#include <limits>
#include <math.h>


#include "constants.h"
#include "utils.h"
#include "integration.h"
#include "stringoperations.h"

#include "tablerow.h"

#pragma once

namespace nucmath
{

enum class DATATABLEFORMAT {_Unknown, Standard, MathematicaTable2};

class ColumnProperties
{
public:
    std::string title;

    double binWidth;

    double min;
    double max;
};


class DataTable
{
private:

    size_t nColumns;

    std::vector<TableRow> m_dataTableOriginal;
    std::vector<TableRow> m_dataTable;
    std::vector<ColumnProperties> m_columnProp;

    DATATABLEFORMAT format;
public:
    DataTable(size_t columns = 2);
    DataTable(const std::vector<TableRow>& dataTable);
    ~DataTable();

    bool load(const std::string &file);
    bool save(const std::string &file) const;

    void addRow(const TableRow& row);
    void addRow(const std::vector<double> &row);

    void mul(double factor, size_t column);
    void add(double shift, size_t column);


    void setNumOfColumns(size_t columns, double defaultValue = 0.0);
    void setNumOfRows(size_t rows, double defaultValue = 0.0);
    void reset();

    void setHeader(const std::string &str);

    // Faltet die Daten mit der Normalverteilung
    void fold(DataTable& folded, float resolution, size_t column) const;

    double calcSumOfSquares(const DataTable& hist, size_t column) const;
    double calcSumOfSquares(const DataTable& hist, size_t column, double beginCh, double endCh) const;

    double getMax(size_t column) const;
    double getMin(size_t column) const;

    void calcMaxMin();
    void calcMaxMin(size_t column);

    void sort(size_t column);

    double getSum(size_t column);

    size_t getNumberOfColumns() const { return nColumns; }
    size_t getNumberOfRows() const { return m_dataTable.size(); }

    const TableRow& operator[](size_t row) const;
    TableRow& operator[](size_t row);

public:
    const std::vector<TableRow>& getData() const { return m_dataTable; }
    std::vector<TableRow>& getData() { return m_dataTable; }

private:

    void updateStatistics(size_t newRowIndex);

    /**
         * @brief Comparator for the std::sort function.
         */
    struct sort_comparator
    {
        sort_comparator(size_t column){ this->column = column; }
        bool operator()(const TableRow &a, const TableRow &b) { return (a.data[column] < b.data[column]); }
        size_t column;
    };

    bool changed;

    std::string header;
};

}
