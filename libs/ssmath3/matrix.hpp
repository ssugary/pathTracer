#ifndef SSMATH_MATRIX_HPP
#define SSMATH_MATRIX_HPP

#include <initializer_list>
#pragma once

#include "foward.hpp"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace ssmath3
{

    template<typename T>
    struct Matrix<T, 2>
    {
        T data[2][2];

        constexpr Matrix() noexcept
        {
            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    data[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
                }
            }
        };

        constexpr Matrix(T a, T b, T c, T d) noexcept : data({a, b, c, d}) {};

        constexpr Matrix(std::initializer_list<T> l) 
        {
            assert(l.size() == 4);
            
            auto it = l.begin();

            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    data[i][j] = *it++;
                }
            }
        };
        constexpr Matrix(std::initializer_list<T> l1, std::initializer_list<T> l2) noexcept
        {
            auto it1 = l1.begin();
            auto it2 = l2.begin();
            data[0][0] = *it1++; data[0][1] = *it2++;
            data[1][0] = *it1++; data[1][1] = *it2++;
        };

        constexpr Matrix(const T k) noexcept
        {
            data[0][0] = k;                 data[0][1] = static_cast<T>(0);
            data[1][0] = static_cast<T>(0); data[1][1] = k;
        };

        constexpr Matrix(const Vector<T, 2>& v1, const Vector<T, 2> v2) noexcept;
        constexpr explicit Matrix(const Matrix<T, 3>&) noexcept;
        
        constexpr const T* operator[](const std::size_t i) const
        {
            assert(i < 2);
            return data[i];
        };

        constexpr T* operator[](const std::size_t i)
        {
            assert(i < 2);
            return data[i];
        };

        constexpr Matrix<T, 2>& operator=(const Matrix<T, 2> m) noexcept
        {
            data[0][0] = m[0][0];
            data[0][1] = m[0][1];
            data[1][0] = m[1][0];
            data[1][1] = m[1][1];

            return *this;
        };

        constexpr bool operator==(const Matrix<T, 2> m) const noexcept
        {
            return data[0][0] == m[0][0] &&
                   data[0][1] == m[0][1] &&
                   data[1][0] == m[1][0] &&
                   data[1][1] == m[1][1];
        };

        constexpr bool operator!=(const Matrix<T, 2> m) const noexcept
        {
            return data[0][0] != m[0][0] ||
                   data[0][1] != m[0][1] ||
                   data[1][0] != m[1][0] ||
                   data[1][1] != m[1][1];
        };

        constexpr Matrix<T, 2> operator+() const noexcept 
        {
            Matrix<T, 2> res;

            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    res[i][j] = data[i][j];
                }
            }
            return res;
        }
        
        constexpr Matrix<T, 2> operator-() const noexcept 
        {
            Matrix<T, 2> res;

            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    res[i][j] = -data[i][j];
                }
            }
            return res;
        }

        constexpr Matrix<T, 2> operator+(const Matrix<T, 2> m) const noexcept
        {
            return Matrix<T, 2> (
                                 data[0][0] + m[0][0],
                                 data[0][1] + m[0][1],
                                 data[1][0] + m[1][0],
                                 data[1][1] + m[1][1]
                                );
        };

        constexpr Matrix<T, 2> operator-(const Matrix<T, 2> m) const noexcept
        {
            return Matrix<T, 2> (
                                 data[0][0] - m[0][0],
                                 data[0][1] - m[0][1],
                                 data[1][0] - m[1][0],
                                 data[1][1] - m[1][1]
                                );
        };

        constexpr Matrix<T, 2> operator*(const T k) const noexcept
        {
            return Matrix<T, 2> (
                                 data[0][0] * k,
                                 data[0][1] * k,
                                 data[1][0] * k,
                                 data[1][1] * k
                                );
        };

        constexpr Matrix<T, 2> operator/(const T k) const noexcept
        {
            return Matrix<T, 2> (
                                 data[0][0] / k,
                                 data[0][1] / k,
                                 data[1][0] / k,
                                 data[1][1] / k
                                );
        };

        constexpr Matrix<T, 2>& operator+=(const Matrix<T, 2> m) noexcept
        {
            
            data[0][0] += m[0][0];
            data[0][1] += m[0][1];
            data[1][0] += m[1][0];
            data[1][1] += m[1][1];

            return *this;
                                
        };

        constexpr Matrix<T, 2>& operator-=(const Matrix<T, 2> m) noexcept
        {
            data[0][0] -= m[0][0];
            data[0][1] -= m[0][1];
            data[1][0] -= m[1][0];
            data[1][1] -= m[1][1];

            return *this;
        };

        constexpr Matrix<T, 2>& operator*=(const T k) noexcept
        {
            data[0][0] *= k;
            data[0][1] *= k;
            data[1][0] *= k;
            data[1][1] *= k;

            return *this;
        };

        constexpr Matrix<T, 2>& operator/=(const T k) noexcept
        {
            data[0][0] /= k;
            data[0][1] /= k;
            data[1][0] /= k;
            data[1][1] /= k;

            return *this;
        };

        constexpr Matrix<T, 2>& operator*=(const Matrix<T, 2>& m) noexcept
        {
            Matrix<T, 2> res;

            for (std::size_t i = 0; i < 2; ++i) 
            {
                for (std::size_t j = 0; j < 2; ++j) 
                {
                    T sum = 0;

                    for (std::size_t k = 0; k < 2; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }

            *this = res;
            return *this;
        };
        constexpr Matrix<T, 2> operator*(const Matrix<T, 2>& m) const noexcept
        {
            Matrix<T, 2> res;

            for (std::size_t i{0}; i < 2; ++i) 
            {
                for (std::size_t j{0}; j < 2; ++j) 
                {
                    T sum{0};

                    for (std::size_t k{0}; k < 2; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }
            
            return res;
        };

        constexpr Vector<T, 2> operator*(const Vector<T, 2>&) const noexcept;
        constexpr Point<T, 2>  operator*(const Point<T, 2> &) const noexcept; 

        constexpr Matrix<T, 2>& transpose() noexcept
        {
            Matrix<T, 2> res;

            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    res[i][j] = data[j][i];
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 2>& inverse() noexcept
        {
            
            Matrix<T, 2> res{data[1][1], -data[0][1], -data[1][0], data[0][0]};

            T d = data[1][1] * data[0][0] - data[1][0] * data[0][1];

            assert(d != static_cast<T>(0));

            res *= 1/(d);

            *this = res;
            return *this;
        };

        friend std::ostream &operator<<(std::ostream &os, const Matrix<T, 2> &m) noexcept 
        {
            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    os << m[i][j] << ' ';
                }
            }
            return os;
        }
        
        friend std::istream &operator>>(std::istream &is, Matrix<T, 2>& m) noexcept 
        {

            for(std::size_t i{0}; i < 2; ++i)
            {
                for(std::size_t j{0}; j < 2; ++j)
                {
                    is >> m[i][j];
                }
            }
            return is;
        }
    };
    template<typename T>
    struct Matrix<T, 3>
    {
        T data[3][3];

        constexpr Matrix() noexcept
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    data[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
                }
            }
        };

        constexpr Matrix(std::initializer_list<T> l) 
        {
            assert(l.size() == 9);
            
            auto it = l.begin();

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    data[i][j] = *it++;
                }
            }
        };

        constexpr Matrix(const T k) noexcept
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    data[i][j] = (i == j) ? k : static_cast<T>(0);
                }
            }
        };
        constexpr Matrix(const Vector<T, 2> v1, const Vector<T, 2> v2) noexcept;
        constexpr Matrix(const Vector<T, 3>& v1, const Vector<T, 3> v2, const Vector<T, 3>& v3) noexcept;
        constexpr Matrix(const Matrix<T, 3>&) noexcept = default;
        constexpr explicit Matrix(const Matrix<T, 2>&) noexcept;
        constexpr explicit Matrix(const Matrix<T, 4>&) noexcept;
        
        constexpr const T* operator[](const std::size_t i) const
        {
            assert(i < 3);
            return data[i];
        };

        constexpr T* operator[](const std::size_t i)
        {
            assert(i < 3);
            return data[i];
        };

        constexpr Matrix<T, 3>& operator=(const Matrix<T, 3> m) noexcept
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    data[i][j] = m[i][j];
                }
            }
            return *this;
        };

        constexpr bool operator==(const Matrix<T, 3> m) const noexcept
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    if(data[i][j] != m[i][j])
                        return false;
                }
            }

            return true;
        };

        constexpr bool operator!=(const Matrix<T, 3> m) const noexcept
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    if(data[i][j] == m[i][j])
                        return false;
                }
            }
            
            return true;
        };

        constexpr Matrix<T, 3> operator+() const noexcept 
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j];
                }
            }
            return res;
        }
        
        constexpr Matrix<T, 3> operator-() const noexcept 
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = -data[i][j];
                }
            }
            return res;
        }

        constexpr Matrix<T, 3> operator+(const Matrix<T, 3> m) const noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] + m[i][j];
                }
            }
            return res;
        };

        constexpr Matrix<T, 3> operator-(const Matrix<T, 3> m) const noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] - m[i][j];
                }
            }
            return res;
        };

        constexpr Matrix<T, 3> operator*(const T k) const noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] * k;
                }
            }
            return res;
        };

        constexpr Matrix<T, 3> operator/(const T k) const noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] / k;
                }
            }
            return res;
        };

        constexpr Matrix<T, 3>& operator+=(const Matrix<T, 3> m) noexcept
        {
            
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] + m[i][j];
                }
            }
            *this = res;
            return *this;
                                
        };

        constexpr Matrix<T, 3>& operator-=(const Matrix<T, 3> m) noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] - m[i][j];
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 3>& operator*=(const T k) noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] * k;
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 3>& operator/=(const T k) noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[i][j] / k;
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 3>& operator*=(const Matrix<T, 3>& m) noexcept
        {
            Matrix<T, 3> res;

            for (std::size_t i = 0; i < 3; ++i) 
            {
                for (std::size_t j = 0; j < 3; ++j) 
                {
                    T sum = 0;

                    for (std::size_t k = 0; k < 3; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }

            *this = res;
            return *this;
        };
        constexpr Matrix<T, 3> operator*(const Matrix<T, 3>& m) const noexcept
        {
            Matrix<T, 3> res;

            for (std::size_t i{0}; i < 3; ++i) 
            {
                for (std::size_t j{0}; j < 3; ++j) 
                {
                    T sum{0};

                    for (std::size_t k{0}; k < 3; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }
            
            return res;
        };

        constexpr Vector<T, 3> operator*(const Vector<T, 3>&) const noexcept;
        constexpr Point<T, 3>  operator*(const Point<T, 3> &) const noexcept;
        constexpr Normal<T, 3> operator*(const Normal<T, 3>&) const noexcept;

        constexpr Matrix<T, 3>& transpose() noexcept
        {
            Matrix<T, 3> res;

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    res[i][j] = data[j][i];
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 3>& inverse() noexcept
        {
            T det = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
                    data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
                    data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);

            assert(det != static_cast<T>(0));

            Matrix<T, 3> res;

            res[0][0] = (data[1][1] * data[2][2] - data[1][2] * data[2][1]) / det;
            res[0][1] = (data[0][2] * data[2][1] - data[0][1] * data[2][2]) / det;
            res[0][2] = (data[0][1] * data[1][2] - data[0][2] * data[1][1]) / det;

            res[1][0] = (data[1][2] * data[2][0] - data[1][0] * data[2][2]) / det;
            res[1][1] = (data[0][0] * data[2][2] - data[0][2] * data[2][0]) / det;
            res[1][2] = (data[0][2] * data[1][0] - data[0][0] * data[1][2]) / det;

            res[2][0] = (data[1][0] * data[2][1] - data[1][1] * data[2][0]) / det;
            res[2][1] = (data[0][1] * data[2][0] - data[0][0] * data[2][1]) / det;
            res[2][2] = (data[0][0] * data[1][1] - data[0][1] * data[1][0]) / det;
            
            *this = res;
            return *this;

        };

        friend std::ostream &operator<<(std::ostream &os, const Matrix<T, 3> &m) noexcept 
        {
            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    os << m[i][j] << ' ';
                }
            }
            return os;
        }
        
        friend std::istream &operator>>(std::istream &is, Matrix<T, 3>& m) noexcept 
        {

            for(std::size_t i{0}; i < 3; ++i)
            {
                for(std::size_t j{0}; j < 3; ++j)
                {
                    is >> m[i][j];
                }
            }
            return is;
        }
    };
    
    template<typename T>
    struct Matrix<T, 4>
    {
        T data[4][4];

        constexpr Matrix() noexcept
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    data[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
                }
            }
        };

        constexpr Matrix(std::initializer_list<T> l) 
        {
            assert(l.size() == 16);
            
            auto it = l.begin();

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    data[i][j] = *it++;
                }
            }
        };

        constexpr Matrix(const T k) noexcept
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    data[i][j] = (i == j) ? k : static_cast<T>(0);
                }
            }
        };
        constexpr Matrix(const Matrix<T, 4>&) noexcept = default;
        constexpr explicit Matrix(const Vector<T, 3>& v1, const Vector<T, 3> v2, const Vector<T, 3>& v3) noexcept;
        constexpr explicit Matrix(const Matrix<T, 2>&) noexcept;
        constexpr explicit Matrix(const Matrix<T, 3>&) noexcept;
        
        constexpr const T* operator[](const std::size_t i) const
        {
            assert(i < 4);
            return data[i];
        };

        constexpr T* operator[](const std::size_t i)
        {
            assert(i < 4);
            return data[i];
        };

        constexpr Matrix<T, 4>& operator=(const Matrix<T, 4> m) noexcept
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    data[i][j] = m[i][j];
                }
            }
            return *this;
        };

        constexpr bool operator==(const Matrix<T, 4> m) const noexcept
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    if(data[i][j] != m[i][j])
                        return false;
                }
            }

            return true;
        };

        constexpr bool operator!=(const Matrix<T, 4> m) const noexcept
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    if(data[i][j] == m[i][j])
                        return false;
                }
            }
            
            return true;
        };

        constexpr Matrix<T, 4> operator+() const noexcept 
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j];
                }
            }
            return res;
        }
        
        constexpr Matrix<T, 4> operator-() const noexcept 
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = -data[i][j];
                }
            }
            return res;
        }

        constexpr Matrix<T, 4> operator+(const Matrix<T, 4> m) const noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] + m[i][j];
                }
            }
            return res;
        };

        constexpr Matrix<T, 4> operator-(const Matrix<T, 4> m) const noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] - m[i][j];
                }
            }
            return res;
        };

        constexpr Matrix<T, 4> operator*(const T k) const noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] * k;
                }
            }
            return res;
        };

        constexpr Matrix<T, 4> operator/(const T k) const noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] / k;
                }
            }
            return res;
        };

        constexpr Matrix<T, 4>& operator+=(const Matrix<T, 4> m) noexcept
        {
            
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] + m[i][j];
                }
            }
            *this = res;
            return *this;
                                
        };

        constexpr Matrix<T, 4>& operator-=(const Matrix<T, 4> m) noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] - m[i][j];
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 4>& operator*=(const T k) noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] * k;
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 4>& operator/=(const T k) noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[i][j] / k;
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 4>& operator*=(const Matrix<T, 4>& m) noexcept
        {
            Matrix<T, 4> res;

            for (std::size_t i = 0; i < 4; ++i) 
            {
                for (std::size_t j = 0; j < 4; ++j) 
                {
                    T sum = 0;

                    for (std::size_t k = 0; k < 4; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }

            *this = res;
            return *this;
        };
        constexpr Matrix<T, 4> operator*(const Matrix<T, 4>& m) const noexcept
        {
            Matrix<T, 4> res;

            for (std::size_t i{0}; i < 4; ++i) 
            {
                for (std::size_t j{0}; j < 4; ++j) 
                {
                    T sum{0};

                    for (std::size_t k{0}; k < 4; ++k) 
                    {
                        sum += data[i][k] * m[k][j];
                    }

                    res[i][j] = sum; 
                }
            }
            
            return res;
        };

        constexpr Vector<T, 4> operator*(const Vector<T, 4>&) const noexcept;
        constexpr Point<T, 4>  operator*(const Point<T, 4> &) const noexcept;
        constexpr Normal<T, 4> operator*(const Normal<T, 4>&) const noexcept;

        constexpr Matrix<T, 4>& transpose() noexcept
        {
            Matrix<T, 4> res;

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    res[i][j] = data[j][i];
                }
            }
            *this = res;
            return *this;
        };

        constexpr Matrix<T, 4>& inverse() noexcept
        {
            T s0 = data[0][0] * data[1][1] - data[0][1] * data[1][0];
            T s1 = data[0][0] * data[1][2] - data[0][2] * data[1][0];
            T s2 = data[0][0] * data[1][3] - data[0][3] * data[1][0];
            T s3 = data[0][1] * data[1][2] - data[0][2] * data[1][1];
            T s4 = data[0][1] * data[1][3] - data[0][3] * data[1][1];
            T s5 = data[0][2] * data[1][3] - data[0][3] * data[1][2];

            T c0 = data[2][0] * data[3][1] - data[2][1] * data[3][0];
            T c1 = data[2][0] * data[3][2] - data[2][2] * data[3][0];
            T c2 = data[2][0] * data[3][3] - data[2][3] * data[3][0];
            T c3 = data[2][1] * data[3][2] - data[2][2] * data[3][1];
            T c4 = data[2][1] * data[3][3] - data[2][3] * data[3][1];
            T c5 = data[2][2] * data[3][3] - data[2][3] * data[3][2];

            T det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
            
            assert(det != static_cast<T>(0));
            

            Matrix<T, 4> inv;

            inv[0][0] = ( data[1][1] * c5 - data[1][2] * c4 + data[1][3] * c3) / det;
            inv[0][1] = (-data[0][1] * c5 + data[0][2] * c4 - data[0][3] * c3) / det;
            inv[0][2] = ( data[3][1] * s5 - data[3][2] * s4 + data[3][3] * s3) / det;
            inv[0][3] = (-data[2][1] * s5 + data[2][2] * s4 - data[2][3] * s3) / det;

            inv[1][0] = (data[1][0] * c5 + data[1][2] * c2 - data[1][3] * c1) / det;
            inv[1][1] = (data[0][0] * c5 - data[0][2] * c2 + data[0][3] * c1) / det;
            inv[1][2] = (data[3][0] * s5 + data[3][2] * s2 - data[3][3] * s1) / det;
            inv[1][3] = (data[2][0] * s5 - data[2][2] * s2 + data[2][3] * s1) / det;

            inv[2][0] = ( data[1][0] * c4 - data[1][1] * c2 + data[1][3] * c0) / det;
            inv[2][1] = (-data[0][0] * c4 + data[0][0] * c2 - data[0][3] * c0) / det; 
            inv[2][2] = ( data[3][0] * s4 - data[3][1] * s2 + data[3][3] * s0) / det;
            inv[2][3] = (-data[2][0] * s4 + data[2][1] * s2 - data[2][3] * s0) / det;

            inv.data[2][1] = (-data[0][0] * c4 + data[0][1] * c2 - data[0][3] * c0) / det;
            inv.data[3][0] = (-data[1][0] * c3 + data[1][1] * c1 - data[1][2] * c0) / det;
            inv.data[3][1] = ( data[0][0] * c3 - data[0][1] * c1 + data[0][2] * c0) / det;
            inv.data[3][2] = (-data[3][0] * s3 + data[3][1] * s1 - data[3][2] * s0) / det;
            inv.data[3][3] = ( data[2][0] * s3 - data[2][1] * s1 + data[2][2] * s0) / det;

            *this = inv;
            return *this;

        };

        friend std::ostream &operator<<(std::ostream &os, const Matrix<T, 4> &m) noexcept 
        {
            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    os << m[i][j] << ' ';
                }
            }
            return os;
        }

        friend std::istream &operator>>(std::istream &is, Matrix<T, 4>& m) noexcept 
        {

            for(std::size_t i{0}; i < 4; ++i)
            {
                for(std::size_t j{0}; j < 4; ++j)
                {
                    is >> m[i][j];
                }
            }
            return is;
        }
        
    };

}; //< namespace ssmath

#endif //< SSMATH_MATRIX_HPP