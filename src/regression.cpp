

#include "regression.h"


nucmath::Regression::Regression()
{


}

nucmath::Regression::~Regression()
{


}

// ax + b
void nucmath::Regression::linear(const std::vector<std::array<double, 2>> &points, double &a, double&b, double &R2)
{
    const double N = points.size();

    if(N < 2)
    {
        std::cout << "linear_regression: at least 2 data points are needed. "<< std::endl;
    }

    double sum_y = 0.0, sum_x = 0.0;
    double sum_x_y = 0.0, sum_x2 = 0.0, sum_y2 = 0.0;

    for(size_t i = 0; i < N; i++)
    {
        sum_x +=    points[i][0];
        sum_y +=    points[i][1];
        sum_x2 +=   points[i][0]*points[i][0];
        sum_y2 +=   points[i][1]*points[i][1];
        sum_x_y+=   points[i][0]*points[i][1];
    }

    const double b_counter = (N*sum_x_y-sum_x*sum_y);
    const double b_denominator = (N*sum_x2 - sum_x*sum_x);
    if(b_denominator == 0) // TODO: ungenau
    {
        std::cout << "linear_regression: division by 0.";
        return;
    }
    b = b_counter/b_denominator;
    a = (sum_y - b*sum_x)/N;

    const double R2_counter = b_counter*b_counter;
    const double R2_denominator = (N*sum_x2 - sum_x*sum_x)*(N*sum_y2 - sum_y*sum_y);
    if(R2_denominator == 0) // TODO: ungenau
    {
        std::cout << "linear_regression: division by 0.";
        return;
    }
    R2 = R2_counter/R2_denominator;
}


// ax^2 + bx + c
void nucmath::Regression::quadratic(const std::vector<std::array<double, 2>> &points, double &a, double &b, double &c)
{
    double y_bar = 0.0, x_bar = 0.0;
    double x2_bar = 0.0;
    double x3_bar = 0.0;
    double x4_bar = 0.0;
    double y_x2_bar = 0.0, x_y_bar = 0.0;


    const double N = points.size();

    for(size_t i = 0; i < N; i++)
    {
        x_bar +=    points[i][0];
        y_bar +=    points[i][1];
        x2_bar +=   points[i][0]*points[i][0];
        x3_bar +=   points[i][0]*points[i][0]*points[i][0];
        x4_bar +=   points[i][0]*points[i][0]*points[i][0]*points[i][0];
        y_x2_bar+=  points[i][1]*points[i][0]*points[i][0];
        x_y_bar+=   points[i][0]*points[i][1];
    }
    x_bar /= N;
    x2_bar/=N;
    x3_bar/=N;
    x4_bar/=N;
    y_bar /= N;
    y_x2_bar /=N;
    x_y_bar /=N;

    a = ((y_x2_bar-y_bar*x2_bar)*(x2_bar-x_bar*x_bar)-(x_y_bar-y_bar*x_bar)*(x3_bar-x_bar*x2_bar))/
            ((x4_bar-x2_bar*x2_bar)*(x2_bar-x_bar*x_bar)-(x3_bar-x_bar*x2_bar)*(x3_bar-x_bar*x2_bar));


    b = (x_y_bar-x_bar*y_bar-a*(x3_bar-x_bar*x2_bar)) /
            (x2_bar-x_bar*x_bar);

    c = y_bar - a*x2_bar - b*x_bar;
}

