/* 
 
File: csr_magtrix.hpp

Brief: Compressed Sparse Row matrix representation

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#include <vector>
#include <fmatrix.hpp>

template <unsigned n, unsigned m>
class CSRMatrix
{
public:

    CSRMatrix(FMatrix<n,m> A = {});

    FMatrix<n,m> to_fmatrix() const;

    unsigned _row[n + 1]  = {};
    std::vector<double>   _vals;
    std::vector<unsigned> _cols;
};

template <unsigned n, unsigned m>
CSRMatrix<n,m>::CSRMatrix(FMatrix<n,m> A)
{
    // Worst case scenario, we were handed a dense matrix
    _vals.reserve(n*m);
    _cols.reserve(n*m);

    unsigned row_index = 0;
    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < m; ++j)
        {
            if(A[i][j] != 0)
            {
                ++row_index;
                _vals.push_back(A[i][j]);
                _cols.push_back(j);
            }

        }
        _row[i+1] = row_index;
    }

    _vals.shrink_to_fit();
    _cols.shrink_to_fit();
}

template <unsigned n, unsigned m>
FMatrix<n,m> CSRMatrix<n,m>::to_fmatrix() const
{
    FMatrix<n,m> A;

    for(unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = _row[i]; j < _row[i+1]; ++j)
        {
            A[i][_cols[j]] = _vals[j];
        }
    }
    return A;
}
