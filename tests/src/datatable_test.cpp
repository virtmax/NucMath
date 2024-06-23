
#include "catch.hpp"

#include "tablerow.h"
#include "datatable.h"

TEST_CASE("Datatable")
{
    SECTION("create with constructor")
    {
        nucmath::DataTable table(4);

        REQUIRE(table.getNumberOfColumns() == 4);
        REQUIRE(table.getNumberOfRows() == 0);

        SECTION("add rows")
        {
            nucmath::TableRow row({1, 2, 3, 4});
            table.addRow(row);
            table.addRow({5, 6, 7, 8});
            table.addRow({9, 10, 11, 12});

            REQUIRE(table[1][3] == Catch::Approx(8));

            SECTION("change entries")
            {
                table[2][0] = 124.753;
                REQUIRE(table[2][0] == Catch::Approx(124.753));
            }

            SECTION("save as file")
            {
                table.setHeader(" test header \n line 2 \n line 3");
                std::string file = "./table_test.txt";
                table.save(file);
            }
        }
    }

    SECTION("load standard table")
    {
        nucmath::DataTable table;
        table.load("../NucMath/tests/data/table_standard.txt");

        REQUIRE(table.getNumberOfColumns() == 6);
    }
}
