

#include "regression.h"


nucmath::Regression::Regression()
{


}

nucmath::Regression::~Regression()
{


}


nucmath::Regression::LinearRegressionResult nucmath::Regression::linear(const std::vector<std::array<double, 2>> &points)
{
    LinearRegressionResult result;
    const double N = points.size();

    if(N < 2)
    {
        throw std::invalid_argument("Regression::linear: at least 2 data points are needed.");
    }

    double sum_y = 0.0, sum_x = 0.0;
    double sum_x_y = 0.0, sum_x2 = 0.0, sum_y2 = 0.0;

    for(size_t i = 0; i < N; i++)
    {
        sum_x +=    points[i][0];
        sum_x2 +=   points[i][0]*points[i][0];

        sum_x_y+=   points[i][0]*points[i][1];

        sum_y +=    points[i][1];
        sum_y2 +=   points[i][1]*points[i][1];
    }

    const double b_counter = (N*sum_x_y-sum_x*sum_y);
    const double b_denominator = (N*sum_x2 - sum_x*sum_x);
    if(isEqual(b_denominator, 0)) // TODO: ungenau
    {
        throw std::invalid_argument("Regression::linear: division by 0 (b_denominator). check your input data.");
    }
    result.a1 = b_counter/b_denominator;
    result.a0 = (sum_y - result.a1*sum_x)/N;
    result.r2 = 0;

    result.sqr = 0;
    for(size_t i = 0; i < N; i++)
    {
        double residual = points[i][1] - (result.a0 + result.a1*points[i][0]);
        result.sqr += residual*residual;
    }


    const double R2_counter = b_counter*b_counter;
    const double R2_denominator = (N*sum_x2 - sum_x*sum_x)*(N*sum_y2 - sum_y*sum_y);
    if(!isEqual(R2_denominator, 0))
        result.r2 = R2_counter/R2_denominator;

    result.r2_red = 1.0 - (1.0 - result.r2)*(N-1.0)/(N-2.0);

    return result;
}


nucmath::Regression::QuadraticRegressionResult nucmath::Regression::quadratic(const std::vector<std::array<double, 2>> &points)
{
    QuadraticRegressionResult result;
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

    const double sxx = x2_bar - x_bar*x_bar;
    const double sxy = x_y_bar - x_bar*y_bar;
    const double sxx2 = x3_bar - x_bar*x2_bar;
    const double sx2x2 = x4_bar - x2_bar*x2_bar;
    const double sx2y = y_x2_bar - x2_bar*y_bar;

    result.c = result.a2 = (sx2y * sxx - sxy * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
    result.a1 = (sxy * sx2x2 - sx2y * sxx2) / (sxx * sx2x2 - sxx2 * sxx2);
    result.a0 = y_bar - result.a1*x_bar - result.a2*x2_bar;

    result.x0 = -result.a1/(2*result.a2);
    result.y0 = result.a0 - result.a1*result.a1/(4*result.a2);

    return result;
}

