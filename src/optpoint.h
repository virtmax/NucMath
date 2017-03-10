



#include <iostream>
#include <algorithm>
#include <array>
#include <chrono>

#include "minimizerdef.h"

#pragma once
#undef max
#undef min

class optPoint
{
public:
    optPoint(size_t N)
    {
        fp = std::numeric_limits<double>::max();
        this->N = N;
        p.resize(N);
    }

    ~optPoint()
    {
    }


    optPoint& operator=(optPoint other)
    {
        if(other.N != N)
            std::cout << "N!=N";

        fp = other.fp;
        N = other.N;
        p = other.p;
        return *this;
    }

    bool operator < (const optPoint &p2) const
    {
        if(p2.N == N)
            return this->fp < p2.fp;
        else std::cout<< "dsPoint::operator < (const dsPoint &p2): this->N != ps.N"<<std::endl;
    }

    std::vector<double> p;
    double fp;
    size_t N;

private:
    optPoint()
    {
        fp = std::numeric_limits<double>::max();
        this->N = 0;
    }
};


