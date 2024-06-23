#include "catch.hpp"

#include "functions.h"

TEST_CASE("Functions", "Everything" )
{

    SECTION( "Factorial" )
    {
        REQUIRE(nucmath::factorial(0) == Catch::Approx(1));
        REQUIRE(nucmath::factorial(1) == Catch::Approx(1));
        REQUIRE(nucmath::factorial(2) == Catch::Approx(2));
        REQUIRE(nucmath::factorial(3) == Catch::Approx(6));
        REQUIRE(nucmath::factorial(10) == Catch::Approx(3628800));
        REQUIRE(nucmath::factorial(50) == Catch::Approx(3.0414093201e64));
    }

    SECTION( "binomialCoefficient" )
    {
        REQUIRE(nucmath::binomialCoefficient(0, 0) == Catch::Approx(1));
        REQUIRE(nucmath::binomialCoefficient(34, 0) == Catch::Approx(1));
        REQUIRE(nucmath::binomialCoefficient(34, 1) == Catch::Approx(34));
        REQUIRE(nucmath::binomialCoefficient(0, 1) == Catch::Approx(0));
        REQUIRE(nucmath::binomialCoefficient(1, 1) == Catch::Approx(1));
        REQUIRE(nucmath::binomialCoefficient(5, 1) == Catch::Approx(5));
        REQUIRE(nucmath::binomialCoefficient(10, 10) == Catch::Approx(1));
        REQUIRE(nucmath::binomialCoefficient(10, 1) == Catch::Approx(10));
        REQUIRE(nucmath::binomialCoefficient(24, 6) == Catch::Approx(134596));
        REQUIRE(nucmath::binomialCoefficient(49, 6) == Catch::Approx(13983816));
        REQUIRE(nucmath::binomialCoefficient(49, 40) == Catch::Approx(2054455634));
    }


    SECTION( "binomialPMF" )
    {
        REQUIRE(nucmath::binomialPMF(0, 0, 0) == Catch::Approx(1));
        REQUIRE(nucmath::binomialPMF(0.5, 1, 0) == Catch::Approx(0.5));
        REQUIRE(nucmath::binomialPMF(0.5, 1, 1) == Catch::Approx(1));
        REQUIRE(nucmath::binomialPMF(0.084, 34, 34) == Catch::Approx(1));
        REQUIRE(nucmath::binomialPMF(0.484, 34, 34) == Catch::Approx(1));
        REQUIRE(nucmath::binomialPMF(0.084, 24, 6) == Catch::Approx(0.99717));
        REQUIRE(nucmath::binomialPMF(0.7524, 49, 40) == Catch::Approx(0.888442));
    }
}
