#include "TextureMap.hpp"

namespace Geo 
{
    
    UVMapping2D::UVMapping2D(float su, float sv, float du, float dv)
    : su(su), sv(sv), du(du), dv(dv) {};

    Point2 UVMapping2D::map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const 
    {
        if(dstdx)
            *dstdx = Vec2(su * sf.dudx, sv * sf.dvdx);

        if(dstdy)
            *dstdy = Vec2(su * sf.dudy, sv * sf.dvdy);

        return Point2(su * sf.uv.x + du, sv * sf.uv.y + dv);
    }

    Point2 SphericalMapping2D::sphere(const Point3& p) const
    {
        Vec3 v = ::normalize(static_cast<Vec3>(p));
        float theta = ::sphericalTheta(v);
        float phi = ::sphericalPhi(v);

        return Point2(theta / PI, phi / (2.f * PI));
    }

    SphericalMapping2D::SphericalMapping2D(const Transform& W2T)
    : W2T(W2T) {};

    Point2 SphericalMapping2D::map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const 
    {
        Point3 p = W2T(sf.p);

        if(dstdx || dstdy)
        {
            Vec3 dpdx = W2T(sf.dpdx);
            Vec3 dpdy = W2T(sf.dpdy);
    
            float rho2 = p.x * p.x + p.y * p.y;
            float r2 = rho2 + p.z * p.z;
            float rho = std::sqrt(rho2);
    
            Vec3 ds{0.f};
            Vec3 dt{0.f};
    
            if (rho2 > EPSILON_8 && r2 > EPSILON_8) 
            {
                ds = (1.f / (2.f * PI * rho2)) * Vec3(-p.y, p.x, 0.f);
                dt = (1.f / (r2  * PI * rho))  * Vec3(p.z * p.x, p.z * p.y, -rho2);
            }
    
            if(dstdx)
                *dstdx = Vec2(::dot(ds, dpdx), ::dot(dt, dpdx));
            if(dstdy)
                *dstdy = Vec2(::dot(ds, dpdy), ::dot(dt, dpdy));
        }

        return sphere(p);
    }

    Point2 CylindricalMapping2D::cylinder(const Point3& p) const
    {
        Vec3 v = ::normalize(static_cast<Vec3>(p));
        return Point2((PI + std::atan2(v.y, v.x)) / (2.f * PI), v.z);
    }

    CylindricalMapping2D::CylindricalMapping2D(const Transform& W2T)
    : W2T(W2T) {};

    Point2 CylindricalMapping2D::map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const 
    {
        Point3 p = W2T(sf.p);

        if(dstdx || dstdy)
        {
            Vec3 dpdx = W2T(sf.dpdx);
            Vec3 dpdy = W2T(sf.dpdy);
    
            float rho2 = p.x * p.x + p.y * p.y;
    
            Vec3 ds{0.f};
            Vec3 dt{0.f, 0.f, 1.f};
    
            if (rho2 > EPSILON_8) 
                ds = (1.f / (2.f * PI * rho2)) * Vec3(-p.y, p.x, 0.f);
    
            
            if(dstdx)
                *dstdx = Vec2(::dot(ds, dpdx), ::dot(dt, dpdx));
            if(dstdy)
                *dstdy = Vec2(::dot(ds, dpdy), ::dot(dt, dpdy));
        }


        return cylinder(p);
    }

    PlanarMapping2D::PlanarMapping2D(const Vec3& vs, const Vec3& vt, float ds, float dt)
    : vs(vs), vt(vt), ds(ds), dt(dt) {};

    Point2 PlanarMapping2D::map(const SurfaceInteraction& sf, Vec2* dstdx, Vec2* dstdy) const 
    {
        
        if(dstdx)
            *dstdx = Vec2(::dot(sf.dpdx, vs), ::dot(sf.dpdx, vt));

        if(dstdy)
            *dstdy = Vec2(::dot(sf.dpdy, vs), ::dot(sf.dpdy, vt));

        return Point2(ds + ::dot(static_cast<Vec3>(sf.p), vs), dt + ::dot(static_cast<Vec3>(sf.p), vt));
    }

    TransformMapping3D::TransformMapping3D(const Transform& W2T)
    : W2T(W2T) {};

    Point3 TransformMapping3D::map(const SurfaceInteraction& sf, Vec3* dpdx, Vec3* dpdy) const 
    {
        if(dpdx)
            *dpdx = W2T(sf.dpdx);
        if(dpdy)
            *dpdy = W2T(sf.dpdy);

        return W2T(sf.p);
    }
}