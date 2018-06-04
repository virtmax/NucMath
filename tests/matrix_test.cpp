#include "../../catch.hpp"

#include "matrix.h"

TEST_CASE("Matrix")
{
    nucmath::Matrix<double> mat(2, 3, {1,2,3,
                                       4,5,6});

    SECTION("create")
    {
        REQUIRE(mat(1,2) == Approx(6));
    }

    SECTION("operator ==")
    {
        nucmath::Matrix<double> mat1(2, 4, {-0.5, 23, 21, -24, 286.00000023, 3e-8, -20.9472e300, -234.2e-300});
        nucmath::Matrix<double> mat2(2, 4, {-0.5, 23, 21, -24, 286.00000023, 3e-8, -20.9472e300, -234.2e-300});
        nucmath::Matrix<double> mat3(2, 4, {-0.6, 23, 21, -24, 286.00000023, 3e-8, -20.9472e300, -234.2e-300});

        REQUIRE(mat1 == mat2);
        REQUIRE(mat1 != mat3);
    }

    SECTION("operator * 1")
    {
        nucmath::Matrix<double> mat1(1, 1, {-0.5});
        nucmath::Matrix<double> mat2(1, 1, {10.0});

        auto& res = mat1*mat2;

        REQUIRE(res(0,0) == Approx(-5.0));
    }

    SECTION("operator * 2")
    {
        nucmath::Matrix<double> mat1(1, 3, {3, 2, -1});
        nucmath::Matrix<double> mat2(3, 1, {1,
                                            0,
                                            4});
        nucmath::Matrix<double> mat3(1, 1, {-1});

        auto& res = mat1*mat2;

        REQUIRE(res.columns() == 1);
        REQUIRE(res.rows() == 1);

        REQUIRE(res == mat3);
    }

    SECTION("operator * 3")
    {
        nucmath::Matrix<double> mat1(2, 3, {3, 2, 1,
                                            1, 0, 2});
        nucmath::Matrix<double> mat2(3, 2, {1, 2,
                                            0, 1,
                                            4, 0});
        nucmath::Matrix<double> mat3(2, 2, {7, 8,
                                            9, 2});

        auto& res = mat1*mat2;

        REQUIRE(res.columns() == 2);
        REQUIRE(res.rows() == 2);

        REQUIRE(res == mat3);
    }

    SECTION("operator * (const Vector<T>& vec)")
    {
        nucmath::Matrix<double> mat(2, 3, {3, 2, 1,
                                           1, 0, 2});
        nucmath::Vector<double> vec({1, 0, 4});


        const auto& res = mat*vec;

        REQUIRE(res == std::vector<double>({7, 9}));
    }
}
