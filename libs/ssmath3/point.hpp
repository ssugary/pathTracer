#ifndef SSMATH_POINT_HPP
#define SSMATH_POINT_HPP

#include <cassert>
#pragma once

#include "foward.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace ssmath3
{

    template<typename T>
    struct Point<T, 2>
    {
        T x, y;

        constexpr Point() noexcept : x(), y(){};
        constexpr Point(T k) noexcept : x(k), y(k) {};
        constexpr Point(T x, T y) noexcept : x(x), y(y){};
        constexpr Point(const Point<T, 2>&) noexcept = default;
        template<typename U>
        constexpr explicit Point(const Point<U, 2>&  p) noexcept;
        template<typename U>
        constexpr explicit Point(const Vector<U, 2>& v) noexcept;

        constexpr explicit Point(const Vector<T, 2>&)noexcept;
        constexpr explicit Point(const Point<T, 4>&) noexcept;
        constexpr explicit Point(const Point<T, 3>&) noexcept;

        constexpr const T& operator[](const std::size_t i) const noexcept
        {
            assert(i < 2);

            return i == 0 ? x : y;
        }

        constexpr T& operator[](const std::size_t i) noexcept
        {
            assert(i < 2);

            return i == 0 ? x : y;
        }
    
        constexpr Point<T, 2>& operator=(const Point<T, 2>& p) noexcept
        {
            this->x = p.x;
            this->y = p.y;

            return *this;
        }

        constexpr bool operator==(const Point<T, 2>& p) const noexcept
        {
            return this->x == p.x && this->y == p.y;
        }

        constexpr bool operator!=(const Point<T, 2>& p) const noexcept
        {
            return this->x != p.x || this->y != p.y;
        }

        constexpr Point<T, 2> operator+() const noexcept 
        {
            return Point<T, 2>(x, y);
        }
        
        constexpr Point<T, 2> operator-() const noexcept 
        {
            return Point<T, 2>(-x, -y);
        }

        constexpr Point<T, 2> operator+(const Point<T, 2>& p) const noexcept
        {
            return Point<T, 2>(x + p.x, y + p.y);
        }

        constexpr Point<T, 2>& operator+=(const Point<T, 2>& p) noexcept
        {
            this->x += p.x;
            this->y += p.y;
            
            return *this;
        }

        constexpr Point<T, 2> operator+(const Vector<T, 2>& v) const noexcept;
        constexpr Point<T, 2>& operator+=(const Vector<T, 2>& v) noexcept;
        constexpr Point<T, 2> operator-(const Vector<T, 2>& v) const noexcept;
        constexpr Point<T, 2>& operator-=(const Vector<T, 2>& v) noexcept;
        constexpr Vector<T, 2> operator-(const Point<T, 2>& p) const noexcept;

        constexpr Point<T, 2>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;

            return *this;
        }
        constexpr Point<T, 2>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;

            return *this;
        }
        
        constexpr Point<T, 2> operator*(T t) const noexcept
        {
            return Point<T, 2>(x * t, y * t);
        }
        constexpr Point<T, 2> operator/(T t) const noexcept
        {
            return Point<T, 2>(x / t, y / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Point<T, 2> &p) noexcept 
        {
            return os << p.x << ' ' << p.y;
        }
        friend std::istream &operator>>(std::istream &is, Point<T, 2>& p) noexcept 
        {
            return is >> p.x >> p.y;
        }
    };

    template<typename T>
    struct Point<T, 3>
    {
        union
        {
            __extension__ struct {T x, y, z;};
            __extension__ struct {T r, g, b;};
        };
        constexpr Point() noexcept : x(), y(), z() {}
        constexpr Point(T k) noexcept : x(k), y(k), z(k) {};
        constexpr Point(T x, T y, T z) noexcept : x(x), y(y), z(z){};
        constexpr Point(const Point<T, 3>&) noexcept = default;

        template<typename U>
        constexpr explicit Point(const Point<U, 3>&  p) noexcept;
        template<typename U>
        constexpr explicit Point(const Vector<U, 3>& v) noexcept;
        template<typename U>
        constexpr explicit Point(const Normal<U, 3>& n) noexcept;

        constexpr explicit Point(const Vector<T, 3>&) noexcept;
        constexpr explicit Point(const Normal<T, 3>&) noexcept;
        constexpr explicit Point(const Point<T, 4> &) noexcept;
        constexpr explicit Point(const Point<T, 2> &) noexcept;

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
    
        constexpr Point<T, 3>& operator=(const Point<T, 3>& p) noexcept
        {
            
            this->x = p.x;
            this->y = p.y;
            this->z = p.z;

            return *this;
        }

        constexpr bool operator==(const Point<T, 3>& p) const noexcept
        {
            return this->x == p.x && this->y == p.y && this->z == p.z;
        }

        constexpr bool operator!=(const Point<T, 3>& p) const noexcept
        {
            return this->x != p.x || this->y != p.y || this->z != p.z;
        }

        constexpr Point<T, 3> operator+() const noexcept 
        {
            return Point<T, 3>(x, y, z);
        }
        
        constexpr Point<T, 3> operator-() const noexcept 
        {
            return Point<T, 3>(-x, -y, -z);
        }

        constexpr Point<T, 3> operator+(const Point<T, 3>& p) const noexcept
        {
            return Point<T, 3>(x + p.x, y + p.y, z + p.z);
        }

        constexpr Point<T, 3>& operator+=(const Point<T, 3>& p) noexcept
        {
            this->x += p.x;
            this->y += p.y;
            this->z += p.z;
            
            return *this;
        }

        constexpr Point<T, 3> operator+(const Vector<T, 3>& v) const noexcept;
        constexpr Point<T, 3>& operator+=(const Vector<T, 3>& v) noexcept;
        constexpr Point<T, 3> operator-(const Vector<T, 3>& v) const noexcept;
        constexpr Point<T, 3>& operator-=(const Vector<T, 3>& v) noexcept;
        constexpr Point<T, 3> operator+(const Normal<T, 3>& n) const noexcept;
        constexpr Point<T, 3>& operator+=(const Normal<T, 3>& n) noexcept;
        constexpr Point<T, 3> operator-(const Normal<T, 3>& n) const noexcept;
        constexpr Point<T, 3>& operator-=(const Normal<T, 3>& n) noexcept;
        constexpr Vector<T, 3> operator-(const Point<T, 3>& p) const noexcept;

        constexpr Point<T, 3>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;

            return *this;
        }
        constexpr Point<T, 3>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;

            return *this;
        }
        
        constexpr Point<T, 3> operator*(T t) const noexcept
        {
            return Point<T, 3>(x * t, y * t, z * t);
        }
        constexpr Point<T, 3> operator/(T t) const noexcept
        {
            return Point<T, 3>(x / t, y / t, z / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Point<T, 3> &p) noexcept 
        {
            return os << p.x << ' ' << p.y << ' ' << p.z;
        }
        friend std::istream &operator>>(std::istream &is, Point<T, 3>& p) noexcept 
        {
            return is >> p.x >> p.y >> p.z;
        }
    };

    template<typename T>
    struct Point<T, 4>
    {
        union 
        {
            __extension__ struct {T x, y, z, w;};
            __extension__ struct {T r, g, b, a;};
        };

        constexpr Point() noexcept : x(), y(), z(), w() {}
        constexpr Point(T k) noexcept : x(k), y(k), z(k), w(k) {};
        constexpr Point(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w){};
        constexpr Point(const Point<T, 4>& p) noexcept : x(p.x), y(p.y), z(p.z), w(p.w) {};
        constexpr explicit Point(const Point<T, 3>&) noexcept;
        constexpr explicit Point(const Vector<T, 4>&)noexcept;
        constexpr explicit Point(const Normal<T, 4>&)noexcept;

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
    
        constexpr Point<T, 4>& operator=(const Point<T, 4>& p) noexcept
        {
            this->x = p.x;
            this->y = p.y;
            this->z = p.z;
            this->w = p.w;

            return *this;
        }

        constexpr bool operator==(const Point<T, 4>& p) const noexcept
        {
            return this->x == p.x && this->y == p.y && this->z == p.z && this->w == p.w;
        }

        constexpr bool operator!=(const Point<T, 4>& p) const noexcept
        {
            return this->x != p.x || this->y != p.y || this->z != p.z || this->w != p.w;
        }

        constexpr Point<T, 4> operator+() const noexcept 
        {
            return Point<T, 4>(x, y, z, w);
        }

        constexpr Point<T, 4> operator-() const noexcept 
        {
            return Point<T, 4>(-x, -y, -z, -w);
        }

        constexpr Point<T, 4> operator+(const Point<T, 4>& p) const noexcept
        {
            return Point<T, 4>(x + p.x, y + p.y, z + p.z, w + p.w);
        }

        constexpr Point<T, 4>& operator+=(const Point<T, 4>& p) noexcept
        {
            this->x += p.x;
            this->y += p.y;
            this->z += p.z;
            this->w += p.w;
            
            return *this;
        }

        constexpr Point<T, 4> operator+(const Vector<T, 4>& v) const noexcept;
        constexpr Point<T, 4>& operator+=(const Vector<T, 4>& v) noexcept;
        constexpr Point<T, 4> operator-(const Vector<T, 4>& v) const noexcept;
        constexpr Point<T, 4>& operator-=(const Vector<T, 4>& v) noexcept;
        constexpr Point<T, 4> operator+(const Normal<T, 4>& n) const noexcept;
        constexpr Point<T, 4>& operator+=(const Normal<T, 4>& n) noexcept;
        constexpr Point<T, 4> operator-(const Normal<T, 4>& n) const noexcept;
        constexpr Point<T, 4>& operator-=(const Normal<T, 4>& n) noexcept;
        constexpr Vector<T, 4> operator-(const Point<T, 4>& p) const noexcept;

        constexpr Point<T, 4>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;
            this->w *= t;

            return *this;
        }
        constexpr Point<T, 4>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;
            w /= t;

            return *this;
        }
        
        constexpr Point<T, 4> operator*(T t) const noexcept
        {
            return Point<T, 4>(x * t, y * t, z * t, w * t);
        }
        constexpr Point<T, 4> operator/(T t) const noexcept
        {
            return Point<T, 4>(x / t, y / t, z / t, w / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Point<T, 4> &p) noexcept 
        {
            return os << p.x << ' ' << p.y << ' ' << p.z << ' ' << p.w;
        }
        friend std::istream &operator>>(std::istream &is, Point<T, 4>& p) noexcept 
        {
            return is >> p.x >> p.y >> p.z >> p.w;
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
} // namespace ssmath3
#endif //< SSMATH_POINT_HPP