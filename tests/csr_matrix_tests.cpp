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

}

