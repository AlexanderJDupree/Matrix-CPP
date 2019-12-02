/* 
 
File: fmatrix_tests.hpp

Brief: Unit tests for the Flat matrix type

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/


#include <iostream>
#include <catch.hpp>
#include <fmatrix.hpp>

TEST_CASE("Constructing Matrices", "[constructors], [fmatrix]")
{
    SECTION("Default matrix construction zero initializes values")
    {
        FMatrix<3,3> matrix;

        std::for_each(matrix.begin(), matrix.end(), [](double d)
                {
                    REQUIRE(d == 0);
                });
    }
}

TEST_CASE("Accessing data from a matrix", "[at], [operator], [fmatrix]")
{

    FMatrix<3, 3> matrix { 1, 2, 3
                         , 4, 5, 6
                         , 7, 8, 9 };

    SECTION("Using the at function, matrices are zero indexed")
    {
        int expected = 0;
        for (auto i : { 0, 1, 2 })
        {
            for (auto j : { 0, 1, 2})
            {
                REQUIRE(matrix.at(i, j) == ++expected);
            }
        }
    }
    SECTION("at() throws when given out of bounds indices")
    {
        REQUIRE_THROWS_AS(matrix.at(10, 10), std::out_of_range);
    }
    SECTION("Ranged based for loop")
    {
        int expected = 0;
        for (const auto& i : matrix)
        {
            REQUIRE(i == ++expected);
        }
    }
    SECTION("using the index operator []")
    {
        int expected = 0;
        for (auto i : { 0, 1, 2 })
        {
            for (auto j : { 0, 1, 2})
            {
                REQUIRE(matrix[i][j] == ++expected);
            }
        }
    }
    SECTION("Using the function call operator")
    {
        int expected = 0;
        for (auto i : { 0, 1, 2 })
        {
            for (auto j : { 0, 1, 2})
            {
                REQUIRE(matrix(i, j) == ++expected);
            }
        }
    }
}

TEST_CASE("Matrix transpose", "[transpose], [fmatrix]")
{
    FMatrix<3, 3> A { 1, 2, 3
                    , 4, 5, 6
                    , 7, 8, 9 };

    FMatrix<3, 3> A_t { 1, 4, 7
                      , 2, 5, 8
                      , 3, 6, 9 };

    FMatrix<2,3> B { 1, 2, 3
                   , 4, 5, 6 };

    FMatrix<3,2> B_t { 1, 4
                     , 2, 5
                     , 3, 6 };

    FMatrix<3, 3> C { 1, 2, 3
                    , 2, 1, 5
                    , 3, 5, 2 };

    SECTION("Transpose of a square matrix returns a square matrix transposed")
    {
        REQUIRE(A.transpose() == A_t);
    }
    SECTION("The transpose of an NxM matrix returns a transposed MxN matrix")
    {
        REQUIRE(B.transpose() == B_t);
    }
    SECTION("The transpose of a symmetric matrix is itself")
    {
        REQUIRE(C == C.transpose());
    }
}

TEST_CASE("Matrix Addition", "[addition], [fmatrix]")
{
    FMatrix<3, 3> A { 1, 2, 3
                    , 4, 5, 6
                    , 7, 8, 9 };

    FMatrix<3, 3> B { 1, 1, 1
                    , 1, 1, 1
                    , 1, 1, 1 };

    FMatrix<3, 3> C { 2, 3, 4
                    , 5, 6, 7
                    , 8, 9, 10 };

    SECTION("Matrix addition is commutative")
    {
        REQUIRE( A + B == B + A );
    }
    SECTION("Matrix addition is associative")
    {
        REQUIRE( A + (B + C) == (A + B) + C);
    }
    SECTION("Addition into a matrix")
    {
        A += B;
        REQUIRE(A == C);
    }
}

TEST_CASE("FMatrix Scalar Multiplication", "[multiplication], [scalar], [fmatrix]")
{
    FMatrix<3, 3> A { 1, 1, 1
                    , 1, 1, 1
                    , 1, 1, 1 };

    SECTION("Scalar Multiplication is commutative")
    {
        REQUIRE( 2 * A == A * 2 );
    }
    SECTION("Scalar Multiplication into a matrix")
    {

        FMatrix<3, 3> B { 2, 2, 2
                        , 2, 2, 2
                        , 2, 2, 2 };

        A *= 2;
        REQUIRE( A == B );
    }
}

TEST_CASE("Matrix Multiplication", "[multiplication], [fmatrix]")
{
    FMatrix<3, 3> A {  7, 2,  1
                    ,  0, 3, -1
                    , -3, 4, -2 };

    FMatrix<3, 3> A_i { -2,   8, -5
                      ,  3, -11,  7
                      ,  9, -34,  21 };

    FMatrix<3, 3> I3 { 1, 0, 0
                     , 0, 1, 0
                     , 0, 0, 1 };

    SECTION("Multiplication by the identity is commutative")
    {
        REQUIRE(I3 * A == A * I3);
    }
    SECTION("Multiplication by an inverse returns the identiy")
    {
        REQUIRE(A * A_i == I3);
    }
    SECTION("Multiplying an NxM matrix by an MxP matrix returns an NxP matrix")
    {
        FMatrix<2,3> A { 1, 2, 3
                       , 1, 1, 1 };

        FMatrix<3,4> B { 1, 2, 3, 4
                       , 1, 2, 2, 1
                       , 1, 1, 0, 1 };

        FMatrix<2,4> C { 6, 9, 7, 9
                       , 3, 5, 5, 6 };

        REQUIRE(A * B == C);
    }
    SECTION("Multiplying an NxM matrix by its transpose returns an NxN symmetric matrix")
    {
        FMatrix<2,3> B { 1, 2, 3
                       , 4, 5, 6 };

        FMatrix<2,2> BB_t { 14, 32
                          , 32, 77 };

        REQUIRE(B * B.transpose() == BB_t);
    }
}
