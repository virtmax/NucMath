#include "../catch.hpp"

#include "regression.h"

TEST_CASE("linear regression")
{
    nucmath::Regression regression;
    std::vector<std::array<double, 2>> points0 = {};
    std::vector<std::array<double, 2>> points4 = {{0, 1}, {32e2, 2.4}, {-2, 10}, {-422e3, -300.444}};

    SECTION("correctness")
    {
        auto res = regression.linear(points4);

        REQUIRE(res.a1 == Approx(0.000720631));
        REQUIRE(res.a0 == Approx(3.68938));
        REQUIRE(res.r2 == Approx(0.99914));
    }

    SECTION("empty list")
    {
        REQUIRE_THROWS(regression.linear(points0));
    }
}

TEST_CASE("quadratic regression")
{
    nucmath::Regression regression;
    std::vector<std::array<double, 2>> points4 = {{0, 1}, {32e2, 2.4}, {-2, 10}, {-422e3, -300.444}};

    SECTION("regression.quadratic")
    {
        auto res = regression.quadratic(points4);

        CHECK(res.a0 == Approx(5.50044));
        CHECK(res.a1 == Approx(-0.000957004));
        CHECK(res.a2 == Approx(-3.98576E-9));
        //REQUIRE(res.r2 == Approx(0.99914));
    }
}
