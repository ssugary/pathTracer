#include "Disk.hpp"

namespace Geo 
{

    bool Disk::intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool) const 
    {
        if(r.d.z == 0)
            return false;
        float t = (height - r.o.z) / r.d.z;

        if(t < r.tMin || t > r.tMax)
            return false;

        Point3 phit = r(t);
        float dist2 = phit.x * phit.x + phit.y * phit.y;
        if(dist2 > radius * radius || dist2 < innerRadius * innerRadius)
            return false;

        float phi = std::atan2(phit.y, phit.x);
        if (phi < 0) 
            phi += 2 * PI;
        if (phi > phiMax)
            return false;

        if(tHit)
            *tHit = t;

        if(sf)
        {
            Vec3 pError = static_cast<float>(gamma(3)) * abs(Vec3(phit.x, phit.y, 0.f));
            sf->p = phit;
            sf->wo = -r.d;
            sf->n = Normal3(0.f, 0.f, 1.f);

            if(reverseOrientation)
                sf->n = -sf->n;

            sf->pError = pError;
            float u = phi / phiMax;
            float v = 1 - ((std::sqrt(dist2) - innerRadius)/(radius - innerRadius));
            sf->uv = {u, v};
            sf->shape = this;

        }
        return true;
    };
    bool Disk::intersectP(const Ray &r, bool) const 
    {
        if(r.d.z == 0)
            return false;
        float t = (height - r.o.z) / r.d.z;

        if(t < r.tMin || t > r.tMax)
            return false;

        Point3 phit = r(t);
        float dist2 = phit.x * phit.x + phit.y * phit.y;
        if(dist2 > radius * radius || dist2 < innerRadius * innerRadius)
            return false;

        float phi = std::atan2(phit.y, phit.x);
          if (phi < 0) 
            phi += 2 * PI;
          if (phi > phiMax)
              return false;
            
        return true;
    };
    Bounds3f Disk::objectBound() const 
    {
        return Bounds3f(Point3(-radius, -radius, height),
                        Point3(radius, radius, height));
    };
    float Disk::area() const 
    {
        return 0.5f * phiMax * (radius * radius - innerRadius * innerRadius);
    };
    Interaction Disk::sample(const Point2& u, float* pdf) const 
    {
        Point2 pd = concentricSampleDisk(u);
        Point3 pObj(pd.x * radius, pd.y * radius, height);

        Interaction it;
        it.p = pObj;
        it.n = Normal3(0.f, 0.f, 1.f);
        it.pError = static_cast<float>(gamma(3)) * abs(Vec3(it.p.x, it.p.y, 0.f));
        
        if (reverseOrientation)
            it.n = -it.n;

        *pdf = 1.f / area();

        return it;
    }

};