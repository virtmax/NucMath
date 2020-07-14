



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

    optPoint(const optPoint& other)
        : p(other.p), fp(other.fp), N(other.N)
    {
    }

    optPoint(const std::vector<double> &p, double fp)
    {
        this->fp = fp;
        this->N = p.size();
        this->p = p;
    }

    ~optPoint()
    {
    }

    optPoint& operator=(const optPoint& other)
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
        return false;
    }

    std::vector<double> p   {{}};
    double fp   {std::numeric_limits<double>::max()};
    size_t N    {0};

private:
    optPoint()
    {
    }
};


