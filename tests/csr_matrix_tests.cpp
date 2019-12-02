/* 
 
File: csr_matrix_tests.hpp

Brief: Unit tests for the CSR matrix type

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#include <iostream>
#include <catch.hpp>
#include <fmatrix.hpp>
#include <csr_matrix.hpp>

TEST_CASE("Constructing CSR Matrices", "[constructors], [csr_matrix]")
{
    SECTION("Construction from an empty FMatrix")
    {
        FMatrix<3,3> empty;

        CSRMatrix<3,3> matrix(empty);

        REQUIRE(matrix.to_fmatrix() == empty);
    }
    SECTION("Construction from a sparse FMatrix")
    {
        FMatrix<4,5> matrix { 0, 0, 0, 0, 0
                            , 5, 8, 0, 0, 0
                            , 0, 0, 0, 0, 0
                            , 0, 0, 2, 0, 6 };

        CSRMatrix<4,5> sparse(matrix);

        REQUIRE(sparse.to_fmatrix() == matrix);
    }
    SECTION("Construction from a dense FMatrix")
    {
        FMatrix<3,3> matrix { 1, 2, 3
                            , 4, 5, 6
                            , 7, 8, 9 };

        CSRMatrix<3,3> csr(matrix);

        REQUIRE(csr.to_fmatrix() == matrix);
    }
    SECTION("Construction with an initializer list")
    {
        CSRMatrix<3, 3> csr { 1, 2, 3 
                            , 4, 5, 6
                            , 7, 8, 9 };

        FMatrix<3,3> matrix { 1, 2, 3
                            , 4, 5, 6
                            , 7, 8, 9 };

        REQUIRE(csr.to_fmatrix() == matrix);
    }
    /*
    SECTION("Construction with an incomplete initializer list assumes the missing entries are zero")
    {
        CSRMatrix<3, 4> csr { 1, 2, 3, 4 };

        FMatrix<3, 4> matrix { 1, 2, 3, 4 
                             , 0, 0, 0, 0
                             , 0, 0, 0, 0 };

        REQUIRE(csr.to_fmatrix() == matrix);
    }
    */
}

TEST_CASE("Matrix Transpose", "[transpose], [csr_matrix]")
{

    FMatrix<4,5> A { 0, 0, 0, 0, 0
                   , 5, 8, 0, 0, 0
                   , 0, 0, 0, 0, 0
                   , 0, 0, 2, 0, 6 };

    FMatrix<5,4> A_t { 0, 5, 0, 0
                     , 0, 8, 0, 0
                     , 0, 0, 0, 2
                     , 0, 0, 0, 0
                     , 0, 0, 0, 6 };

    FMatrix<3,3> B { 1, 2, 3
                   , 4, 5, 6
                   , 7, 8, 9 };

    FMatrix<3,3> B_t { 1, 4, 7
                     , 2, 5, 8
                     , 3, 6, 9 };

    CSRMatrix<4,5> CSR_A(A);
    CSRMatrix<3,3> CSR_B(B);

    SECTION("Transpose of an NxM matrix returns an MxN matrix where A[i][j] = A_t[j][i]")
    {
        REQUIRE(CSR_A.transpose().to_fmatrix() == A_t);
    }
    SECTION("Transpose a dense matrix")
    {
        REQUIRE(CSR_B.transpose().to_fmatrix() == B_t);
    }
}

TEST_CASE("CSR Scalar Multiplication", "[multiplication], [scalar], [csr_matrix]")
{
    CSRMatrix<3, 3> A { 1, 1, 1
                      , 1, 1, 1
                      , 1, 1, 1 };

    SECTION("Scalar Multiplication is commutative")
    {
        REQUIRE( 2 * A == A * 2 );
    }
    SECTION("Scalar Multiplication into a matrix")
    {

        CSRMatrix<3, 3> B { 2, 2, 2
                          , 2, 2, 2
                          , 2, 2, 2 };

        A *= 2;
        REQUIRE( A == CSRMatrix(B) );
    }
}

TEST_CASE("CSR Matrix Multiplication", "[multiplication], [csr_matrix]")
{
    SECTION("CSR Matrix multipled by an F Matrix")
    {
        CSRMatrix<3, 4> A { 1, 2, 0, 0
                          , 0, 1, 0, 1
                          , 0, 0, 0, 0 };
                        
        FMatrix<4, 3> B { 1, 2, 3 
                        , 1, 2, 3 
                        , 1, 2, 3 
                        , 1, 2, 3 };

        FMatrix<3,3> C { 3, 6, 9 
                       , 2, 4, 6
                       , 0, 0, 0 };
        REQUIRE( A * B == C);

    }
}
