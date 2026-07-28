#ifndef COLOR_HPP
#define COLOR_HPP

#include <cassert>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <istream>
#include "foward.hpp"

namespace ssmath3 
{
    struct Color 
    {
        float r;
        float g;
        float b;

        constexpr Color() noexcept: r(0.f), g(0.f), b(0.f) {}
        constexpr Color(float k) noexcept : r(k), g(k), b(k) {} 
        constexpr Color(float r, float g, float b) noexcept : r(r), g(g), b(b) {}
        constexpr Color(const Color&) noexcept = default;
        constexpr explicit Color(const Vector<float, 3>&) noexcept;
        constexpr explicit Color(const Normal<float, 3>&) noexcept;
        constexpr explicit Color(const Point<float, 3> &) noexcept;

        constexpr bool operator==(const Color& c) const noexcept
        {
            return r == c.r && g == c.g && b == c.b;
        }
        constexpr bool operator!=(const Color& c) const noexcept
        {
            return r != c.r || g != c.g || b != c.b;
        }

        constexpr float& operator[](const std::size_t i) noexcept
        {
            assert(i < 3);

            if(i == 0) return r;
            if(i == 1) return g;
            return b;
        }
        constexpr const float& operator[](const std::size_t i) const noexcept
        {
            assert(i < 3);

            if(i == 0) return r;
            if(i == 1) return g;
            return b;
        }

        constexpr Color& operator=(const Color& c) noexcept
        {
            r = c.r;
            g = c.g;
            b = c.b;
            return *this;
        }

        constexpr Color operator+(const Color &c) const noexcept
        {
            return Color(r + c.r, g + c.g, b + c.b);
        }

        constexpr Color& operator+=(const Color &c) noexcept
        {
            r += c.r; 
            g += c.g; 
            b += c.b;
            return *this;
        }

        constexpr Color operator*(const Color &c) const noexcept
        {
            return Color(r * c.r, g * c.g, b * c.b);
        }

        constexpr Color& operator*=(const Color &c) noexcept
        {
            r *= c.r; 
            g *= c.g; 
            b *= c.b;
            return *this;
        }

        constexpr Color operator*(float s) const 
        {
            return Color(r * s, g * s, b * s);
        }

        constexpr Color& operator*=(float s) noexcept
        {
            r *= s; 
            g *= s; 
            b *= s;
            return *this;
        }

        constexpr Color operator/(float s) const noexcept
        {
            return Color(r / s, g / s, b / s);
        }
        constexpr Color& operator/=(float s) noexcept
        {
            r /= s;
            g /= s; 
            b /= s;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Color &c) noexcept 
        {
            return os << c.r << ' ' << c.g << ' ' << c.b;
        }
        friend std::istream &operator>>(std::istream &is, Color& c) noexcept 
        {
            return is >> c.r >> c.g >> c.b;
        }
        
    };

    inline Color operator*(float s, const Color &c) noexcept
    {
        return c * s;
    }
}

#endif // COLOR_HPP