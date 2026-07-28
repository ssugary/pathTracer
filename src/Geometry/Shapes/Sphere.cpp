#include "Sphere.hpp"
#include <Geometry/Interactions/Interaction.hpp>
#include <Utils/common.hpp>

namespace Geo 
{
    bool Sphere::intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool) const 
    {        
        float a = r.d.x * r.d.x + r.d.y * r.d.y + r.d.z * r.d.z;
        float b = 2.f * (r.d.x * r.o.x + r.d.y * r.o.y + r.d.z * r.o.z);
        float c = r.o.x * r.o.x + r.o.y * r.o.y + r.o.z * r.o.z - radius * radius;


        float delta = b * b - 4.f * a * c;

        if (delta < EPSILON_8) 
            return false;

        float sqr = std::sqrt(delta);

        float t0 = (-b + sqr) / (2 * a);
        float t1 = (-b - sqr) / (2 * a);

        if (t0 > t1) 
            std::swap(t0, t1);

        if (t0 > r.tMax || t1 < r.tMin) 
            return false;

        if (t0 < r.tMin) {
            t0 = t1;
            if (t0 > r.tMax) {
                return false;   
            }
        }

        Point3 phit = r(t0);   

        if (phit.x == 0 && phit.y == 0) 
            phit.x = radius * EPSILON_6;

        float phi = std::atan2(phit.y, phit.x);

        if(phi < 0)
            phi += 2 * PI;

        float theta = std::acos(clamp(-phit.z / radius, -1.0f, 1.0f));

        if(tHit)
            *tHit = t0; 

        if (sf) {
            sf->time = t0;

            float u = phi / (2.f * PI);
            float v = theta / PI;

            sf->uv = Point2(u, v);
            sf->p = phit + static_cast<Vec3>(phit) * SHADOW_EPSILON;
            sf->n =  Normal3(phit) / radius;
            sf->wo = -r.d;
            sf->shape = this;

        }

        return true;
    }
    bool Sphere::intersectP(const Ray &r, bool) const 
    {
        float a = r.d.x * r.d.x + r.d.y * r.d.y + r.d.z * r.d.z;
        float b = 2.f * (r.d.x * r.o.x + r.d.y * r.o.y + r.d.z * r.o.z);
        float c = r.o.x * r.o.x + r.o.y * r.o.y + r.o.z * r.o.z - radius * radius;


        float delta = b * b - 4.f * a * c;

        if (delta < EPSILON_8) 
            return false;

        float sqr = std::sqrt(delta);

        float t0 = (-b + sqr) / (2 * a);
        float t1 = (-b - sqr) / (2 * a);

        if (t0 > t1) 
            std::swap(t0, t1);
        

        if (t0 > r.tMax || t1 < r.tMin)
            return false;

        if (t0 < r.tMin) 
        {
            t0 = t1;
            if (t0 > r.tMax) 
                return false;
        }

        return true;
    }
    // Interaction Sphere::sample(const Point2& u, float* pdf) const 
    // {
    //     float z = 1.f - 2.f * u.x;
    //     float r = std::sqrt(std::max(0.f, 1.f - z * z));
    //     float phi = 2.f * PI * u.y;

    //     Point3 pobj = Point3(r * std::cos(phi), r * std::sin(phi), z) * radius;

    //     Interaction it;
    //     it.p = pobj;
    //     it.n = static_cast<Normal3>(pobj) / radius;
    //     *pdf = 1.f/area();

    //     return it;
    // }

    Bounds3f Sphere::objectBound() const  
    {   
        return Bounds3f(Point3(-radius, -radius, -radius), 
                        Point3( radius,  radius,  radius));
    }

    float Sphere::area() const
    {
        return phiMax * radius * (zMax - zMin); 
    }
}