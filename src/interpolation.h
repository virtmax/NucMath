

#include "datatable.h"


#pragma once


namespace nucmath
{
enum InterpolationMode { Linear = 0, Steffen };

class Interpolation
{
public:
    Interpolation();
    virtual ~Interpolation();

    void interpolate(InterpolationMode mode);

    void setData(const DataTable& data);
    void setData(const std::vector<double>& data);

private:

    DataTable m_dataTable;
    std::vector<std::pair<double, double>> m_data;

    InterpolationMode m_mode;


    std::vector<std::pair<double,double>> LinearInterpolation();

    std::vector<std::pair<double,double>> SteffenInterpolation();
};

}
