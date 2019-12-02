/* 
 
File: csr_magtrix.hpp

Brief: Compressed Sparse Row matrix representation

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#include <vector>
#include <algorithm>
#include <initializer_list>

#include <fmatrix.hpp>

template <unsigned n, unsigned m>
class CSRMatrix
{
public:

    CSRMatrix() = default;
    CSRMatrix(const CSRMatrix<n,m>&) = default;

    CSRMatrix(FMatrix<n,m> A);
    CSRMatrix(std::initializer_list<double> il);

    FMatrix<n,m> to_fmatrix() const;

    unsigned nnz() const { return _vals.size(); }

    /* Transformations */
    CSRMatrix<m,n> transpose() const;

    /* Arithmetic Operations */

    CSRMatrix<n, m>& mult_into (const double& scalar);
    CSRMatrix<n, m>& operator*=(const double& scalar);

    CSRMatrix<n, m> multiply (const double& scalar) const;
    CSRMatrix<n, m> operator*(const double& scalar) const;
    template <unsigned v, unsigned w>
    friend CSRMatrix<v, w> operator*(double scalar, const CSRMatrix<v,w>& rhs);

    template <unsigned p>
    FMatrix<n, p> multiply (const FMatrix<m, p>& rhs) const;
    template <unsigned p>
    FMatrix<n, p> operator* (const FMatrix<m, p>& rhs) const;
    template <unsigned v, unsigned w, unsigned p>
    friend FMatrix<v, p> operator*(FMatrix<v,w>, const CSRMatrix<w,p>&);

    /* Equality Operations */
    bool operator == (const CSRMatrix<n, m>& rhs) const noexcept;
    bool operator != (const CSRMatrix<n, m>& rhs) const noexcept;
    // TODO Implement equality operations for different matrix types

    unsigned _row[n + 1]  = {}; // _row[0] ==> 1st row start
    std::vector<double>   _vals; // Zero-indexed
    std::vector<unsigned> _cols; // Zero-indexed
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
CSRMatrix<n,m>::CSRMatrix(std::initializer_list<double> il)
{
    // Worst case scenario, we were handed a dense matrix
    _vals.reserve(n*m);
    _cols.reserve(n*m);

    unsigned row_index = 0;
    std::initializer_list<double>::const_iterator it = il.begin();
    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < m; ++j)
        {
            if(it != il.end() && *it != 0)
            {
                ++row_index;
                _vals.push_back(*it);
                _cols.push_back(j);
            }
            ++it;
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
    FMatrix<m,n> T;

    for(unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = _row[i]; j < _row[i+1]; ++j)
        {
            T[_cols[j]][i] = _vals[j];
        }
    }

    return T;
}

/** ARITHMETIC OPERATIONS **/ 
template<unsigned n, unsigned m>
CSRMatrix<n, m>& CSRMatrix<n,m>::mult_into (const double& scalar)
{
    std::for_each(_vals.begin(), _vals.end(), [&](double& val)
    {
        val *= scalar;
    });
    return *this;
}
template<unsigned n, unsigned m>
CSRMatrix<n, m>& CSRMatrix<n,m>::operator*=(const double& scalar)
{
    return mult_into(scalar);
}
template<unsigned n, unsigned m>
CSRMatrix<n, m> CSRMatrix<n,m>::multiply (const double& scalar) const
{
    return CSRMatrix(*this).mult_into(scalar);
}
template<unsigned n, unsigned m>
CSRMatrix<n, m> CSRMatrix<n,m>::operator*(const double& scalar) const
{
    return multiply(scalar);
}
template <unsigned v, unsigned w>
CSRMatrix<v, w> operator*(double scalar, const CSRMatrix<v,w>& rhs)
{
    return rhs * scalar;
}

template <unsigned n, unsigned m>
template <unsigned p>
FMatrix<n, p> CSRMatrix<n,m>::multiply (const FMatrix<m, p>& B) const
{
    FMatrix<n,p> C;

    for(unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < p; ++j)
        {
            double sum = 0;
            for (unsigned k = _row[i]; k < _row[i+1]; ++k)
            {
                sum += _vals[k] * B[_cols[k]][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

template <unsigned n, unsigned m>
template <unsigned p>
FMatrix<n, p> CSRMatrix<n,m>::operator* (const FMatrix<m, p>& rhs) const
{
    return multiply(rhs);
}

/** EQUALITY OPERATIONS **/

template<unsigned n, unsigned m>
bool CSRMatrix<n,m>::operator == (const CSRMatrix<n, m>& rhs) const noexcept
{
    return _vals == rhs._vals 
        && _cols == rhs._cols
        && std::equal(_row, _row + n, rhs._row);
}

template<unsigned n, unsigned m>
bool CSRMatrix<n,m>::operator != (const CSRMatrix<n, m>& rhs) const noexcept
{
    return !(*this == rhs);
}

