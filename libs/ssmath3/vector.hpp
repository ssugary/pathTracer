#ifndef SSMATH_VECTOR_HPP
#define SSMATH_VECTOR_HPP

#include <cassert>
#pragma once

#include "foward.hpp"

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace ssmath3 
{
    template<typename T>
    struct Vector<T, 2>
    {
        T x, y;

        constexpr Vector() noexcept : x(), y(){};
        constexpr Vector(T k) noexcept: x(k), y(k) {};
        constexpr Vector(T x, T y) noexcept : x(x), y(y){};
        constexpr Vector(const Vector<T, 2>& v) noexcept : x(v.x), y(v.y) {};

        template<typename U>
        constexpr explicit Vector(const Point<U, 2>& p)noexcept;
        template<typename U>
        constexpr explicit Vector(const Vector<U, 2>& v)noexcept;

        constexpr explicit Vector(const Vector<T, 4>&) noexcept;
        constexpr explicit Vector(const Vector<T, 3>&) noexcept;
        constexpr explicit Vector(const Point<T, 2> &) noexcept;

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
    
        constexpr Vector<T, 2> operator+() const noexcept 
        {
            return Vector<T, 2>(x, y);
        }
        constexpr Vector<T, 2> operator-() const noexcept 
        {
            return Vector<T, 2>(-x, -y);
        }

        constexpr Vector<T, 2>& operator=(const Vector<T, 2>& v) noexcept
        {
            this->x = v.x;
            this->y = v.y;

            return *this;
        }

        constexpr bool operator==(const Vector<T, 2>& v) const noexcept
        {
            return this->x == v.x && this->y == v.y;
        }

        constexpr bool operator!=(const Vector<T, 2>& v) const noexcept
        {
            return this->x != v.x || this->y != v.y;
        }


        constexpr Vector<T, 2> operator+(const Vector<T, 2>& v) const noexcept
        {
            return Vector<T, 2>(x + v.x, y + v.y);
        }

        constexpr Vector<T, 2>& operator+=(const Vector<T, 2>& v) noexcept
        {
            this->x += v.x;
            this->y += v.y;
            
            return *this;
        }

    
        constexpr Vector<T, 2>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;

            return *this;
        }
        constexpr Vector<T, 2>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;

            return *this;
        }
        
        constexpr Vector<T, 2> operator*(T t) const noexcept
        {
            return Vector<T, 2>(x * t, y * t);
        }
        constexpr Vector<T, 2> operator/(T t) const noexcept
        {
            return Vector<T, 2>(x / t, y / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector<T, 2> &v) noexcept 
        {
            return os << v.x << ' ' << v.y;
        }
        friend std::istream &operator>>(std::istream &is, Vector<T, 2>& v) noexcept 
        {
            return is >> v.x >> v.y;
        }
    };


    template<typename T>
    struct Vector<T, 3>
    {
        union
        {
            __extension__ struct {T x, y, z;};
            __extension__ struct {T r, g, b;};
        };
        constexpr Vector() noexcept : x(), y(), z() {}
        constexpr Vector(T k) noexcept : x(k), y(k), z(k) {};
        constexpr Vector(T x, T y, T z) noexcept : x(x), y(y), z(z){};
        constexpr Vector(const Vector<T, 3>& v) noexcept : x(v.x), y(v.y), z(v.z) {};

        template<typename U>
        constexpr explicit Vector(const Point<U, 3>& p)noexcept;
        template<typename U>
        constexpr explicit Vector(const Vector<U, 3>& v)noexcept;
        template<typename U>
        constexpr explicit Vector(const Normal<U, 3>& n)noexcept;

        constexpr explicit Vector(const Point<T, 3> &) noexcept;
        constexpr explicit Vector(const Normal<T, 3>&) noexcept;
        constexpr explicit Vector(const Vector<T, 4>&) noexcept;
        constexpr explicit Vector(const Vector<T, 2>&) noexcept;

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
    
        constexpr Vector<T, 3>& operator=(const Vector<T, 3>& v) noexcept
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;

            return *this;
        }

        constexpr bool operator==(const Vector<T, 3>& v) const noexcept
        {
            return this->x == v.x && this->y == v.y && this->z == v.z;
        }

        constexpr bool operator!=(const Vector<T, 3>& v) const noexcept
        {
            return this->x != v.x || this->y != v.y || this->z != v.z;
        }

        constexpr Vector<T, 3> operator+() const noexcept 
        {
            return Vector<T, 3>(x, y, z);
        }
        constexpr Vector<T, 3> operator-() const noexcept 
        {
            return Vector<T, 3>(-x, -y, -z);
        }


        constexpr Vector<T, 3> operator+(const Vector<T, 3>& v) const noexcept
        {
            return Vector<T, 3>(x + v.x, y + v.y, z + v.z);
        }

        constexpr Vector<T, 3>& operator+=(const Vector<T, 3>& v) noexcept
        {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            
            return *this;
        }

        constexpr Vector<T, 3> operator-(const Vector<T, 3>& v) const noexcept
        {
            return Vector<T, 3>(x - v.x, y - v.y, z - v.z);
        }

        constexpr Vector<T, 3>& operator-=(const Vector<T, 3>& v) noexcept
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;

            return *this;
        }


        constexpr Vector<T, 3>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;

            return *this;
        }
        constexpr Vector<T, 3>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;

            return *this;
        }
        
        constexpr Vector<T, 3> operator*(T t) const noexcept
        {
            return Vector<T, 3>(x * t, y * t, z * t);
        }
        constexpr Vector<T, 3> operator/(T t) const noexcept
        {
            return Vector<T, 3>(x / t, y / t, z / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector<T, 3> &v) noexcept 
        {
            return os << v.x << ' ' << v.y << ' ' << v.z;
        }
        friend std::istream &operator>>(std::istream &is, Vector<T, 3>& v) noexcept 
        {
            return is >> v.x >> v.y >> v.z;
        }
    };

    template<typename T>
    struct Vector<T, 4>
    {
        union 
        {
            __extension__ struct {T x, y, z, w;};
            __extension__ struct {T t, r, theta, phi;};
        };

        constexpr Vector() noexcept : x(), y(), z(), w() {}
        constexpr Vector(T k) noexcept : x(k), y(k), z(k), w(k) {};
        constexpr Vector(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w){};
        constexpr Vector(const Vector<T, 4>& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {};

        template<typename U>
        constexpr explicit Vector(const Vector<U, 4>&  p) noexcept;

        constexpr explicit Vector(const Point<T, 4> &) noexcept;
        constexpr explicit Vector(const Normal<T, 4>&) noexcept;
        constexpr explicit Vector(const Vector<T, 3>&) noexcept;

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
    
        constexpr Vector<T, 4>& operator=(const Vector<T, 4>& v) noexcept
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;

            return *this;
        }

        constexpr bool operator==(const Vector<T, 4>& v) const noexcept
        {
            return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
        }

        constexpr bool operator!=(const Vector<T, 4>& v) const noexcept
        {
            return this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w;
        }

        constexpr Vector<T, 4> operator+() const noexcept 
        {
            return Vector<T, 4>(x, y, z, w);
        }
        constexpr Vector<T, 4> operator-() const noexcept 
        {
            return Vector<T, 4>(-x, -y, -z, -w);
        }

        constexpr Vector<T, 4> operator+(const Vector<T, 4>& v) const noexcept
        {
            return Vector<T, 4>(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        constexpr Vector<T, 4>& operator+=(const Vector<T, 4>& v) noexcept
        {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            this->w += v.w;
            
            return *this;
        }

        constexpr Vector<T, 4> operator-(const Vector<T, 4>& v) const noexcept
        {
            return Vector<T, 4>(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        constexpr Vector<T, 4>& operator-=(const Vector<T, 4>& v) noexcept
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;

            return *this;
        }


        constexpr Vector<T, 4>& operator*=(T t) noexcept
        {
            this->x *= t;
            this->y *= t;
            this->z *= t;
            this->w *= t;

            return *this;
        }
        constexpr Vector<T, 4>& operator/=(T t) noexcept
        {
            x /= t;
            y /= t;
            z /= t;
            w /= t;

            return *this;
        }
        
        constexpr Vector<T, 4> operator*(T t) const noexcept
        {
            return Vector<T, 4>(x * t, y * t, z * t, w * t);
        }
        constexpr Vector<T, 4> operator/(T t) const noexcept
        {
            return Vector<T, 4>(x / t, y / t, z / t, w / t);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector<T, 4> &v) noexcept 
        {
            return os << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
        }
        friend std::istream &operator>>(std::istream &is, Vector<T, 4>& v) noexcept 
        {
            return is >> v.x >> v.y >> v.z >> v.w;
        }
        


    };

    


} // namespace ssmath
#endif //< SSMATH_VECTOR_HPP