#pragma once

#ifndef INTERVALS_HPP
#define INTERVALS_HPP

#include <algorithm>
#include <cmath>

namespace Geo 
{
    class Interval 
    {
        public:
            float low;
            float high;

            Interval(float v) : low(v), high(v) {};
            Interval(float a, float b) : low(std::min(a, b)), high(std::max(a, b)) {};
    
            Interval operator+(const Interval &i) const 
            {
                return Interval(low + i.low, high + i.high);
            }
            Interval operator-(const Interval &i) const 
            {
                return Interval(low - i.high, high - i.low);
            }
    
            Interval operator*(const Interval &i) const 
            {
                return Interval(std::min( std::min(low * i.low,  high * i.low),
                                            std::min(low * i.high, high * i.high)),
                                std::max( std::max(low * i.low,  high * i.low),
                                            std::max(low * i.high, high * i.high)));
            }
    };

    inline Interval sin(const Interval &i) 
    {
        float sinLow = std::sin(i.low), sinHigh = std::sin(i.high);

        if (sinLow > sinHigh)
            std::swap(sinLow, sinHigh);

        if (i.low < M_PI / 2 && i.high > M_PI / 2)
            sinHigh = 1.;

        if (i.low < (3.f / 2.f) * M_PI && i.high > (3.f / 2.f) * M_PI)
            sinLow = -1.;

        return Interval(sinLow, sinHigh);
    }
    inline Interval cos(const Interval &i) 
    {
        float cosLow = std::cos(i.low), cosHigh = std::cos(i.high);

        if (cosLow > cosHigh)
            std::swap(cosLow, cosHigh);

        if (i.low < M_PI / 2 && i.high > M_PI / 2)
            cosHigh = 1.;

        if (i.low < (3.f / 2.f) * M_PI && i.high > (3.f / 2.f) * M_PI)
            cosLow = -1.;

        return Interval(cosLow, cosHigh);
    }

    inline void intervalFindZeros(float c1, float c2, float c3, float c4,
                           float c5, float theta, Interval tInterval, float *zeros,
                           std::size_t *zeroCount, std::size_t depth = 8) 
    {
        Interval range = Interval(c1) +
            (Interval(c2) + Interval(c3) * tInterval) * cos(Interval(2 * theta) * tInterval) +
            (Interval(c4) + Interval(c5) * tInterval) * sin(Interval(2 * theta) * tInterval);
        if (range.low > 0. || range.high < 0. || range.low == range.high)
            return;

        if(depth > 0)
        {
            float mid = (tInterval.low + tInterval.high) * 0.5f;
            intervalFindZeros(c1, c2, c3, c4, c5, theta,Interval(tInterval.low, mid), zeros, zeroCount, depth - 1);
            intervalFindZeros(c1, c2, c3, c4, c5, theta,Interval(mid, tInterval.high), zeros, zeroCount, depth - 1);
        }
        else 
        {
            float tNewton = (tInterval.low + tInterval.high) * 0.5f;
            for (std::size_t i = 0; i < 4; ++i) {
                float fNewton = c1 +
                    (c2 + c3 * tNewton) * std::cos(2.f * theta * tNewton) +
                    (c4 + c5 * tNewton) * std::sin(2.f * theta * tNewton);
                float fPrimeNewton =
                    (c3 + 2 * (c4 + c5 * tNewton) * theta) *
                        std::cos(2.f * tNewton * theta) +
                    (c5 - 2 * (c2 + c3 * tNewton) * theta) *
                        std::sin(2.f * tNewton * theta);
                if (fNewton == 0 || fPrimeNewton == 0)
                    break;
                tNewton = tNewton - fNewton / fPrimeNewton;
            }
            zeros[*zeroCount] = tNewton;
            (*zeroCount)++;
        
        }
    }

}; //< namespace Geo

#endif //< INTERVALS_HPP