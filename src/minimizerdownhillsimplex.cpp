
#include "minimizerdownhillsimplex.h"


double nucmath::downhill_simplex_optimization(FUNC2MIN opt_func,
                                     const std::vector< std::array<double,3> > &initial_p,
                                     std::vector<double> &result_p,
                                     std::vector<double> &result_sigma,
                                     double tolerance,
                                     size_t max_interations,
                                     size_t number_of_seed_points
                                     )
{
    const size_t N = initial_p.size();
    const size_t NP = N + 1;
    const double a = 1.0, b = 1.8, c = 0.55, d = 0.5;
    const double double_max = std::numeric_limits<double>::max();


    // internal function used to set a proposed optimization point back into the constrained region.
    std::function<void (std::vector<double> &)> considerConstrains =
            [&](std::vector<double> &p)
    {
        for (size_t j = 0; j < N; j++)
        {
            if(p[j] > initial_p[j][2])
            {
                p[j] = initial_p[j][2];
            }
            if(p[j] < initial_p[j][1])
            {
                p[j] = initial_p[j][1];
            }
        }
    };


    std::vector<optPoint> trusted_region_points;    // only points with "fp < bestpoint.fp+9"
    std::function<void (optPoint &)> saveTrustedPoints =
            [&](optPoint &p)
    {
        if(trusted_region_points.size()==0 || (p.fp < trusted_region_points.at(0).fp + 9))
        {
            trusted_region_points.push_back(p);
            std::sort(trusted_region_points.begin(), trusted_region_points.end());

            for(auto it = trusted_region_points.crbegin(); it != trusted_region_points.crend(); it++)
            {
                if((*it).fp > trusted_region_points.at(0).fp + 9)
                {
                    trusted_region_points.erase(trusted_region_points.begin());
                    it = trusted_region_points.crbegin();
                }
                else
                    break;
            }
        }
    };


    std::vector<optPoint> points;
    for(size_t i= 0; i < NP; i++)
        points.push_back(optPoint(N));

    result_p.clear();
    result_p.resize(N,0);
    result_sigma.clear();
    result_sigma.resize(N,0);

    optPoint x0(N);	// centroid
    optPoint xr(N);	// reflected point
    optPoint xe(N);	// expanded point
    optPoint xc(N);	// contracted point

    std::vector<double> start_p;
    for(size_t i= 0; i < initial_p.size(); i++)
    {
        start_p.push_back(initial_p[i][0]);
        result_sigma.push_back(-1);
    }


    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rand_gen(seed1);




    // Generate initial points

    // take the start point
    points[0].p = start_p;
    points[0].fp = opt_func(start_p);

    // create at least 5 points per parameter
    if(number_of_seed_points < NP*5)
        number_of_seed_points = NP*5;

    // create initial points from a flat distribution over the parameter space
    std::vector<double> random_point;
    random_point.resize(N);
    for(size_t k = 0; k < number_of_seed_points; k++)
    {
        for (size_t j = 0; j < N; j++)
        {
            std::uniform_real_distribution<double> flatdist(initial_p[j][1], initial_p[j][2]);
            random_point[j] = flatdist(rand_gen);
        }

        const double temp_fp = opt_func(random_point);
        if(points[NP-1].fp > temp_fp)
        {
            points[NP-1].p = random_point;
            points[NP-1].fp = temp_fp;
            std::sort(points.begin(), points.end());
        }
    }



    bool done = false;
    size_t loop_counter = 0;
    while (done == false)
    {
        std::sort(points.begin(), points.end());	// 1 2 3 4 5


        // calculate the centroid except the worst
        x0.p = std::vector<double>(N,0);
        for (size_t i = 0; i < N; i++)
        {
            for (size_t p = 0; p < N; p++)
            {
                x0.p[i] += points[p].p[i];
            }
            x0.p[i] /= static_cast<double>(N);
        }


        // calculate reflected point xr = x0 + a(x0-xWorst)
        for (size_t j = 0; j < N; j++)
        {
            xr.p[j] = x0.p[j] + a*(x0.p[j] - points[N].p[j]);
        }
        considerConstrains(xr.p);
        xr.fp = opt_func(xr.p);


        if (xr.fp < points[0].fp)	// reflected point is the best one -> expansion
        {
            // calculate expaned point
            for (size_t j = 0; j < N; j++)
            {
                xe.p[j] = x0.p[j] + b*(xr.p[j] - x0.p[j]);
            }
            considerConstrains(xe.p);
            xe.fp = opt_func(xe.p);

            // replace the worst point with a better one and repeat the algorithm
            if (xe.fp < xr.fp)
                points[N] = xe;
            else
                points[N] = xr;
        }
        else if (points[0].fp <= xr.fp && xr.fp < points[N-1].fp) // right direction -> repeat
        {
            points[N] = xr;
        }
        else if (xr.fp >= points[N-1].fp)
        {
            // calculate contracted point  xc = x0 + a(x0-xWorst)
            for (size_t j = 0; j < N; j++)
            {
                xc.p[j] = x0.p[j] + c*(points[N].p[j] - x0.p[j]);
            }
            considerConstrains(xc.p);
            xc.fp = opt_func(xc.p);


            if (xc.fp < points[N].fp)
            {
                points[N] = xc;
            }
            else // -> shrink
            {
                for (size_t i = 1; i < NP; i++)
                {
                    for (size_t j = 0; j < N; j++)
                    {
                        points[i].p[j] = points[0].p[j] + d*(points[i].p[j] - points[0].p[j]);
                    }
					considerConstrains(points[i].p);
                    points[i].fp = opt_func(points[i].p);
                }
            }
        }




        // Abbruchbedingung
        loop_counter++;
        if (loop_counter == max_interations)
            done = true;


        if(tolerance > 0)
        {

            // Abbruchkriterium auf Toleranz
            double fbar = 0;
            for (size_t i = 0; i < N; i++)
            {
                fbar += points[i].fp;
            }
            fbar /= (double)(N);

            double dhs_tol = 0;
            for (size_t i = 0; i < N; i++)
            {
                dhs_tol += (points[i].fp-fbar)*(points[i].fp-fbar);
            }
            dhs_tol /=  (double)(N);

            if(sqrt(dhs_tol) < tolerance)
            {
                done = true;
            }
        }
    }



    //////////////////////////
    // uncertainty estimation
    //////////////////////////

    std::sort(points.begin(), points.end());	// aufsteigend 1 2 3 4


    const double var_step = 0.01;   // Faktor um den der beste Punkt variiert werden soll (0.01 = 1%)
    const int var_points = 11;       // Ungerade !, min 3
    size_t readjust_bestPoint_max = 3;


	Regression regression;

    std::vector< std::vector< std::array<double, 2> > > variations;
    optPoint actualBestPoint = points[0];   // den besten Punkt als Variantionspunkt wählen
    bool stop = false;

    for (size_t l = 0; l < readjust_bestPoint_max && stop == false; l++)
    {
        optPoint variationPoint = actualBestPoint;
        variations.clear();
        bool positive_par_a = false;


        for (size_t j = 0; j < N; j++)  // for every parameter
        {
            std::vector< std::array<double, 2> > par_variations;

            // calculate new points around the best point. adjust every parameter in steps.
            for (int i = -(var_points-1)/2; i < (var_points-1)/2; i++)
            {
                optPoint varP = variationPoint;
                varP.p[j] = varP.p[j]*(1.0 + var_step*i);
                considerConstrains(varP.p);
                varP.fp = opt_func(varP.p);

                par_variations.push_back({varP.p[j], varP.fp});

//                std::cout << "{"<<varP.p[j] <<", "<<varP.fp << "},";
            }
//            std::cout << std::endl;

            // minimum der regressionsparabel als möglichen besten punkt anschauen
            double a,b,c;   // zu bestimmende parametriesierung der parabel
			regression.quadratic(par_variations, a, b, c);
            if(a > 0)
            {
                const double xs = -b/(2*a);         // scheitelpunkt
                variationPoint.p[j] = xs;
                considerConstrains(variationPoint.p);
                variationPoint.fp = opt_func(variationPoint.p);
                if(variationPoint.fp < actualBestPoint.fp && variationPoint.p[j] < initial_p[j][2] && variationPoint.p[j] > initial_p[j][1])
                {
                    actualBestPoint = variationPoint;
                }
            }
            else
            {
                positive_par_a = true;

                for(auto &par : par_variations)
                {
                    if(par[1] < actualBestPoint.fp)
                    {
                        actualBestPoint.fp = par[1];
                        actualBestPoint.p[j] = par[0];
                    }
                }
                //break;
            }


            variations.push_back(par_variations);
        }

        if(positive_par_a)
        {
            stop = false;
//            std::cout << " a < 0. Wiederhole. " << std::endl;
        }
        else
        {
           stop = true;
        }
    }



    // Calculate sigma
    for (size_t j = 0; j < N; j++)
    {
        // Eine quadratiche Funktion an die Datenpunkte anpassen
        double a,b,c;
		regression.quadratic(variations.at(j), a, b, c);
//        std::cout << "a=" << a << std::endl<< "b=" << b << std::endl<< "c=" << c << std::endl;

        if(a > 0)   // parabel nach oben geöffnet
        {

            // 1 für sqrt(1)=1 sigma abweichung, 4 für sqrt(4)=2 sigma, 9 für sqrt(9)=3 sigma
            const double sigma = 1;
            // double x1 = xs - sqrt(sigma/a);
            result_sigma[j] = sqrt(sigma/a);
        }
        else
        {
//            std::cout << "Parabel nach unten geöffnet. a < 0. Unsicherheit kann nicht berechnet werden. " << std::endl;
        }
    }


    result_p = actualBestPoint.p;

    return points[0].fp;
}
