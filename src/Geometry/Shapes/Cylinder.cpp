#include "Cylinder.hpp"

namespace Geo 
{

    bool Cylinder::intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool) const
    {
        float a = r.d.x * r.d.x + r.d.y * r.d.y;

        if (a < EPSILON_8)  
            return false;

        float b = 2.f * (r.o.x * r.d.x + r.o.y * r.d.y);
        float c = r.o.x * r.o.x + r.o.y * r.o.y - radius * radius;
        
        float delta = b * b - 4.f * a * c;

        if (delta < EPSILON_8) 
            return false;

        float sqr = std::sqrt(delta);

        float t0 = (-b - sqr) / (2.f * a);
        float t1 = (-b + sqr) / (2.f * a);


        if (t0 > r.tMax || t1 < r.tMin) 
            return false;

        float t = t0;
        if(t < r.tMin)
        {
            t = t1;
            if(t > r.tMax)
                return false;
        }


        Point3 pHit = r(t);   


        float hitRad = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
        if (hitRad < EPSILON_8)
            return false;

        pHit.x *= radius / hitRad;
        pHit.y *= radius / hitRad;


        float phi = std::atan2(pHit.y, pHit.x);

        if(phi < 0.f)
            phi += 2.f * PI;

        if(pHit.z < zMin || pHit.z > zMax || phi > phiMax)
        {
            if(t == t1)
                return false;
            t = t1;

            if(t > r.tMax)
                return false;    

            pHit = r(t);
            hitRad = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
            pHit.x *= radius / hitRad;
            pHit.y *= radius / hitRad;
            phi = std::atan2(pHit.y, pHit.x);

            if (phi < 0.f) 
                phi += 2.f * PI;

            if (pHit.z < zMin || pHit.z > zMax || phi > phiMax)
               return false;
        }


        if(tHit)
            *tHit = t;
        
        if(sf)
        {
            if (phi < 0.f) 
                phi += 2.f * PI;

            float u = phi / phiMax;
            float v = (pHit.z - zMin) / (zMax - zMin);
            Vec3 pError = static_cast<float>(gamma(3)) * abs(Vec3(pHit.x, pHit.y, 0.f));

            sf->p = pHit;
            sf->n = static_cast<Normal3>(pHit) / radius;
            sf->n.z = 0.f;
            sf->wo = -r.d;
            sf->uv = Point2(u, v);
            sf->time = r.time;
            sf->pError = pError;
            sf->shape = this;
        }
        return true;
    }
    bool Cylinder::intersectP(const Ray &r, bool) const
    {
        float a = r.d.x * r.d.x + r.d.y * r.d.y;

        if (a < EPSILON_8)  
            return false;
        float b = 2.f * (r.o.x * r.d.x + r.o.y * r.d.y);
        float c = r.o.x * r.o.x + r.o.y * r.o.y - radius * radius;
        
        float delta = b * b - 4.f * a * c;

        if (delta < EPSILON_8) 
            return false;

        float sqr = std::sqrt(delta);

        float t0 = (-b - sqr) / (2.f * a);
        float t1 = (-b + sqr) / (2.f * a);


        if (t0 > r.tMax || t1 < r.tMin) 
            return false;

        float t = t0;
        if(t < r.tMin)
        {
            t = t1;
            if(t > r.tMax)
                return false;
        }

        Point3 pHit = r(t);   

        float hitRad = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
        if (hitRad < EPSILON_8)
            return false;
        pHit.x *= radius / hitRad;
        pHit.y *= radius / hitRad;

        float phi = std::atan2(pHit.y, pHit.x);

        if(phi < 0.f)
            phi += 2.f * PI;

        if(pHit.z < zMin || pHit.z > zMax || phi > phiMax)
        {
            if(t == t1)
                return false;
            t = t1;

            if(t > r.tMax)
                return false;    

            pHit = r(t);
            hitRad = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
            pHit.x *= radius / hitRad;
            pHit.y *= radius / hitRad;
            phi = std::atan2(pHit.y, pHit.x);

            if (phi < 0.f) 
                phi += 2.f * PI;
            
            if (pHit.z < zMin || pHit.z > zMax || phi > phiMax)
               return false;
        }

        return true;

    }
    Bounds3f Cylinder::objectBound() const
    {
        return Bounds3f(Point3(-radius, -radius, zMin),
                        Point3(radius, radius, zMax));
    }
    float Cylinder::area() const
    {
        return phiMax * radius * (zMax - zMin);
    }    
    Interaction Cylinder::sample(const Point2& u, float* pdf) const 
    {
        float z = zMin + u.x * (zMax - zMin);
        float phi = u.y * phiMax;

        Point3 pObj(radius * std::cos(phi), radius * std::sin(phi), z);

        Interaction it;
        it.p = pObj;
        it.n = Normal3(std::cos(phi),std::sin(phi), 0.f);
        it.pError = static_cast<float>(gamma(3)) * abs(Vec3(it.p.x, it.p.y, 0.f));

        if (reverseOrientation)
            it.n = -it.n;

        *pdf = 1.f / area();
    
        return it;
    }   

};