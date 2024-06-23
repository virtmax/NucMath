
#include "minimizer.h"

using namespace nucmath;

Minimizer::Minimizer()
{
    const auto startseed = std::chrono::system_clock::now().time_since_epoch().count();
    rand_gen.seed(static_cast<unsigned int>(startseed));

    minMode = MinimizerMethode::DownhillSimplex;

    number_of_seed_points = 0;
}

Minimizer::~Minimizer()
{
}

void Minimizer::setData(const std::vector<double>& inputs, const std::vector<double>& y)
{
    // create a new list with 1D vectors
    this->inputs.clear();
    for(const double& v : inputs)
    {
        nucmath::Vector<double> vec(1);
        vec(0) = v;
        this->inputs.push_back(vec);
    }

    this->y = y;
}

void Minimizer::setData(const std::vector<nucmath::Vector<double>>& inputs, const std::vector<double>& y)
{
    this->inputs = inputs;
    this->y = y;
}

void Minimizer::setWeights(const std::vector<double>& weights)
{
    this->weights = weights;
}

void Minimizer::setModelFunction(MODELFUNC& modelFunction)
{
    func2min = [&](const std::vector<double>& p)
    {
        double chi2result = 0.0;
        const size_t len = y.size();
        for(size_t i = 0; i < len; i++)
        {
            const double delta_y = modelFunction(p, inputs.at(i)) - y[i];
            const double delta_y2 = delta_y * delta_y;

            if(weights.size() > 0)
                chi2result += weights.at(i) * delta_y2;
            //else if(!nucmath::isEqual(y[i], 0.0))
            //chi2result += delta_y2 / (y[i]*y[i]);
            else
                chi2result += delta_y2;
        }
        return chi2result / static_cast<double>(len);
    };
}

void Minimizer::setNumberOfSeedPoints(size_t number_of_seed_points)
{
    this->number_of_seed_points = number_of_seed_points;
}

void Minimizer::setInitialPointsAndConstrains(const std::vector<std::array<double, 3>>& initial_p)
{
    initial_points = initial_p;
}

void Minimizer::setParameterNames(const std::vector<std::string>& names)
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
        text += param_names[i] + "=(" + std::to_string(initial_points[i][1]) + " < x=" + std::to_string(initial_points[i][0]) + " < " +
                std::to_string(initial_points[i][2]) + ")\n";
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
    returnType = downhill_simplex_optimization(func2min, initial_points, result, tolerance, interations, number_of_seed_points);

    const size_t N = initial_points.size();

    // internal function used to set a proposed optimization point back into the constrained region.
    std::function<void(std::vector<double>&)> considerConstrains = [&](std::vector<double>& p)
    {
        for(size_t j = 0; j < N; j++)
        {
            if(p[j] > initial_points[j][2])
            {
                p[j] = initial_points[j][2];
            }

            if(p[j] < initial_points[j][1])
            {
                p[j] = initial_points[j][1];
            }
        }
    };

    //---------------------------------------------------
    // uncertainty estimation
    //
    // calculate function values around the optimal point for every fit parameter.
    // take the width of the quadratic regression to the points as the fit uncertainty.
    // chi2+1: 1 sigma, chi2+4: 2 sigma, chi2+9: 3 sigma
    // if the calculated value is even better than the optimal point, then replace it up to 3 times.
    //
    //---------------------------------------------------


    result_sigma.clear();
    result_sigma.resize(N, 0);

    const double varWidthStart = 0.01;
    const int nVarPoints = 11;   // Ungerade !, min 3
    size_t nMaxOptimalPointReadjustmests = 3;


    Regression regression;

    std::vector<std::vector<std::array<double, 2>>> variations;
    std::vector<double> varWidth;
    varWidth.resize(N, varWidthStart);

    OptPoint minPoint(result, func2min(result));
    bool stop = false;

    //std::cout <<"optimizer best fit: " << result[0] << " " << result[1] << std::endl;

    for(size_t l = 0; l < nMaxOptimalPointReadjustmests && stop == false; l++)
    {
        variations.clear();

        for(size_t j = 0; j < N; j++)   // for every parameter
        {
            std::vector<std::array<double, 2>> par_variations;

            // first, find a approx. width
            // y0 + ys = m*x + y0
            // x = ys/m
            double ys = 1;
            for(size_t i = 0; i < 10 && l == 0;)
            {
                OptPoint varP = minPoint;
                varP.p[j] = varP.p[j] * (1.0 + varWidth[j]);
                considerConstrains(varP.p);
                varP.fp = func2min(varP.p);

                if(std::abs(varP.fp - minPoint.fp) > ys)
                    varWidth[j] *= 0.6;
                else
                {
                    varWidth[j] *= 2.0;
                    par_variations.push_back({varP.p[j], varP.fp});
                    i++;
                }

                if(varP.fp < minPoint.fp)
                {
                    //std::cout << "replace: "<< minPoint.fp <<" durch "<< varP.fp <<std::endl;
                    minPoint = varP;
                }

                //std::cout << "{"<<varP.p[j] <<", "<< varP.fp - minPoint.fp<< "}" <<std::endl;
                //std::cout << "varWidth: " << varWidth[j] <<std::endl;
            }
            ////std::cout << "varWidth end: " << varWidth <<std::endl;

            // calculate new points around the best point. adjust every parameter in steps.
            par_variations.clear();

            for(int i = 0; i <= nVarPoints; i++)
            {
                OptPoint varP = minPoint;
                varP.p[j] = varP.p[j] - varWidth[j] + (2 * varWidth[j] / nVarPoints) * i;
                considerConstrains(varP.p);
                varP.fp = func2min(varP.p);

                par_variations.push_back({varP.p[j], varP.fp});

                //std::cout << "{"<<varP.p[j] <<", "<<varP.fp << "},";
            }
            //std::cout << std::endl;

            // minimum der regressionsparabel als möglichen besten punkt anschauen
            const auto& regres = regression.quadratic(par_variations);
            if(regres.c > 0)
            {
                const double xs = -regres.a1 / (2 * regres.a0);   // scheitelpunkt
                OptPoint varP = minPoint;
                varP.p[j] = xs;
                considerConstrains(varP.p);
                varP.fp = func2min(varP.p);
                if(varP.fp < minPoint.fp)
                {
                    minPoint = varP;
                    stop = false;
                }

                varWidth[j] = sqrt(ys / regres.a2);
            }

            variations.push_back(par_variations);
        }
    }

    // Calculate sigma
    for(size_t j = 0; j < N; j++)
    {
        const auto& regres = regression.quadratic(variations.at(j));
        if(regres.c > 0)
        {
            // sigma result = (chi2_for_sigma), e.g. for 3 sigma -> chi2_for_sigma=9
            const double chi2_for_sigma = 1;
            result_sigma[j] = sqrt(chi2_for_sigma / regres.a2);
        }
        else
        {
            // TODO: can't calculate sigma with this method
        }
    }

    return func2min(result);
}
