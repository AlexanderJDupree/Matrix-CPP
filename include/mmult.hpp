/* 
 
File: mmult.hpp

Brief: Matrix Multiplier defines a uniform interface for implementing matrix
       multiplication algorithms

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#ifndef MMULT_CPP_H
#define MMULT_CPP_H

#include <fmatrix.hpp>

template <typename T, unsigned n, unsigned m, unsigned p>
class Multiplier
{
    virtual FMatrix<T,n,p> operator()( const FMatrix<T,n,m>& A
                                     , const FMatrix<T,m,p>& B) const = 0;

};

template <typename T, unsigned n, unsigned m, unsigned p>
class Iterative_Multiplier : public Multiplier<T,n,m,p>
{

    FMatrix<T,n,p> operator()( const FMatrix<T,n,m>& A
                             , const FMatrix<T,m,p>& B) const
    {
        FMatrix<T,n,p> C;

        for (unsigned i = 0; i < n; ++i)
        {
            for (unsigned j = 0; j < p; ++j)
            {
                T sum = T();
                for (unsigned k = 0; k < m; ++k)
                {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        return C;
    }
};

#endif // MMULT_CPP_H


