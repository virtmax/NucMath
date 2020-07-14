
#include "minimizerdownhillsimplex.h"


OPTIMIZER_RETURN_TYPE nucmath::downhill_simplex_optimization(FUNC2MIN opt_func,
                                     const std::vector< std::array<double,3> > &initial_p,
                                     std::vector<double> &result_p,
                                     double tolerance,
                                     size_t max_interations,
                                     size_t number_of_seed_points
                                     )
{
    const size_t N = initial_p.size();
    const size_t NP = N + 1;
    const double a = 1.0, b = 2.0, c = 0.5, d = 0.5;
    //const double double_max = std::numeric_limits<double>::max();
    OPTIMIZER_RETURN_TYPE ret;


    // internal function used to set a proposed optimization point back into the constrained region.
    std::function<void (std::vector<double> &)> considerConstrains =
            [&](std::vector<double> &p)
    {
        for(size_t j = 0; j < N; j++)
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

/*
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
*/

    std::vector<optPoint> points;
    for(size_t i= 0; i < NP; i++)
        points.push_back(optPoint(N));

    result_p.clear();
    result_p.resize(N, 0);

    optPoint x0(N); // centroid
    optPoint xr(N);	// reflected point
    optPoint xe(N);	// expanded point
    optPoint xc(N);	// contracted point

    std::vector<double> start_p;
    for(size_t i= 0; i < initial_p.size(); i++)
    {
        start_p.push_back(initial_p[i][0]);
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

    // create initial points on a grid over the parameter space
    std::vector<double> initial_points;
    initial_points.resize(N);
    for(size_t k = 0; k < number_of_seed_points; k++)
    {
        for(size_t j = 0; j < N; j++)
        {
            std::uniform_real_distribution<double> flatdist(initial_p[j][1], initial_p[j][2]);
            initial_points[j] = flatdist(rand_gen);
        }

        const double temp_fp = opt_func(initial_points);
        if(points[NP-1].fp > temp_fp)
        {
            points[NP-1].p = initial_points;
            points[NP-1].fp = temp_fp;
            std::sort(points.begin(), points.end());
        }
    }


    bool done = false;
    size_t loop_counter = 0;
    while(!done)
    {
        std::sort(points.begin(), points.end());	// 1 2 3 4 5

        // calculate the centroid except the worst
        x0.p = std::vector<double>(N, 0);
        for(size_t i = 0; i < N; i++)
        {
            for(size_t p = 0; p < N; p++)
            {
                x0.p[i] += points[p].p[i];
            }
            x0.p[i] /= static_cast<double>(N);
        }


        // calculate reflected point xr = x0 + a(x0-xWorst)
        for(size_t j = 0; j < N; j++)
        {
            xr.p[j] = x0.p[j] + a*(x0.p[j] - points[NP-1].p[j]);
        }
        considerConstrains(xr.p);
        xr.fp = opt_func(xr.p);


        if(xr.fp < points[0].fp)	// reflected point is the best one -> expansion
        {
            // calculate expaned point
            for(size_t j = 0; j < N; j++)
            {
                xe.p[j] = x0.p[j] + b*(xr.p[j] - x0.p[j]);
            }
            considerConstrains(xe.p);
            xe.fp = opt_func(xe.p);

            // replace the worst point with a better one and repeat the algorithm
            if(xe.fp < xr.fp)
                points[N] = xe;
            else
                points[N] = xr;
        }
        else if(xr.fp < points[N].fp)     // good direction -> repeat
        {
            points[N] = xr;
        }
        else
        {
            if(xr.fp < points[N].fp)
                points[N] = xr;

            // calculate contracted point  xc = x0 + a(x0-xWorst)
            for (size_t j = 0; j < N; j++)
            {
                xc.p[j] = x0.p[j] + c*(points[N].p[j] - x0.p[j]);
            }
            considerConstrains(xc.p);
            xc.fp = opt_func(xc.p);


            if(xc.fp < points[N].fp)
            {
                points[N] = xc;
            }
            else // -> shrink
            {
                for(size_t i = 1; i < NP; i++)
                {
                    for(size_t j = 0; j < N; j++)
                    {
                        points[i].p[j] = points[0].p[j] + d*(points[i].p[j] - points[0].p[j]);
                    }
					considerConstrains(points[i].p);
                    points[i].fp = opt_func(points[i].p);
                }
            }
        }


        // search stop, if to many iterations
        loop_counter++;
        if(loop_counter == max_interations)
        {
            done = true;
            ret = OPTIMIZER_RETURN_TYPE::MaxIterations;
        }


        // Termination criterion for tolerance
        // tolerance > Standard deviation around the current best optimization point?
        double fbar = 0;
        for(size_t i = 0; i < N; i++)
        {
            fbar += points[i].fp;
        }
        fbar /= static_cast<double>(N);
        double dhs_tol = 0;
        for(size_t i = 0; i < N; i++)
        {
            dhs_tol += (points[i].fp-fbar)*(points[i].fp-fbar);
        }
        dhs_tol /= static_cast<double>(N);
        if(sqrt(dhs_tol) < tolerance)
        {
            done = true;
            ret = OPTIMIZER_RETURN_TYPE::ToleranceReached;
        }
    }

    result_p = points[0].p;

    return ret;
}
