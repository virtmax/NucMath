#ifndef MINIMIZER_RANDOM_H
#define MINIMIZER_RANDOM_H

#include <random>
#include <array>

template <size_t N>
double random_optimization(std::function<double (const std::array<double, N> &)> chi2func,
                           const std::array<double, N> &initial_p,
                           const std::array<std::array<double, 2>, N> &constrains,
                           std::array<double, N> &result_p,
                           std::array<double, N> &result_sigma,
                           size_t max_interations)
{

    std::array<double, N> best_p = initial_p;
    std::array<double, N> p = initial_p;
    double min_chi2 = 1e300;

    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rand_gen(seed1);

    std::array<double, N> sigma;
    double cur_sigmaDiff = 4;   // sigma = sqrt(9) = 3
    const double sigmaDiff = 1;
    for (size_t i = 0; i < N; i++)
    {
        sigma[i] = (constrains[i][1] - constrains[i][0]) / 4.0;
    }

    size_t k = 0;
    while (k++ < max_interations)
    {

        for (size_t i = 0; i < N; i++)
        {
            do
            {
                std::normal_distribution<double> ndistr(best_p[i], sigma[i] * 5);
                p[i] = ndistr(rand_gen);

                //   std::cout << "p: A="<<p[0] << " mu="<<p[1] << " sigma=" << p[2] << std::endl;
            } while (p[i] > constrains[i][1] || p[i] < constrains[i][0]);
        }


        const double new_chi2 = chi2func(p);

        // sigma-wert an an die vorgegebene sigma-umgebung sigmaDiff anpassen
        /*    const double t = fabs(new_chi2 - (min_chi2 + sigmaDiff));
        if (t < cur_sigmaDiff)
        {
            cur_sigmaDiff = t;
            for (size_t i = 0; i < N; i++)
            {
                sigma[i] = fabs(best_p[i] - p[i]);
            }
        }*/



        if (min_chi2 > new_chi2)
        {
            for (size_t i = 0; i < N; i++)
            {
                sigma[i] = fabs(best_p[i] - p[i]);
            }

            min_chi2 = new_chi2;
            best_p = p;

            cur_sigmaDiff = sigmaDiff;



            //  std::cout << "p: A="<<p[0] << " mu="<<p[1] << " sigma=" << p[2] << std::endl;
        }
    }


    for (size_t i = 0; i < N; i++)
    {
        result_sigma[i] = sigma[i];
    }
    result_p = best_p;

    return min_chi2;
}

#endif // MINIMIZER_RANDOM_H
