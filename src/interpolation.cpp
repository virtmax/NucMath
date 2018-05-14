

#include "interpolation.h"


using namespace nucmath;


Interpolation::Interpolation()
{
    m_data.reserve(1000);
}

Interpolation::~Interpolation()
{

}


std::vector<std::pair<double, double>> Interpolation::LinearInterpolation()
{
    // Liste mit m und b für die lineare Gleichung zwischen zwei Punkten
    // f(x) = m*x + b
    std::vector<std::pair<double, double>> param_list;

    for (size_t i = 0; i < m_data.size()-1; i++)
    {
        double m = (m_data[i + 1].second - m_data[i].second) / (m_data[i + 1].first - m_data[i].first);

        param_list.push_back(std::pair<double, double>(m_data[i].second, m));
    }

    return param_list;
}


std::vector<std::pair<double, double>> Interpolation::SteffenInterpolation()
{
    // Liste mit m und b für die lineare Gleichung zwischen zwei Punkten
    // f(x) = m*x + b
    std::vector<std::pair<double, double>> param_list;

    for (size_t i = 0; i < m_data.size() - 1; i++)
    {
        double m = (m_data[i + 1].second - m_data[i].second) / (m_data[i + 1].first - m_data[i].first);

        param_list.push_back(std::pair<double, double>(m_data[i].second, m));
    }

    return param_list;
}
