#pragma once

#ifndef QUATErNION_HPP
#define QUATErNION_HPP

#include "Utils/common.hpp"
#include "Transform.hpp"

namespace Geo 
{

    struct Quaternion 
    {   
        public:
        
            Vec3 v; //< xyz components
            float w; //< real component

            Quaternion(Vec3 v={0,0,0}, float w=1) noexcept;
            Quaternion(const Transform& t) noexcept;
            Quaternion operator-() const noexcept;
            Quaternion operator+() const noexcept;
            Quaternion operator+(const Quaternion& q) const noexcept;
            Quaternion& operator+=(const Quaternion& q) noexcept;
            Quaternion operator-(const Quaternion& q) const noexcept;
            Quaternion& operator-=(const Quaternion& q) noexcept;
            Quaternion operator*(const float t) const noexcept;
            Quaternion& operator*=(const float t) noexcept;
            Quaternion operator/(const float t) const noexcept;
            Quaternion& operator/=(const float t) noexcept;

        };
        
    inline Transform toTransform(const Quaternion& q) noexcept
    {
        float xx = q.v[0] * q.v[0], yy = q.v[1] * q.v[1], zz = q.v[2] * q.v[2];
        float xy = q.v[0] * q.v[1], xz = q.v[0] * q.v[2], yz = q.v[1] * q.v[2];
        float wx = q.w    * q.v[0], wy = q.w    * q.v[1], wz = q.w    * q.v[2];

        Mat4 m{
            1.f - 2.f * (yy + zz), 2.f * (xy - wz),       2.f * (xz + wy),       0.f,
            2.f * (xy + wz),       1.f - 2.f * (xx + zz), 2.f * (yz - wx),       0.f,
            2.f * (xz - wy),       2.f * (yz + wx),       1.f - 2.f * (xx + yy), 0.f,
            0.f,                   0.f,                   0.f,                   1.f
        };

        return Transform(m, ::transpose(m));
    }

    inline Quaternion operator*(const float t, const Quaternion& q) noexcept
    {
        return q * t;
    }
    inline Quaternion& operator*=(const float t,  Quaternion& q) noexcept
    {
        return q *= t;
    }
    inline float dot(const Quaternion& q1, const Quaternion& q2)
    {
    return dot(q1.v, q2.v) + (q1.w * q2.w);
    }

    inline Quaternion normalize(const Quaternion& q)
    {
        return q / std::sqrt(dot(q, q));
    }
    inline Quaternion slerp(const float t, const Quaternion& q1, const Quaternion& q2)
    {
        float cosTheta = dot(q1, q2);
        if (cosTheta > .9995f)
            return normalize((1 - t) * q1 + t * q2);
        else 
        {
            float theta = std::acos(::clamp(cosTheta, -1.f, 1.f));
            float thetap = theta * t;
            Quaternion qperp = normalize(q2 - q1 * cosTheta);
            return q1 * std::cos(thetap) + qperp * std::sin(thetap);
        }
    }
};

#endif