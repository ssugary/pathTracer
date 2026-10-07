#pragma once
#ifndef GEODESIC_RAY_HPP
#define GEODESIC_RAY_HPP

#include <Utils/common.hpp>

namespace Geo 
{

    struct GeodesicRay 
    {
      Point4 x;
      Vec4   p;

      float lambda;
      float E;
      float L;
      float Q;

      GeodesicRay(const Point4& x, const Vec4& p, float lambda=0.f, float E=0.f, float L=0.f, float Q=0.f)
      : x(x), p(p), lambda(lambda), E(E), L(L), Q(Q) {};

      
    };

} // namespace Geo


#endif // GEODESIC_RAY_HPP
