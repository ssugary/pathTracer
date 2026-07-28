#include "Quaternion.hpp"

namespace Geo 
{
    Quaternion::Quaternion(Vec3 v, float w) noexcept : v(v), w(w) {};
    Quaternion::Quaternion(const Transform& t) noexcept : v(0,0,0), w(1) 
    {
        const Mat4& m = t.getTMat(); 

        float trace = m[0][0] + m[1][1] + m[2][2];

        if (trace > 0.f) 
        {
            float s = std::sqrt(trace + 1.f) * 2.f;                           // s = 4 * w
            w    = 0.25f * s;
            v[0] = (m[2][1] - m[1][2]) / s;
            v[1] = (m[0][2] - m[2][0]) / s;
            v[2] = (m[1][0] - m[0][1]) / s;
        } 
        else 
        {
            if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) 
            {
                float s = std::sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f; // s = 4 * x
                w    = (m[2][1] - m[1][2]) / s;
                v[0] = 0.25f * s;
                v[1] = (m[0][1] + m[1][0]) / s;
                v[2] = (m[0][2] + m[2][0]) / s; 
            } 

            else if (m[1][1] > m[2][2]) 
            {
                float s = std::sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f; // s = 4 * y
                w    = (m[0][2] - m[2][0]) / s;
                v[0] = (m[0][1] + m[1][0]) / s;
                v[1] = 0.25f * s;
                v[2] = (m[1][2] + m[2][1]) / s;
            } 

            else 
            {
                float s = std::sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f; // s = 4 * z
                w    = (m[1][0] - m[0][1]) / s;
                v[0] = (m[0][2] + m[2][0]) / s;
                v[1] = (m[1][2] + m[2][1]) / s;
                v[2] = 0.25f * s;
            }
        }
    }
    Quaternion Quaternion::operator-() const noexcept
    {
        return Quaternion(-v, -w);
    }
    Quaternion Quaternion::operator+() const noexcept
    {
        return Quaternion(v, w);
    }
    Quaternion Quaternion::operator+(const Quaternion& q) const noexcept
    {
        return Quaternion(v + q.v, w + q.w);
    }

    Quaternion& Quaternion::operator+=(const Quaternion& q) noexcept
    {
        this->v += q.v;
        this->w += q.w;

        return *this;
    }
    Quaternion Quaternion::operator-(const Quaternion& q) const noexcept
    {
        return Quaternion(v - q.v, w - q.w);
    }

    Quaternion& Quaternion::operator-=(const Quaternion& q) noexcept
    {
        this->v -= q.v;
        this->w -= q.w;

        return *this;
    }
    Quaternion Quaternion::operator*(const float t) const noexcept
    {
        return Quaternion(v * t, w * t);
    }

    Quaternion& Quaternion::operator*=(const float t) noexcept
    {
        this->v *= t;
        this->w *= t;

        return *this;
    }
    Quaternion Quaternion::operator/(const float t) const noexcept
    {
        return Quaternion(v / t, w / t);
    }

    Quaternion& Quaternion::operator/=(const float t) noexcept
    {
        this->v /= t;
        this->w /= t;

        return *this;
    }





    
};
