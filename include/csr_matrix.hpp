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

    using val_iterator       = std::vector<double>::iterator;
    using const_val_iterator = std::vector<double>::const_iterator;

    CSRMatrix() = default;
    CSRMatrix(FMatrix<n,m> A);

    FMatrix<n,m> to_fmatrix() const;

    // Value iterators iterate through all populated entries in the matrix
    val_iterator       vbegin() noexcept       { return _vals.end(); }
    const_val_iterator vbegin() const noexcept { return _vals.end(); }
    val_iterator       vend() noexcept       { return _vals.end(); }
    const_val_iterator vend() const noexcept { return _vals.end(); }

    /* Transformations */
    CSRMatrix<m,n> transpose() const;

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

template<unsigned n, unsigned m>
CSRMatrix<m, n> CSRMatrix<n,m>::transpose() const
{
    CSRMatrix<m, n> T;

    T._vals.reserve(_vals.size());
    T._cols.reserve(_cols.size());

    unsigned row_index = 0;

    return T;
}
