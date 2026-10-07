#pragma once 

#ifndef TEXTURE_MAPPING_HPP
#define TEXTURE_MAPPING_HPP

#include "Geometry/Transformation/Transform.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"

namespace Geo 
{
    class TextureMapping2D
    {   
        public:

            virtual ~TextureMapping2D() = default;
            virtual Point2 map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const = 0;
    };

    class UVMapping2D : public TextureMapping2D 
    {
        private:
            const float su;
            const float sv;
            const float du;
            const float dv;
        public:
            UVMapping2D(float su, float sv, float du, float dv);
            Point2 map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const override;
    };

    class SphericalMapping2D : public TextureMapping2D
    {
        private:
            const Transform W2T;
            Point2 sphere(const Point3& p) const;
        public:
            SphericalMapping2D(const Transform& W2T);
            Point2 map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const override;
    }; 

    class CylindricalMapping2D : public TextureMapping2D
    {
        private:
            const Transform W2T;
            Point2 cylinder(const Point3& p) const;
        public:
            CylindricalMapping2D(const Transform& W2T);
            Point2 map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const override;
    };

    class PlanarMapping2D : public TextureMapping2D
    {
        private:
            Vec3 vs;
            Vec3 vt;
            float ds;
            float dt;
        public:
            PlanarMapping2D(const Vec3& vs, const Vec3& vt, float ds, float dt);
            Point2 map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const override;
    };

    class TextureMapping3D 
    {
        public:
            virtual ~TextureMapping3D() = default;
            virtual Point3 map(const SurfaceInteraction& sf, Vec3* dpdx, Vec3* dpdy) const = 0;
    };

    class TransformMapping3D : public TextureMapping3D
    {
        private:
            const Transform W2T;
        public:
            TransformMapping3D(const Transform& W2T);
            Point3 map(const SurfaceInteraction& sf, Vec3* dpdx, Vec3* dpdy) const override;

    };
}


#endif //< TEXTURE_MAPPING_HPP