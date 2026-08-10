#include "Plane.hpp"
#include <cmath>


namespace Geo 
{
    bool Plane::intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool) const
    {
        float dn = dot(r.d, n);

        if(std::abs(dn) < EPSILON_6)
            return false;
        
        auto w = p - r.o;

        float t = dot(w, n) / dn;


        if(t < r.tMin || t > r.tMax)
            return false;
        

        if(tHit) 
            *tHit = t;

        if(sf)
        {
            sf->time = r.time;
            sf->n = dn < 0 ? n : -n;;
            
            sf->p = r(t);
            sf->pError = static_cast<float>(gamma(3)) * abs(static_cast<Vec3>(sf->p));;
            sf->wo = -r.d;
            sf->shape = this;
            
        }

        return true;
    }
    bool Plane::intersectP(const Ray &r, bool) const
    {   
        float dn = dot(r.d, n);

        if(std::abs(dn) < EPSILON_6)
            return false;
        
        auto w = p - r.o;

        float t = dot(w, n) / dn;


        if(t < r.tMin || t > r.tMax)
            return false;
        

        return true;

    }
    Bounds3f Plane::objectBound() const 
    {
        return Bounds3f();
    }
    float Plane::area() const 
    {
        return INF;
    }
    Interaction Plane::sample(const Point2&, float* pdf) const
    {
        if(pdf)
            *pdf = 0.f;

        return Interaction();
    }
}