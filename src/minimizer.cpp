
#include "minimizer.h"

using namespace nucmath;

Minimizer::Minimizer()
{
    rand_gen.seed(std::chrono::system_clock::now().time_since_epoch().count());

    minMode = MinimizerMethode::DownhillSimplex;

    number_of_seed_points = 0;
}

Minimizer::~Minimizer()
{

}

void Minimizer::setData(const std::vector<double> &inputs, const std::vector<double> &y)
{
    // create a new list with 1D vectors
    std::vector<nucmath::Vector<double>> inptVecList;
    this->inputs.clear();
    for(const double& v : inputs)
    {
        nucmath::Vector<double> vec(1);
        vec(0) = v;
        this->inputs.push_back(vec);
    }

    this->y = y;
}

void Minimizer::setData(const std::vector<nucmath::Vector<double>> &inputs, const std::vector<double> &y)
{
    this->inputs = inputs;
    this->y = y;
}

void Minimizer::setWeights(const std::vector<double> &weights)
{
    this->weights = weights;
}

void Minimizer::setModelFunction(MODELFUNC &modelFunction)
{
    func2min = [&](const std::vector<double> &p)
    {
        double chi2result = 0.0;
        const size_t len = y.size();
        for(size_t i = 0; i < len; i++)
        {
            const double delta_y = modelFunction(p, inputs.at(i)) - y[i];

            if(weights.size() > 0)
                chi2result += weights.at(i)*delta_y*delta_y;
            else if(y[i] > 0)
                chi2result += delta_y*delta_y/y[i];
            else
                chi2result += delta_y*delta_y;

        }
        return chi2result/static_cast<double>(len);
    };
}

void Minimizer::setNumberOfSeedPoints(size_t number_of_seed_points)
{
    this->number_of_seed_points = number_of_seed_points;
}

void Minimizer::setInitialPointsAndConstrains(const std::vector< std::array<double,3> > &initial_p)
{
    initial_points = initial_p;
}

void Minimizer::setParameterNames(const std::vector<std::string> &names)
{
    param_names = names;
}

std::string Minimizer::getFormatedInitialPoints()
{
    std::string text = "initial points:\n";

    if(initial_points.size() != param_names.size())
    {
        text += "warning: number of initial points != number of parameter names.\n";
    }

    for(size_t i = 0; i < param_names.size() && i < initial_points.size(); i++)
    {
        text += param_names[i] + "=" + std::to_string(initial_points[i][0]) + "\n";
    }

    return text;
}

std::string Minimizer::getFormatedConstrains()
{
    std::string text = "minimizer constrains:\n";

    if(initial_points.size() != param_names.size())
    {
        text += "warning: number of constrains points != number of parameter names.\n";
    }

    for(size_t i = 0; i < param_names.size() && i < initial_points.size(); i++)
    {
        text += param_names[i] + "=" + std::to_string(initial_points[i][1]) + " - " + std::to_string(initial_points[i][2]) + "\n";
    }

    return text;
}

std::string Minimizer::getFormatedInitialPointsAndConstrains()
{
    std::string text = "initial points with constrains:\n";

    if(result.size() != param_names.size())
    {
        text += "warning: number of parameters != number of parameter names.\n";
    }

    for(size_t i = 0; i < param_names.size() && i < initial_points.size(); i++)
    {
        text += param_names[i] + "=(" + std::to_string(initial_points[i][1]) + " < x=" + std::to_string(initial_points[i][0]) + " < " + std::to_string(initial_points[i][2]) + ")\n";
    }

    return text;
}

std::string Minimizer::getFormatedFitResults()
{
    std::string text = "fit results:\n";

    if(result.size() != param_names.size())
    {
        text += "warning: number of parameters != number of parameter names.\n";
    }

    for(size_t i = 0; i < param_names.size() && i < result.size(); i++)
    {
        text += param_names[i] + "=" + std::to_string(result[i]) + "±" + std::to_string(result_sigma[i]) + "\n";
    }

    return text;
}

double Minimizer::findFit(size_t interations, double tolerance)
{
    /*
    func2min = [&](const std::vector<double> &p)
    {
        double chi2result = 0.0;
        const size_t len = y.size();
        for (size_t i = 0; i < len; i++)
        {
            const double delta_y = modelFunction(p, x[i]) - y[i];
            if(y[i] > 0)
                chi2result += delta_y*delta_y/y[i];
            else
                chi2result += delta_y*delta_y;

        }
        return chi2result/((double)len);
    };
    */

    return downhill_simplex_optimization(func2min,initial_points, result, result_sigma, tolerance, interations, number_of_seed_points);
}
