#pragma once

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Utils/common.hpp"
#include "Geometry/Rays/Ray.hpp"

namespace Geo 
{
    class SurfaceInteraction;
    
    class Transform 
    {
        
        private:
            Mat4 transMat;
            Mat4 invTransMat;
            friend class AnimatedTransform;
            friend struct Quaternion;
        public:
            Transform();
            Transform(const Mat4&);
            Transform(const Mat4&, const Mat4&);
            Transform(const Transform&) = default;
            bool swapHandedness() const; 
            
            void operator=(const Transform& t) 
            {
                transMat = t.getTMat();
                invTransMat = t.getTMatInv();
            };
            
            Point4                operator()(const Point4& p)                     const;
            Point3                operator()(const Point3& p, Vec3* =nullptr)     const;
            Point3                operator()(const Point3& p, const Vec3&, Vec3*) const;
            Vec3                  operator()(const Vec3& p)                       const;
            Vec4                  operator()(const Vec4& p)                       const;
            Normal3               operator()(const Normal3& p)                    const;
            Ray                   operator()(const Ray& r)                        const;
            Bounds3f              operator()(const Bounds3f& b)                   const;
            SurfaceInteraction    operator()(const SurfaceInteraction& s)         const;
            Transform             operator()(const Transform& t)                  const;

            bool operator==(const Transform& t)     const;
            bool operator!=(const Transform& t)     const;
            Mat4 getTMat()    const {return transMat;};
            Mat3 getTMat3()   const {return Mat3
                                        {transMat[0][0], transMat[0][1], transMat[0][2],
                                         transMat[1][0], transMat[1][1], transMat[1][2],
                                         transMat[2][0], transMat[2][1], transMat[2][2]};
                                    }
            Mat4 getTMatInv() const {return this->invTransMat;};
            std::string toString() const;
            static Transform inverse(const Transform& t);
            static Transform transpose(const Transform& t);
            static Transform translate(const Point3& delta);
            static Transform translate(const Vec3& delta);
            static Transform rotate(const float& angle, const Point3& delta);
            static Transform rotateX(const float& angle);
            static Transform rotateY(const float& angle);
            static Transform rotateZ(const float& angle);
            static Transform scale(const Point3& scales);
            static Transform scale(const float x, const float y, const float z);

            static Transform orthographic(float znear, float zfar);
            static Transform perspective(float fov, float n, float f);
            static Transform lookAt(const Point3& look_from, const Point3& look_at, const Vec3& vup);

    };

    struct TransformHash 
    {
        std::size_t operator()(const Transform& t) const noexcept
        {
            Mat4 m = t.getTMat();
            std::size_t h = 0;

            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    std::size_t hi = std::hash<float>{}(m[i][j]);
                    h ^= hi + 0x9e3779b9 + (h << 6) + (h >> 2);
                }
            }

            return h;
        }
    };
}
#endif