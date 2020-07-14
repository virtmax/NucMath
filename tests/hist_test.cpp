
#include "../catch.hpp"

#include "hist.h"

TEST_CASE("Hist", "[vector]" )
{
    SECTION( "constructor" )
    {
        nucmath::Hist hist(-10.0, 0.25, 8);

        REQUIRE(hist.getBinWidth() == Approx(0.25));
        REQUIRE(hist.getLowestEdge() == Approx(-10.0));
        REQUIRE(hist.getHighestEdge() == Approx(-10.0 + 0.25*8));
    }

    SECTION( "copy constructor" )
    {
        nucmath::Hist hist(-10.0, 0.25, 8);

        nucmath::Hist hist2(hist);

        REQUIRE(hist2.getBinWidth() == Approx(0.25));
        REQUIRE(hist2.getLowestEdge() == Approx(-10.0));
        REQUIRE(hist2.getHighestEdge() == Approx(-10.0 + 0.25*8));
    }

    SECTION( "init" )
    {
        nucmath::Hist hist;
        hist.init(-10.0, 0.25, 8);

        REQUIRE(hist.getBinWidth() == Approx(0.25));
        REQUIRE(hist.getLowestEdge() == Approx(-10.0));
        REQUIRE(hist.getHighestEdge() == Approx(-10.0 + 0.25*8));
    }

    SECTION( "add" )
    {
        nucmath::Hist hist;
        hist.init(-10.0, 0.25, 8);

        hist.add(3.0);
        REQUIRE(hist.getLowestEdge() == Approx(-10.0));
        REQUIRE(hist.getHighestEdge() == Approx(3.0 + 0.25));

        hist.add(-20.1);
        REQUIRE(hist.getLowestEdge() == Approx(-20.0 - 0.25));
        REQUIRE(hist.getHighestEdge() == Approx(3.0 + 0.25));
    }

    SECTION( "min/max" )
    {
        nucmath::Hist hist;

        hist.add(-20.1, -10);
        hist.add(-20.1, -30);
        hist.add(-20.1, -5);

        REQUIRE(hist.min() == Approx(-45));
        REQUIRE(hist.max() == Approx(-45));

        hist.add(10, -125);
        hist.add(-30, 5);

        REQUIRE(hist.min() == Approx(-125));
        REQUIRE(hist.max() == Approx(5));
    }

    SECTION( "getRangeOfInsertedData" )
    {
        nucmath::Hist hist;

        hist.add(323.0003352);

        const auto& [low1, high1] = hist.getRangeOfInsertedData();
        REQUIRE(low1 == Approx(323.0003352));
        REQUIRE(high1 == Approx(323.0003352));

        hist.add(-3.123);
        hist.add(-153.876523);

        REQUIRE(hist.getLowestEdge() == Approx(-154));
        REQUIRE(hist.getHighestEdge() == Approx(324));

        const auto& [low, high] = hist.getRangeOfInsertedData();
        REQUIRE(low == Approx(-153.876523));
        REQUIRE(high == Approx(323.0003352));
    }

    SECTION( "sum" )
    {
        nucmath::Hist hist(-10.0, 0.25, 8);

        double sum = 0;
        const size_t N = 100;
        for(size_t i = 0; i < N; i++)
        {
            const double x = nucmath::getRandom(-100, 200);
            const double y = nucmath::getRandom(-100, 200);
            sum += y;
            hist.add(x, y);
        }

        REQUIRE(hist.sum() == Approx(sum));
    }

    SECTION( "mean" )
    {
        nucmath::Hist hist(-10.0, 1, 8);

        double sum = 0;
        const size_t N = 100;
        for(size_t i = 0; i < N; i++)
        {
            const double x = nucmath::getRandom(-100, 200);
            const double y = nucmath::getRandom(-100, 200);
            sum += y;
            hist.add(x, y);
        }

        REQUIRE(hist.mean() == Approx(sum/hist.nBins()));
    }
/*
    SECTION("iteration")
    {
        nucmath::Hist hist;
        hist.init(-10.0, 0.25, 8);
        for(auto& entry : hist)
        {

        }
    }
            */
}
