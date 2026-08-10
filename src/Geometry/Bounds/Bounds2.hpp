#pragma once

#ifndef BOUNDS_2_HPP
#define BOUNDS_2_HPP

#include "ssmath3/ssmath3.hpp"

using namespace ssmath3;

namespace Geo 
{
    template<typename T>
    class Bounds2
    {
        public:

            Point<T, 2> pMin;
            Point<T, 2> pMax;

            Bounds2() 
            {
                float minNum = std::numeric_limits<float>::lowest();
                float maxNum = std::numeric_limits<float>::max();
                pMin = Point<T, 2>(maxNum, maxNum);
                pMax = Point<T, 2>(minNum, minNum);
            }
            Bounds2(const Point<T, 2> &p) : pMin(p), pMax(p) {}
            Bounds2(const Point<T, 2> &p1, const Point<T, 2> &p2) 
            {
                pMin = Point<T, 2>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
                pMax = Point<T, 2>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
            }
            
            Vector<T, 2> diagonal() const 
            {
                return pMax - pMin;
            }
            float area() const 
            {
                Vector<T, 2> d = diagonal();
                return (d.x * d.y);
            }
            std::size_t maximumExtent() const 
            {
                Vector<T, 2> diag = diagonal();
                if (diag.x > diag.y)
                    return 0;
                else
                    return 1;
            }
            inline const Point<T, 2>& operator[](size_t i) const 
            {
                return (i == 0) ? pMin : pMax;
            }
            inline Point<T, 2>& operator[](size_t i) 
            {
                return (i == 0) ? pMin : pMax;
            }
            bool operator==(const Bounds2<T> &b) const 
            {
                return b.pMin == pMin && b.pMax == pMax;
            }
            bool operator!=(const Bounds2<T> &b) const 
            {
                return b.pMin != pMin || b.pMax != pMax;
            }
            Vector<T, 2> offset(const Point<T, 2> &p) const 
            {
                Vector<T, 2> o = p - pMin;
                if (pMax[0] > pMin[0]) 
                    o[0] /= pMax[0] - pMin[0];
                if (pMax[1] > pMin[1]) 
                    o[1] /= pMax[1] - pMin[1];
                return o;
            }
    };  
    template<typename T>
    inline bool overlaps(const Bounds2<T> &b1, const Bounds2<T> &b2) 
    {
        bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
        bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);

        return (x && y);
    }
    template<typename T>
    inline bool inside(const Point<T, 2> &p, const Bounds2<T> &b) 
    {
        return (p.x >= b.pMin.x && p.x <= b.pMax.x &&
                p.y >= b.pMin.y && p.y <= b.pMax.y);
    }
    template<typename T>
    inline bool insideExclusive(const Point<T, 2> &p, const Bounds2<T> &b) 
    {
        return (p.x >= b.pMin.x && p.x < b.pMax.x &&
                p.y >= b.pMin.y && p.y < b.pMax.y);
    }
    template<typename T>
    inline Bounds2<T> expand(const Bounds2<T> &b, float delta)
    {
        return Bounds2<T>(b.pMin - Vector<T, 2>(delta, delta),
                        b.pMax + Vector<T, 2>(delta, delta));
    }
}

#endif