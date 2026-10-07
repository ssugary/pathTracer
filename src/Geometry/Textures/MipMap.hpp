#pragma once 

#ifndef MIPMAP_HPP
#define MIPMAP_HPP

#include "Utils/common.hpp"
#include <memory>
#include <vector>

namespace Geo 
{   
    enum class WrapMode 
    {
        REPEAT=0,
        BLACK,
        CLAMP,
    };

    template<typename T>
    class MIPMap 
    {
        private:

            Point2i res;
            WrapMode wrapMode;
            std::vector<std::unique_ptr<Array2D<T>>> pyramid;
            bool doTrilinear;
            float maxAnisotropy;

            T texel(int level, int x, int y) const;
            T triangle(int level, const Point2 &st) const;
            static float weightLut[128];
            
        public:

            MIPMap(const Point2i& res, T* data, WrapMode mode, bool doTrilinear, float maxAnisotropy);
            T lookup(const Point2& st, const Vec2& dstdx, const Vec2& dstdy) const;
            T lookup(const Point2& st, float width = 0.f) const;
            T EWA(int level, Point2 st, Vec2 dstdx, Vec2 dstdy) const;
            T bilerp(int level, const Point2& st) const;
    };
};


#endif //< MIPMAP_HPP