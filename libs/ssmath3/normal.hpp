#ifndef SSMATH_NORMAL_HPP
#define SSMATH_NORMAL_HPP

#include <cassert>
#pragma once

#include "foward.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace ssmath3
{
    template<typename T>
    struct Normal<T, 3>
    {
        union
        {
            __extension__ struct {T x, y, z;};
            __extension__ struct {T r, g, b;};
        };
        constexpr Normal() noexcept : x(), y(), z() {}
        constexpr Normal(T k) noexcept : x(k), y(k), z(k) {};
        constexpr Normal(T x, T y, T z) noexcept : x(x), y(y), z(z){};
        constexpr Normal(const Normal<T, 3>&) noexcept = default;
        constexpr explicit Normal(const Point<T, 3>&) noexcept;
        constexpr explicit Normal(const Vector<T, 3>&)noexcept;
        constexpr explicit Normal(const Normal<T, 4>&)noexcept;

        constexpr const T& operator[](const std::size_t i) const noexcept
        {
            assert(i < 3);

            if(i == 0) return x;
            if(i == 1) return y;
            return z;
        }

        constexpr T& operator[](const std::size_t i) noexcept
        {
            assert(i < 3);

            if(i == 0) return x;
            if(i == 1) return y;
            return z;
        }
    
        constexpr Normal<T, 3>& operator=(const Normal<T, 3>& n) noexcept
        {
            this->x = n.x;
            this->y = n.y;
            this->z = n.z;

            return *this;
        }

        constexpr bool operator==(const Normal<T, 3>& n) const noexcept
        {
            return this->x == n.x && this->y == n.y && this->z == n.z;
        }

        constexpr bool operator!=(const Normal<T, 3>& n) const noexcept
        {
            return this->x != n.x || this->y != n.y || this->z != n.z;
        }

        constexpr Normal<T, 3> operator+() const noexcept 
        {
            return Normal<T, 3>(x, y, z);
        }
        
        constexpr Normal<T, 3> operator-() const noexcept 
        {
            return Normal<T, 3>(-x, -y, -z);
        }

        constexpr Normal<T, 3> operator+(const Normal<T, 3>& n) const noexcept
        {
            return Normal<T, 3>(x + n.x, y + n.y, z + n.z);
        }

        constexpr Normal<T, 3>& operator+=(const Normal<T, 3>& n) noexcept
        {
            this->x += n.x;
            this->y += n.y;
            this->z += n.z;
            
            return *this;
        }

        constexpr Normal<T, 3> operator-(const Normal<T, 3>& n) const noexcept
        {
            return Normal<T, 3>(x - n.x, y - n.y, z - n.z);
        }

        constexpr Normal<T, 3>& operator-=(const Normal<T, 3>& n) noexcept
        {
            x -= n.x;
            y -= n.y;
            z -= n.z;

            return *this;
        }

        constexpr Normal<T, 3> operator+(const Vector<T, 3>&) const noexcept;
        constexpr Normal<T, 3> operator-(const Vector<T, 3>&) const noexcept;
        constexpr Normal<T, 3>& operator+=(const Vector<T, 3>&) noexcept;
        constexpr Normal<T, 3>& operator-=(const Vector<T, 3>&) noexcept;

        constexpr Normal<T, 3>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;

            return *this;
        }
        constexpr Normal<T, 3>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;

            return *this;
        }
        
        constexpr Normal<T, 3> operator*(T t) const noexcept
        {
            return Normal<T, 3>(x * t, y * t, z * t);
        }
        constexpr Normal<T, 3> operator/(T t) const noexcept
        {
            return Normal<T, 3>(x / t, y / t, z / t);
        }
        constexpr void normalize() noexcept 
        {
            T length = static_cast<T>(std::sqrt(x * x + y * y + z * z));
            if(length == 0.f)
                return;
            x /= length;
            y /= length;
            z /= length;
        }

        friend std::ostream &operator<<(std::ostream &os, const Normal<T, 3> &n) noexcept 
        {
            return os << n.x << ' ' << n.y << ' ' << n.z;
        }
        friend std::istream &operator>>(std::istream &is, Normal<T, 3>& n) noexcept 
        {
            return is >> n.x >> n.y >> n.z;
        }
    };

    template<typename T>
    struct Normal<T, 4>
    {
        union 
        {
            __extension__ struct {T x, y, z, w;};
            __extension__ struct {T r, g, b, a;};
        };

        constexpr Normal() noexcept : x(), y(), z(), w() {}
        constexpr Normal(T k) noexcept : x(k), y(k), z(k), w(k) {};
        constexpr Normal(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w){};
        constexpr Normal(const Normal<T, 4>&) = default;
        constexpr explicit Normal(const Point<T, 4>&)  noexcept;
        constexpr explicit Normal(const Vector<T, 4>&) noexcept;
        constexpr explicit Normal(const Normal<T, 3>&) noexcept;

        constexpr const T& operator[](const std::size_t i) const noexcept
        {
            assert(i < 4);

            if(i == 0) return x;
            if(i == 1) return y;
            if(i == 2) return z;
            return w;
        }

        constexpr T& operator[](const std::size_t i) noexcept
        {
            assert(i < 4);

            if(i == 0) return x;
            if(i == 1) return y;
            if(i == 2) return z;
            return w;
        }
    
        constexpr Normal<T, 4>& operator=(const Normal<T, 4>& n) noexcept
        {
            this->x = n.x;
            this->y = n.y;
            this->z = n.z;
            this->w = n.w;

            return *this;
        }

        constexpr bool operator==(const Normal<T, 4>& n) const noexcept
        {
            return this->x == n.x && this->y == n.y && this->z == n.z && this->w == n.w;
        }

        constexpr bool operator!=(const Normal<T, 4>& n) const noexcept
        {
            return this->x != n.x || this->y != n.y || this->z != n.z || this->w != n.w;
        }

        constexpr Normal<T, 4> operator+() const noexcept 
        {
            return Normal<T, 4>(x, y, z, w);
        }
        
        constexpr Normal<T, 4> operator-() const noexcept 
        {
            return Normal<T, 4>(-x, -y, -z, -w);
        }

        constexpr Normal<T, 4> operator+(const Normal<T, 4>& n) const noexcept
        {
            return Normal<T, 4>(x + n.x, y + n.y, z + n.z, w + n.w);
        }

        constexpr Normal<T, 4>& operator+=(const Normal<T, 4>& n) noexcept
        {
            this->x += n.x;
            this->y += n.y;
            this->z += n.z;
            this->w += n.w;
            
            return *this;
        }

        constexpr Normal<T, 4> operator-(const Normal<T, 4>& n) const noexcept
        {
            return Normal<T, 4>(x - n.x, y - n.y, z - n.z, w - n.w);
        }

        constexpr Normal<T, 4>& operator-=(const Normal<T, 4>& n) noexcept
        {
            x -= n.x;
            y -= n.y;
            z -= n.z;
            w -= n.w;

            return *this;
        }


        constexpr Normal<T, 4>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;
            this->w *= t;

            return *this;
        }
        constexpr Normal<T, 4>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;
            w /= t;

            return *this;
        }
        
        constexpr Normal<T, 4> operator*(T t) const noexcept
        {
            return Normal<T, 4>(x * t, y * t, z * t, w * t);
        }
        constexpr Normal<T, 4> operator/(T t) const noexcept
        {
            return Normal<T, 4>(x / t, y / t, z / t, w / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Normal<T, 4> &n) noexcept 
        {
            return os << n.x << ' ' << n.y << ' ' << n.z << ' ' << n.w;
        }
        friend std::istream &operator>>(std::istream &is, Normal<T, 4>& n) noexcept 
        {
            return is >> n.x >> n.y >> n.z >> n.w;
        }


    };

    // inline double distance_sqr(const Point& p1, const Point& p2) noexcept
    // {
    //     return (p1 - p2).sqr_length(); 
    // }

    // inline double distance(const Point& p1, const Point& p2) noexcept
    // {
    //     return (p1 - p2).length();
    // }
    // inline Point swap(Point p)
    // {
    //     return Point(p.y, p.x);
    // }
} // namespace ssmath

#endif //< SSMATH_NORMAL_HPP