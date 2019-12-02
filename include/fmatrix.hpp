/* 
 
File: fmatrix.hpp

Brief: Flat Matrix Class Header

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#ifndef FLAT_MATRIX_CPP_H
#define FLAT_MATRIX_CPP_H

#include <stdexcept>
#include <algorithm>

template <unsigned n, unsigned m>
class FMatrix
{
public:
    
    using iterator       = double*;
    using const_iterator = const double*;

    /* Data Access Methods */
    double&       at(unsigned i, unsigned j);
    const double& at(unsigned i, unsigned j) const;

    double&       at_unsafe(unsigned i, unsigned j) noexcept;
    const double& at_unsafe(unsigned i, unsigned j) const noexcept;

    // Returns the flat matrix index in the array
    unsigned index(unsigned i, unsigned j) const noexcept;

    iterator       begin() noexcept       { return _fmat; };
    const_iterator begin() const noexcept { return _fmat; };

    iterator       end() noexcept       { return _fmat + (n * m); };
    const_iterator end() const noexcept { return _fmat + (n * m); };

    // [] index operator is NOdouble safe
    iterator       operator[](unsigned i) noexcept       { return _fmat + (m * i); }
    const_iterator operator[](unsigned i) const noexcept { return _fmat + (m * i); }

    double& operator()(unsigned i, unsigned j)             { return at(i, j); }
    const double& operator()(unsigned i, unsigned j) const { return at(i, j); }

    /* Arithmetic Operations */
    FMatrix<n, m> add        (const FMatrix<n, m>& rhs) const;
    FMatrix<n, m> operator + (const FMatrix<n, m>& rhs) const;

    FMatrix<n, m>& add_into    (const FMatrix<n, m>& rhs);
    FMatrix<n, m>& operator += (const FMatrix<n, m>& rhs);

    FMatrix<n, m>& mult_into (const double& scalar);
    FMatrix<n, m>& operator*=(const double& scalar);

    FMatrix<n, m> multiply (const double& scalar) const;
    FMatrix<n, m> operator*(const double& scalar) const;
    template <unsigned v, unsigned w>
    friend FMatrix<v, w> operator*(double scalar, const FMatrix<v,w>& rhs);

    template <unsigned p>
    FMatrix<n, p> multiply (const FMatrix<m, p>& rhs) const;
    template <unsigned p>
    FMatrix<n, p> operator* (const FMatrix<m, p>& rhs) const;

    /* Comparison Operations */
    bool operator == (const FMatrix<n, m>& rhs) const noexcept;
    bool operator != (const FMatrix<n, m>& rhs) const noexcept;

    /* Transformations */
    FMatrix<m,n> transpose() const;

    /* Flat Matrix Array */
    double _fmat[n * m] = {};
};

template <unsigned n>
using CVector = FMatrix<n,1>;

template <unsigned n>
using RVector = FMatrix<1,n>;

/** DATA ACCESS METHODS **/

template <unsigned n, unsigned m>
const double& FMatrix<n,m>::at(unsigned i, unsigned j) const
{
    if(i >= n || j >= m) { throw std::out_of_range("Matrix index out of range"); }

    return _fmat[index(i, j)];
}

template <unsigned n, unsigned m>
double& FMatrix<n,m>::at(unsigned i, unsigned j)
{
    if(i >= n || j >= m) { throw std::out_of_range("Matrix index out of range"); }

    return _fmat[index(i, j)];
}

template <unsigned n, unsigned m>
const double& FMatrix<n,m>::at_unsafe(unsigned i, unsigned j) const noexcept
{
    return _fmat[index(i, j)];
}

template <unsigned n, unsigned m>
double& FMatrix<n,m>::at_unsafe(unsigned i, unsigned j) noexcept
{
    return _fmat[index(i, j)];
}

template <unsigned n, unsigned m>
unsigned FMatrix<n,m>::index(unsigned i, unsigned j) const noexcept
{
    return (i * m) + j;
}

/* ARITHMETIC OPERATIONS */

template <unsigned n, unsigned m>
FMatrix<n,m> FMatrix<n,m>::add(const FMatrix<n,m>& rhs) const
{
    FMatrix<n,m> result;

    std::transform(begin(), end(), rhs.begin(), result.begin(), std::plus<double>());
    return result;
}

template <unsigned n, unsigned m>
FMatrix<n,m> FMatrix<n,m>::operator+(const FMatrix<n,m>& rhs) const
{
    return add(rhs);
}

template <unsigned n, unsigned m>
FMatrix<n,m>& FMatrix<n,m>::add_into(const FMatrix<n,m>& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(), std::plus<double>());
    return *this;
}

template <unsigned n, unsigned m>
FMatrix<n,m>& FMatrix<n,m>::operator+=(const FMatrix<n,m>& rhs)
{
    return add_into(rhs);
}

template <unsigned n, unsigned m>
FMatrix<n,m>& FMatrix<n,m>::mult_into(const double& scalar)
{
    std::for_each(begin(), end(), [&scalar](double& entry){ entry *= scalar; });
    return *this;
}

template <unsigned n, unsigned m>
FMatrix<n,m>& FMatrix<n,m>::operator*=(const double& scalar)
{
    return mult_into(scalar);
}

template <unsigned n, unsigned m>
FMatrix<n,m> FMatrix<n,m>::multiply(const double& scalar) const
{
    FMatrix<n,m> result;

    std::transform(begin(), end(), result.begin(), 
                  [&scalar](const double& i){ return scalar * i; });
    return result;
}

template <unsigned n, unsigned m>
FMatrix<n,m> FMatrix<n,m>::operator*(const double& scalar) const
{
    return multiply(scalar);
}

template <unsigned n, unsigned m>
FMatrix<n,m> operator*(double scalar, const FMatrix<n,m>& rhs)
{
    return rhs.multiply(scalar);
}

template <unsigned n, unsigned m>
template <unsigned p>
FMatrix<n, p> FMatrix<n,m>::multiply (const FMatrix<m, p>& B) const
{
    FMatrix<n,p> C;

    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < p; ++j)
        {
            double sum = 0;
            for (unsigned k = 0; k < m; ++k)
            {
                sum += (*this)[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

template <unsigned n, unsigned m>
template <unsigned p>
FMatrix<n, p> FMatrix<n,m>::operator* (const FMatrix<m, p>& rhs) const
{
    return multiply(rhs);
}

/* EQUIVALENCE OPERATIONS */
template <unsigned n, unsigned m>
bool FMatrix<n,m>::operator==(const FMatrix<n,m>& rhs) const noexcept
{
    for (unsigned i = 0; i < n * m; ++i)
    {
        if (_fmat[i] != rhs._fmat[i]) { return false; }
    }
    return true;
}

template <unsigned n, unsigned m>
bool FMatrix<n,m>::operator!=(const FMatrix<n,m>& rhs) const noexcept
{
    return !(*this == rhs);
}

/* TRANSFORMATIONS */
template <unsigned n, unsigned m>
FMatrix<m, n> FMatrix<n,m> ::transpose() const
{
    FMatrix<m,n> T;

    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < m; ++j)
        {
            T[j][i] = (*this)[i][j];
        }
    }
    return T;
}

#endif // FLAT_MATRIX_CPP_H
