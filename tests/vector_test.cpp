#include "../catch.hpp"

#include "vector.h"

TEST_CASE("Vector")
{
    SECTION("create with constructor 1")
    {
        nucmath::Vector<double> vec(3);
        REQUIRE(vec.dim() == 3);
    }

    SECTION("create with = ")
    {
        nucmath::Vector<double> vec({1.0, -3, 4, -10.3});

        REQUIRE(vec.dim() == 4);
        REQUIRE(vec == std::vector<double>({1.0, -3, 4, -10.3}));
    }

    SECTION("operator = (const Vector<T>& vec)")
    {
        nucmath::Vector<double> vec1({1, 2, 3.5, 4});
        nucmath::Vector<double> vec2(1);
        vec2 = vec1;

        REQUIRE(vec2 == vec1);
    }

    SECTION("operator +")
    {
        nucmath::Vector<double> vec1({1, 2, 3.5, 4});
        nucmath::Vector<double> vec2({-10, -7, -2.5, 10});

        nucmath::Vector<double> vec = vec1 + vec2;

        REQUIRE(vec.dim() == 4);
    }

    SECTION("operator *")
    {
        nucmath::Vector<double> vec1({1, 2, 3.5, 4});
        nucmath::Vector<double> vec2({-10, -7, -2.5, 10});

        double scalar = vec1 * vec2;

        REQUIRE(scalar == Approx(1*(-10)+2*(-7)+3.5*(-2.5)+4*10));
    }
}
