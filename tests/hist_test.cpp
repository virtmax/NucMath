
#include "../catch.hpp"

#include "hist.h"

TEST_CASE("Hist", "[vector]" )
{

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
