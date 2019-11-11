/* 
 
File: fmatrix_tests.hpp

Brief: Unit tests for the Flat matrix type

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/


#include <iostream>
#include <catch.hpp>
#include <mmult.hpp>
#include <fmatrix.hpp>

TEST_CASE("Constructing Matrices", "[constructors], [fmatrix]")
{

}

TEST_CASE("Accessing data from a matrix", "[at], [operator], [fmatrix]")
{

    FMatrix<int, 3, 3> matrix { 1, 2, 3
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

TEST_CASE("Matrix Addition", "[addition], [fmatrix]")
{
    FMatrix<int, 3, 3> A { 1, 2, 3
                         , 4, 5, 6
                         , 7, 8, 9 };

    FMatrix<int, 3, 3> B { 1, 1, 1
                         , 1, 1, 1
                         , 1, 1, 1 };

    FMatrix<int, 3, 3> C { 2, 3, 4
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

TEST_CASE("Scalar Multiplication", "[multiplication], [scalar], [fmatrix]")
{
    FMatrix<int, 3, 3> A { 1, 1, 1
                         , 1, 1, 1
                         , 1, 1, 1 };

    SECTION("Scalar Multiplication is commutative")
    {
        REQUIRE( 2 * A == A * 2 );
    }
    SECTION("Scalar Multiplication into a matrix")
    {

        FMatrix<int, 3, 3> B { 2, 2, 2
                             , 2, 2, 2
                             , 2, 2, 2 };

        A *= 2;
        REQUIRE( A == B );
    }
}

TEST_CASE("Matrix Multiplication", "[multiplication], [fmatrix]")
{
    FMatrix<int, 3, 3> A {  7, 2,  1
                         ,  0, 3, -1
                         , -3, 4, -2 };

    FMatrix<int, 3, 3> A_Inverse { -2,   8, -5
                                 ,  3, -11,  7
                                 ,  9, -34,  21 };

    FMatrix<int, 3, 3> I3 { 1, 0, 0
                          , 0, 1, 0
                          , 0, 0, 1 };

    Matrix_Multiplier mmult;

    SECTION("Multiplication by the identity is commutative")
    {
        REQUIRE(mmult(I3, A) == mmult(A, I3));
    }
    SECTION("Multiplication by an inverse returns the identiy")
    {
        REQUIRE(mmult(A, A_Inverse) == I3);
    }
    SECTION("Multiplying an NxM matrix by an MxP matrix returns an NxP matrix")
    {
        FMatrix<int,2,3> A { 1, 2, 3
                           , 1, 1, 1 };

        FMatrix<int,3,4> B { 1, 2, 3, 4
                           , 1, 2, 2, 1
                           , 1, 1, 0, 1 };

        FMatrix<int,2,4> C { 6, 9, 7, 9
                           , 3, 5, 5, 6 };

        REQUIRE(mmult(A, B) == C);
    }
}
