
#include "../catch.hpp"

#include "hist.h"

TEST_CASE("Hist", "[vector]" )
{
    nucmath::Hist hist;

    SECTION( "create" )
    {
        hist.init(0, 0.041);

        REQUIRE(hist.getBinWidth() == Approx(0.041));

        hist.add(0);
        REQUIRE(hist.sum() == 1);
    }

    SECTION("iteration")
    {
        for(auto& entry : hist)
        {

        }
    }
}
