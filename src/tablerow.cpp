
#include "tablerow.h"


nucmath::TableRow::TableRow(size_t nColumns, double defaultValue)
{
    reset();
    this->nColumns = nColumns;
    data.assign(nColumns, defaultValue);
}

nucmath::TableRow::TableRow(const std::vector<double> &row)
{
    reset();
    this->data = row;
    nColumns = row.size();
}

nucmath::TableRow::TableRow(const nucmath::TableRow& row)
{
    reset();
    this->data = row.data;
    nColumns = this->data.size();
}

void nucmath::TableRow::reset()
{
    min = std::numeric_limits<double>::min();
    max = std::numeric_limits<double>::max();
    data.clear();
    changed = true;
}

void nucmath::TableRow::setNumOfColumns(size_t columns, double defaultValue)
{
    nColumns = columns;
    data.assign(columns, defaultValue);
    changed = true;
}

std::string nucmath::TableRow::str() const
{
    std::string s = "";
    for(size_t i = 0; i < nColumns; i++)
    {
        s.append(std::to_string(data[i]));

        if(i+1 != nColumns)
            s.append("\t");
    }
    return s;
}

nucmath::TableRow nucmath::TableRow::operator*(double factor) const
{
    TableRow result(nColumns);
    for(size_t i = 0; i < nColumns; i++)
    {
        result.data[i] = data[i] * factor;
    }
    return result;
}

nucmath::TableRow& nucmath::TableRow::operator+=(double shift)
{
    for(size_t i = 0; i < nColumns; i++)
    {
        data[i] += shift;
    }

    return *this;
}

const double& nucmath::TableRow::operator[](size_t column) const
{
    if(column >= nColumns)
        throw std::invalid_argument("TableRow::operator[]: index >= columns");

    return data[column];
}

double& nucmath::TableRow::operator[](size_t column)
{
    changed = true;

    if(column >= nColumns)
        throw std::invalid_argument("TableRow::operator[]: index >= columns");

    return data[column];
}

double nucmath::TableRow::getMax()
{
    if(changed)
        updateStatistics();
    return this->max;
}

double nucmath::TableRow::getMin()
{
    if(changed)
        updateStatistics();
    return this->min;
}

void nucmath::TableRow::updateStatistics()
{
    const auto& [minindex, maxindex] = std::minmax_element(data.begin(), data.end());
            this->min = *minindex;
            this->max = *maxindex;

            changed = false;
}
