#pragma once

#ifndef COMMON_HPP
#define COMMON_HPP


#include "ssmath3/ssmath3.hpp"
#include "OctaedralVector.hpp"
#include "Geometry/Bounds/Bounds2.hpp"
#include "Geometry/Bounds/Bounds3.hpp"
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <bit>
#include <cstring>
// #include "RNG.hpp"

using namespace ssmath3;
using namespace Geo;

using ssmath::Array2D;

typedef Matrix<float, 4> Mat4;
typedef Matrix<float, 3> Mat3;

typedef Point<float, 2> Point2;
typedef Point<float, 3> Point3;
typedef Point<float, 4> Point4;
typedef Point<int, 2> Point2i;
typedef Point<int, 3> Point3i;
typedef Point<int, 4> Point4i;

typedef Vector<float, 2> Vec2;
typedef Vector<float, 3> Vec3;
typedef Vector<float, 4> Vec4;
typedef Vector<int, 2> Vec2i;
typedef Vector<int, 3> Vec3i;
typedef Vector<int, 4> Vec4i;

typedef Normal<float, 3> Normal3;
typedef Normal<float, 4> Normal4;

typedef Bounds2<float> Bounds2f;
typedef Bounds2<int>   Bounds2i;
typedef Bounds3<float> Bounds3f;
typedef Bounds3<int>   Bounds3i;


static constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
static constexpr float INF = std::numeric_limits<float>::infinity();
static constexpr float EPSILON = std::numeric_limits<float>::epsilon();
static constexpr float ONE_MINUS_EPSILON = 1 - EPSILON;
static constexpr float EPSILON_9 = 1e-9f;
static constexpr float EPSILON_8 = 1e-8f;
static constexpr float EPSILON_6 = 1e-6f;
static constexpr float EPSILON_3 = 1e-3f;
static constexpr float SHADOW_EPSILON = 1e-4f;
static constexpr float ONE_MINUS_SHADOW_EPSILON = 1 - SHADOW_EPSILON;
static constexpr float PI =          3.14159265358979323846;
static constexpr float PI_OVER_TWO = 1.57079632679489661923;

   struct CameraSample
   {
       Point2 pFilm;
       Point2 pLens;
       float time;
   };

   inline float radToDeg(float r)
   {
        return r * (180.f / PI); 
   }
   inline float degToRad(float d)
   {
        return d * (PI / 180.f); 
   }

   inline float ffmin(const float &a, const float &b) { return a < b ? a : b; }
   inline float ffmax(const float &a, const float &b) { return a > b ? a : b; }

   inline Vec3 avoidZeroDirection(Vec3 d)
    {
        constexpr float eps = 1e-8f;
        if (std::abs(d.x) < eps) d.x = (d.x < 0) ? -eps : eps;
        if (std::abs(d.y) < eps) d.y = (d.y < 0) ? -eps : eps;
        if (std::abs(d.z) < eps) d.z = (d.z < 0) ? -eps : eps;
        return d;
    }
   
  inline void XYZToRGB(const float xyz[3], float rgb[3]) 
  {
    rgb[0] =  3.240479f*xyz[0] - 1.537150f*xyz[1] - 0.498535f*xyz[2];
    rgb[1] = -0.969256f*xyz[0] + 1.875991f*xyz[1] + 0.041556f*xyz[2];
    rgb[2] =  0.055648f*xyz[0] - 0.204043f*xyz[1] + 1.057311f*xyz[2];
  }

  inline uint64_t multiplicativeInverse(int64_t a, int64_t n) 
  {
    int64_t t = 0, newt = 1;
    int64_t r = n, newr = a;

    while (newr != 0) 
    {
        int64_t quotient = r / newr;
        
        int64_t tmp_t = t;
        t = newt;
        newt = tmp_t - quotient * newt;
        
        int64_t tmp_r = r;
        r = newr;
        newr = tmp_r - quotient * newr;
    }

    if (r > 1) 
        return 0; 
    

    if (t < 0) 
        t += n;
    

    return (uint64_t)t;
  }

  inline float nextFloatUp(float v) 
    {
        if (std::isinf(v) && v > 0.f) 
            return v;
        if (v == -0.0f) 
            v = 0.0f; 

        uint32_t ui;
        std::memcpy(&ui, &v, sizeof(float));
        
        if (v >= 0.0f) 
            ++ui;
        else           
            --ui;
        
        std::memcpy(&v, &ui, sizeof(float));
        return v;
    }

    inline float nextFloatDown(float v) 
    {
        if (std::isinf(v) && v < 0.f) 
            return v;
        if (v == 0.0f) 
            v = -0.0f;

        uint32_t ui;
        std::memcpy(&ui, &v, sizeof(float));
        
        if (v >= 0.0f) 
            --ui;
        else           
            ++ui;
        
        std::memcpy(&v, &ui, sizeof(float));
        return v;
    }

    

    inline bool refract(const Vec3& wi, const Normal3& n, float eta, Vec3* wt)
    {
        float cosThetaI = dot(n, wi);
        float sin2ThetaI = std::max(0.f, 1.f - cosThetaI * cosThetaI);
        float sin2ThetaT = eta * eta * sin2ThetaI;

        if (sin2ThetaT >= 1.f) 
            return false; 

        float cosThetaT = std::sqrt(1.f - sin2ThetaT);

        *wt = -eta * wi + (eta * cosThetaI - cosThetaT) * static_cast<Vec3>(n);

        return true;
    }

    inline Point3 offsetRayOrigin(const Point3 &p, const Vec3 &pError,
                                  const Normal3 &n, const Vec3 &w) 
    {
        float d = dot(abs(n), pError);

        if (d == 0.0f) 
            d = SHADOW_EPSILON; 

        Vec3 offset = d * static_cast<Vec3>(n);
        
        if (dot(w, n) < 0)
            offset = -offset;
        
        Point3 po = p + offset;

        for (int i = 0; i < 3; ++i) 
        {
            if (offset[i] > 0)      
                po[i] = nextFloatUp(po[i]);
            else if (offset[i] < 0) 
                po[i] = nextFloatDown(po[i]);
        }

        return po;
    }

    inline float computeUt(float r, float omega, float M, float a) 
    {
        float r2 = r * r;
        float a2 = a * a;

        float gtt  = -(1.f - (2.f * M) / r);
        float gtph = -(2.f * M * a) / r;
        float gphph = r2 + a2 + (2.f * M * a2) / r;

        float denom = -(gtt + 2.f * omega * gtph + omega * omega * gphph);

        if (denom <= 0.f) 
            return 1.f;

        return 1.f / std::sqrt(denom);
    }


    inline float sphericalTriangleArea(const Vec3& a, const Vec3& b, const Vec3& c) 
    {
        return std::abs(2.0f * std::atan2(dot(a, cross(b, c)), 1 + dot(a, b) + dot(a, c) + dot(b, c)));
    }
    inline float sphericalQuadArea(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d) 
    {
        Vec3 v = cross(b, c);
        float bc = dot(b, c);
        float ac = dot(a, c);
        float ab = dot(a, b);
        float dc = dot(d, c);
        float db = dot(d, b);

        float n1 = dot(a, v);
        float d1 = 1.0f + ab + bc + ac;
        float n2 = dot(d, v);
        float d2 = 1.0f + db + bc + dc;

        return std::abs(2.0f * std::atan2(n1 * d2 + n2 * d1, d1 * d2 + n1 * n2));
    }

    inline Vec3 sphericalDirection(float sinT, float cosT, float phi)
    {
        return Vec3(clamp(sinT, -1, 1) * std::cos(phi),
                    clamp(sinT, -1, 1) * std::sin(phi),
                    clamp(cosT, -1, 1));
    }

    inline float sphericalPhi(const Vec3& v)
    {
        float p = std::atan2(v.y, v.x);
        return p < 0 ? p + 2 * PI : p;
    }
    inline float sphericalTheta(const Vec3& v)
    {
        return std::acos(std::clamp(v.z, -1.f, 1.f));
    }
    inline float cosTheta(const Vec3 w)
    {
        return w.z;
    }
    inline float cos2Theta(const Vec3& w)
    {
        return w.z * w.z;
    }
    inline float absCosTheta(const Vec3& w)
    {
        return std::abs(w.z);
    }
    inline float sin2Theta(const Vec3& w)
    {
        return std::max(0.f, 1.f - w.z * w.z);
    }
    inline float sinTheta(const Vec3& w)
    {
        return std::sqrt(sin2Theta(w));
    }
    inline float tanTheta(const Vec3& w)
    {
        return sinTheta(w) / cosTheta(w);
    }
    inline float tan2Theta(const Vec3& w)
    {
        return sin2Theta(w) / cos2Theta(w);
    }
    inline float sinPhi(const Vec3& w)
    {
        float sin = sinTheta(w);
        return sin == 0.f ? 0 : clamp(w.y / sin, -1, 1);
    }
    inline float cosPhi(const Vec3& w)
    {
        float sin = sinTheta(w);
        return sin == 0 ? 1 : clamp(w.x / sin, -1, 1);
    }
    inline float cos2Phi(const Vec3& w)
    {
        return cosPhi(w) * cosPhi(w);
    }
    inline float sin2Phi(const Vec3& w)
    {
        return sinPhi(w) * sinPhi(w);
    }
    inline float cosDeltaPhi(const Vec3& wa, const Vec3& wb)
    {
        float dxya = wa.x * wa.x + wa.y * wa.y;
        float dxyb = wb.x * wb.x + wb.y * wb.y;
        
        if(dxya == 0 || dxyb == 0)
            return 1.f;

        return clamp((wa.x * wb.x + wa.y * wb.y)/std::sqrt(dxya * dxyb), -1, 1);
    }

    inline float angle(const Vec3& v1, const Vec3& v2)
    {
        if(dot(v1, v2) < 0)
            return PI - 2 * std::asin(std::max(0.f, length(v1 + v2)/2));
        return 2 * std::asin(std::max(0.f, length(v2 - v1)/2));
    }

    inline float gamma(int n) 
    {
        float epsilon = EPSILON * 0.5;

        return (n * epsilon) / (1.0 - n * epsilon);
    }  

    inline Vec3 squareToSphere(const Point2& p)
    {
        float u = 2 * p.x - 1;
        float v = 2 * p.y - 1;
        float up = std::abs(u);
        float vp = std::abs(v);

        float dist = 1 - (up + vp);
        float d = std::abs(dist);
        float r = 1 - d;

        auto phi = (r == 0) ? 1 : ((vp - up) / r + 1) * (PI/4);

        auto z = std::copysign(1 - r * r, dist);

        float cosPhi = std::copysign(std::cos(phi), u);
        float sinPhi = std::copysign(std::sin(phi), v);

        return Vec3(r * cosPhi * std::sqrt(std::max(0.f, 2 - r * r)), 
                    r * sinPhi * std::sqrt(std::max(0.f, 2 - r * r)),
                      z);
    }
    inline Point2 wrapSquare(const Point2& uv) 
    {
        Point2 p = uv;
        if (p.x < 0.0f) 
        {
            p.x = -p.x;
            p.y = 1.0f - p.y;
        } 
        else if (p.x > 1.0f) 
        {
            p.x = 2.0f - p.x;
            p.y = 1.0f - p.y;
        }

        if (p.y < 0.0f) 
        {
            p.y = -p.y;
            p.x = 1.0f - p.x;
        } 
        else if (p.y > 1.0f) 
        {
            p.y = 2.0f - p.y;
            p.x = 1.0f - p.x;
        }

        return p;
    }

    inline Vec3 cosineSampleHemisphere(const Point2& u)
    {
        float r = std::sqrt(std::max(0.f, u.x));
        float theta = 2.f * PI * u.y;

        float x = r * std::cos(theta);
        float y = r * std::sin(theta);
        float z = std::sqrt(std::max(0.f, 1.f - u.x));

        return Vec3(x, y, z);
    }

    inline bool sameHemisphere(const Vec3& w1, const Vec3& w2)
    {
        return w1.z * w2.z > 0.f;
    }

    inline Vec3 alignToNormal(const Vec3& local, const Normal3& n)
    {
        Vec3 w = normalize(static_cast<Vec3>(n));

        Vec3 vup = std::abs(w.z) < ONE_MINUS_SHADOW_EPSILON ? Vec3(0.f, 0.f, 1.f) : Vec3(1.f, 0.f, 0.f);

        Vec3 u = normalize(cross(vup, w));
        Vec3 v = cross(w, u);

        return local.x * u + local.y * v + local.z * w;
    }

    inline Point2 concentricSampleDisk(const Point2 &u) 
    {
        Point2 uOffset = 2.f * u - Vec2(1, 1);

        if (uOffset.x == 0.f && uOffset.y == 0) 
            return Point2(0, 0);

        float theta, r;
        if (std::abs(uOffset.x) > std::abs(uOffset.y)) 
        {
            r = uOffset.x;
            theta = (PI/4) * (uOffset.y / uOffset.x);
        } 
        else
        {
            r = uOffset.y;
            theta = (PI/2) - (PI/4) * (uOffset.x / uOffset.y);
        }

        return r * Point2(std::cos(theta), std::sin(theta));
    }

    inline float planck(float lambda, float t)
    {   
        if(t <= 0)
            return 0.f;
        const float c = 299792458.f;
        const float h = 6.62606957e-34f;
        const float kb = 1.3806488e-23f;

        float l = lambda * EPSILON_9;
        float Le = (2.f * h * c * c) / (std::pow(l, 5) * (std::exp((h * c) / (l * kb * t)) - 1));
        return Le;
    }

#endif
