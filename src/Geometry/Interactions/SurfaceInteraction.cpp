#include "SurfaceInteraction.hpp"
#include "Geometry/Shapes/Shape.hpp"
#include "Light/AreaLight.hpp"

namespace Geo 
{
    SurfaceInteraction::SurfaceInteraction(
                               const Point3 &p, const Vec3 &pError, const Point2 &uv, const Vec3 &wo,
                               const Vec3 &dpdu, const Vec3 &dpdv, const Normal3 &dndu, const Normal3 &dndv,
                               float time, const Shape *shape)
            : Interaction(p, static_cast<Normal3>(normalize(cross(dpdu, dpdv))), pError, wo,
                          time, nullptr),
                uv(uv), dpdu(dpdu), dpdv(dpdv), dndu(dndu), dndv(dndv), shape(shape) 
                {
                    shading.n    = n;
                    shading.dpdu = dpdu;
                    shading.dpdv = dpdv;
                    shading.dndu = dndu;
                    shading.dndv = dndv;

                    if (shape && (shape->reverseOrientation ^ shape->tSwapHandedness)) 
                    {
                        n *= -1;
                        shading.n *= -1;
                    }
                }

    void SurfaceInteraction::setShadingGeometry(const Vec3 &dpdus, const Vec3 &dpdvs, 
                                                const Normal3 &dndus, const Normal3 &dndvs,
                                                bool orientationIsAuthoritative)                                        
        {
            shading.n = ::normalize(static_cast<Normal3>(cross(dpdus, dpdvs)));
            if (shape && (shape->reverseOrientation ^ shape->tSwapHandedness))
                shading.n = -shading.n;
            if (orientationIsAuthoritative)
                n = ::faceFoward(n, shading.n);
            else
                shading.n = faceFoward(shading.n, n);

            shading.dpdu = dpdus;
            shading.dpdv = dpdvs;
            shading.dndu = dndus;
            shading.dndv = dndvs;
        }
    
        Color SurfaceInteraction::Le(const Vec3& dir) const
        {
            auto areaLight = primitive ? primitive->getAreaLight() : nullptr;
            return areaLight ? areaLight->L(*this, dir) : Color(0.f, 0.f, 0.f);
        }
}