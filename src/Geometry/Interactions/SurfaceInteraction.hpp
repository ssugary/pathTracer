#ifndef SURFACE_INTERACTION_HPP
#define SURFACE_INTERACTION_HPP

#include "BSDF.hpp"
#include "Interaction.hpp"
#include "Geometry/Primitives/Primitive.hpp"


using namespace Prim;

namespace Geo 
{
    class Shape;

    class SurfaceInteraction : public Interaction
    {
        public:
            Point2 uv;
            Vec3 dpdu;
            Vec3 dpdv;
            Normal3 dndu;
            Normal3 dndv;
            const Shape *shape = nullptr;
            const Primitive *primitive = nullptr;
                
            struct 
            {
                Normal3 n;
                Vec3 dpdu;
                Vec3 dpdv;
                Normal3 dndu;
                Normal3 dndv;
            } shading;

            mutable Vec3 dpdx; 
            mutable Vec3 dpdy;
            mutable float dudx = 0;
            mutable float dvdx = 0;
            mutable float dudy = 0;
            mutable float dvdy = 0;

            BSDF *bsdf = nullptr;
            BSSRDF *bssrdf = nullptr;

            SurfaceInteraction() = default;
            SurfaceInteraction(const SurfaceInteraction&) = default;
            SurfaceInteraction(const Point3 &p,
                               const Vec3 &pError, const Point2 &uv, const Vec3 &wo,
                               const Vec3 &dpdu, const Vec3 &dpdv,
                               const Normal3 &dndu, const Normal3 &dndv,
                               float time, const Shape *shape);
            void setShadingGeometry(const Vec3 &dpdus,
                                    const Vec3 &dpdvs, const Normal3 &dndus,
                                    const Normal3 &dndvs, bool orientationIsAuthoritative);
    };
};

#endif