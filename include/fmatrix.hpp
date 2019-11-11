/* 
 
File: fmatrix.hpp

Brief: Flat Matrix Class Header

Authors: Alexander DuPree

https://github.com/AlexanderJDupree/matrix-cpp
 
*/

#ifndef FLAT_MATRIX_CPP_H
#define FLAT_MATRIX_CPP_H

#include <array>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <initializer_list>

// TODO SFINAE on type T to ensure T has arithmetic operators defined
template <typename T, unsigned n, unsigned m>
class FMatrix
{
public:
    
    using iterator       = T*;
    using const_iterator = const T*;

    /* Data Access Methods */
    T&       at(unsigned i, unsigned j);
    const T& at(unsigned i, unsigned j) const;

    T&       at_unsafe(unsigned i, unsigned j) noexcept;
    const T& at_unsafe(unsigned i, unsigned j) const noexcept;

    // Returns the flat matrix index in the array
    unsigned index(unsigned i, unsigned j) const noexcept;

    iterator       begin() noexcept       { return _fmat; };
    const_iterator begin() const noexcept { return _fmat; };

    iterator       end() noexcept       { return _fmat + (n * m); };
    const_iterator end() const noexcept { return _fmat + (n * m); };

    // [] index operator is NOT safe
    iterator       operator[](unsigned i) noexcept       { return _fmat + (n * i); }
    const_iterator operator[](unsigned i) const noexcept { return _fmat + (n * i); }

    T& operator()(unsigned i, unsigned j)             { return at(i, j); }
    const T& operator()(unsigned i, unsigned j) const { return at(i, j); }

    /* Arithmetic Operations */
    FMatrix<T, n, m> add        (const FMatrix<T, n, m>& rhs) const;
    FMatrix<T, n, m> operator + (const FMatrix<T, n, m>& rhs) const;

    FMatrix<T, n, m>& add_into    (const FMatrix<T, n, m>& rhs);
    FMatrix<T, n, m>& operator += (const FMatrix<T, n, m>& rhs);

    FMatrix<T, n, m>& mult_into (const T& scalar);
    FMatrix<T, n, m>& operator*=(const T& scalar);

    FMatrix<T, n, m> multiply (const T& scalar) const;
    FMatrix<T, n, m> operator*(const T& scalar) const;
    template <typename U, unsigned v, unsigned w>
    friend FMatrix<U, v, w> operator*(const U& scalar, const FMatrix<U,v,w>& rhs);

    template <unsigned p>
    FMatrix<T, n, p> multiply (const FMatrix<T, m, p>& rhs) const;
    template <unsigned p>
    FMatrix<T, n, p> operator* (const FMatrix<T, m, p>& rhs) const;

    /* Comparison Operations */
    bool operator == (const FMatrix<T, n, m>& rhs) const noexcept;

    /* Flat Matrix Array */
    T _fmat[n * m] = {};
};

/** DATA ACCESS METHODS **/

template <typename T, unsigned n, unsigned m>
const T& FMatrix<T,n,m>::at(unsigned i, unsigned j) const
{
    if(i >= n || j >= m) { throw std::out_of_range("Matrix index out of range"); }

    return _fmat[index(i, j)];

}

template <typename T, unsigned n, unsigned m>
T& FMatrix<T,n,m>::at(unsigned i, unsigned j)
{
    if(i >= n || j >= m) { throw std::out_of_range("Matrix index out of range"); }

    return _fmat[index(i, j)];
}

template <typename T, unsigned n, unsigned m>
const T& FMatrix<T,n,m>::at_unsafe(unsigned i, unsigned j) const noexcept
{
    return _fmat[index(i, j)];
}

template <typename T, unsigned n, unsigned m>
T& FMatrix<T,n,m>::at_unsafe(unsigned i, unsigned j) noexcept
{
    return _fmat[index(i, j)];
}

template <typename T, unsigned n, unsigned m>
unsigned FMatrix<T,n,m>::index(unsigned i, unsigned j) const noexcept
{
    return (i * n) + j;
}

/* ARITHMETIC OPERATIONS */

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m> FMatrix<T,n,m>::add(const FMatrix<T,n,m>& rhs) const
{
    FMatrix<T,n,m> result;

    std::transform(begin(), end(), rhs.begin(), result.begin(), std::plus<T>());
    return result;
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m> FMatrix<T,n,m>::operator+(const FMatrix<T,n,m>& rhs) const
{
    return add(rhs);
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m>& FMatrix<T,n,m>::add_into(const FMatrix<T,n,m>& rhs)
{
    std::transform(begin(), end(), rhs.begin(), begin(), std::plus<T>());
    return *this;
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m>& FMatrix<T,n,m>::operator+=(const FMatrix<T,n,m>& rhs)
{
    return add_into(rhs);
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m>& FMatrix<T,n,m>::mult_into(const T& scalar)
{
    std::for_each(begin(), end(), [&scalar](T& entry){ entry *= scalar; });
    return *this;
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m>& FMatrix<T,n,m>::operator*=(const T& scalar)
{
    return mult_into(scalar);
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m> FMatrix<T,n,m>::multiply(const T& scalar) const
{
    FMatrix<T,n,m> result;

    std::transform(begin(), end(), result.begin(), 
                  [&scalar](const T& i){ return scalar * i; });
    return result;
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m> FMatrix<T,n,m>::operator*(const T& scalar) const
{
    return multiply(scalar);
}

template <typename T, unsigned n, unsigned m>
FMatrix<T,n,m> operator*(const T& scalar, const FMatrix<T,n,m>& rhs)
{
    return rhs.multiply(scalar);
}

template <typename T, unsigned n, unsigned m>
template <unsigned p>
FMatrix<T, n, p> FMatrix<T,n,m>::multiply (const FMatrix<T, m, p>& rhs) const
{
    FMatrix<T,n,p> C;

    for (unsigned i = 0; i < n; ++i)
    {
        for (unsigned j = 0; j < p; ++j)
        {
            T sum = 0;
            for (unsigned k = 0; k < m; ++k)
            {
                sum += (*this)[i][k] * rhs[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}

template <typename T, unsigned n, unsigned m>
template <unsigned p>
FMatrix<T, n, p> FMatrix<T,n,m>::operator* (const FMatrix<T, m, p>& rhs) const
{
    return multiply(rhs);
}

/* EQUIVALENCE OPERATIONS */
template <typename T, unsigned n, unsigned m>
bool FMatrix<T,n,m>::operator==(const FMatrix<T,n,m>& rhs) const noexcept
{
    return std::equal(begin(), end(), rhs.begin());
}

#endif // FLAT_MATRIX_CPP_H
