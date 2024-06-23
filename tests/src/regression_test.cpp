#include "catch.hpp"

#include "regression.h"

TEST_CASE("linear regression")
{
    nucmath::Regression regression;
    std::vector<std::array<double, 2>> points0 = {};
    std::vector<std::array<double, 2>> points1 = {{-3, -3}, {-1.4, -0.5}, {1, 1.3}, {2, 4}, {3, -1.2}, {4, 3.9}, {5, 1}};
    std::vector<std::array<double, 2>> points4 = {{0, 1}, {32e2, 2.4}, {-2, 10}, {-422e3, -300.444}};

    SECTION("correctness 1")
    {
        auto res = regression.linear(points1);

        REQUIRE(res.a1 == Catch::Approx(0.554442));
        REQUIRE(res.a0 == Catch::Approx(-0.0538699));
        REQUIRE(res.r2 == Catch::Approx(0.380997));
    }

    SECTION("correctness 2")
    {
        auto res = regression.linear(points4);

        REQUIRE(res.a1 == Catch::Approx(0.000720631));
        REQUIRE(res.a0 == Catch::Approx(3.68938));
        REQUIRE(res.r2 == Catch::Approx(0.99914));
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

        CHECK(res.a0 == Catch::Approx(5.50044));
        CHECK(res.a1 == Catch::Approx(-0.000957004));
        CHECK(res.a2 == Catch::Approx(-3.98576E-9));
        //REQUIRE(res.r2 == Approx(0.99914));
    }
}
