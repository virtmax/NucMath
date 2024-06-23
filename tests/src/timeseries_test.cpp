#include "catch.hpp"

#include "timeseries.h"

TEST_CASE("Timeseries", "[vector]" )
{
    SECTION( "constructor" )
    {
        nucmath::TimeSeries ts;

        ts.add(0.2, 1);
        ts.add(1.4, 3);
        ts.add(2.5, 5);



        //REQUIRE(ts.get() == Approx(0.25));
        //REQUIRE(ts.getLowestEdge() == Approx(-10.0));
        //REQUIRE(ts.getHighestEdge() == Approx(-10.0 + 0.25*8));
    }

}
