#include "../catch.hpp"

#include "tablerow.h"

TEST_CASE("TableRow")
{
    SECTION("create with constructor 1")
    {
        nucmath::TableRow row(4);

        REQUIRE(row.getNumOfColumns() == 4);
        REQUIRE(row.getMax() == Approx(0.0));
        REQUIRE(row.getMin() == Approx(0.0));
    }

    SECTION("create with constructor 2")
    {
        nucmath::TableRow row({1, -4e7, 5, 7, 4.84e43});

        REQUIRE(row.getNumOfColumns() == 5);
        REQUIRE(row.getMin() == Approx(-4e7));
        REQUIRE(row.getMax() == Approx(4.84e43));
    }

    SECTION("create with constructor 3")
    {
        nucmath::TableRow row2({1, -4e7, 5, 7, 4.84e43});
        nucmath::TableRow row(row2);

        REQUIRE(row.getNumOfColumns() == 5);
        REQUIRE(row.getMin() == Approx(-4e7));
        REQUIRE(row.getMax() == Approx(4.84e43));
    }

    SECTION("setNumOfColumns")
    {
        nucmath::TableRow row(0);

        REQUIRE(row.getNumOfColumns() == 0);

        row.setNumOfColumns(5, 10.12345);

        REQUIRE(row.getNumOfColumns() == 5);

        REQUIRE(row.getMin() == Approx(10.12345));
        REQUIRE(row.getMax() == Approx(10.12345));
    }

    SECTION("operator[] const")
    {
        const nucmath::TableRow row({1, -4e7, 5, 7, 4.84e43});

        REQUIRE(row[3] == Approx(7));
    }

    SECTION("operator* const")
    {
        const nucmath::TableRow row({-1, -2, 3, -4, 1.5});

        const auto& row2 = row*2;
        REQUIRE(row2[0] == Approx(-2.0));
        REQUIRE(row2[4] == Approx(3.0));
    }

    SECTION("operator+=")
    {
        nucmath::TableRow row({-1, -2, 3, -4, 1.5});
        row += 10;

        REQUIRE(row[0] == Approx(9));
        REQUIRE(row[4] == Approx(11.5));
    }

    SECTION("throw exceptions 1")
    {
        nucmath::TableRow row(0);
        REQUIRE_THROWS(row[0]);
    }

    SECTION("throw exceptions 2")
    {
        const nucmath::TableRow row(0);
        REQUIRE_THROWS(row[0]);
    }

    SECTION("to string")
    {
        nucmath::TableRow row({1, -4.3e7, 5, 7, 4.84e15});

        REQUIRE(row.str() == "1.000000	-43000000.000000	5.000000	7.000000	4840000000000000.000000");
    }
}
