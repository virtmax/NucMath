#pragma once


namespace nucmath
{

class SciVal
{
public:
    SciVal(double value, double sigma);
    ~SciVal();


private:
    double value;
    double sigma;
};

}
