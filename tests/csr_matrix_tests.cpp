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
                            , 0, 0, 1, 3, 0
                            , 0, 0, 0, 0, 6 };

        CSRMatrix<4,5> sparse(matrix);

        REQUIRE(sparse.to_fmatrix() == matrix);
    }
}

