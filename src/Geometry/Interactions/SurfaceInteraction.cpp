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
        
        return areaLight ? areaLight->L(*this, dir) : Color(0.f);
    }

    void SurfaceInteraction::computeDifferentials(const RayDifferential& ray) const
    {
        if (!ray.hasDifferentials)
        {
            dudx = dvdx = dudy = dvdy = 0.f;
            return;
        }

        float tx = ::dot((p - ray.rxOrigin), n) / ::dot(ray.rxDirection, n);
        Point3 px = ray.rxOrigin + tx * ray.rxDirection;

        dpdx = px - p;

        float ty = ::dot((p - ray.ryOrigin), n) / ::dot(ray.ryDirection, n);
        Point3 py = ray.ryOrigin + ty * ray.ryDirection;

        dpdy = py - p;

        int dim[2];
        if (std::abs(n.x) > std::abs(n.y) && std::abs(n.x) > std::abs(n.z)) 
        {
            dim[0] = 1; 
            dim[1] = 2; 
        } else if (std::abs(n.y) > std::abs(n.z)) 
        {
            dim[0] = 0; 
            dim[1] = 2; 
        } else {
            dim[0] = 0; 
            dim[1] = 1;
        }

        float A = dpdu[dim[0]];
        float B = dpdv[dim[0]];
        float C = dpdu[dim[1]];
        float D = dpdv[dim[1]];

        float det = A * D - B * C;

        if(std::abs(det) < EPSILON_8)
        {
            dudx = 0.f;
            dvdx = 0.f;
            dudy = 0.f;
            dvdy = 0.f;
        }
        else  
        {
    
            float Ex = dpdx[dim[0]];
            float Fx = dpdx[dim[1]];
            dudx = (Ex * D - B * Fx) / det;
            dvdx = (A * Fx - Ex * C) / det;

            float Ey = dpdy[dim[0]];
            float Fy = dpdy[dim[1]];
            dudy = (Ey * D - B * Fy) / det;
            dvdy = (A * Fy - Ey * C) / det;
        }
    }
}