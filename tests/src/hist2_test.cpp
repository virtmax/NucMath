#include "../../catch.hpp"

#include "hist2.h"
#include "functions.h"

TEST_CASE("Hist2")
{
    nucmath::Hist2 hist2;
    hist2.init(0, 0, 2, 5, 1, 1);
    hist2.add(1, 1, 30);
    hist2.add(5.3, 13.4, -30);

    /*
    SECTION( "save" )
    {

        REQUIRE(hist2.save("../NucMath/tests/data/hist2_list.txt", nucmath::Hist2::FileFormat::List) == true);
        REQUIRE(hist2.save("../NucMath/tests/data/hist2_array.txt", nucmath::Hist2::FileFormat::Array) == true);
    }
    */
}

