#include "GeometricPrimitive.hpp"
#include "Geometry/Interactions/SpaceTimeInteraction.hpp"

namespace Prim 
{

    GeometricPrimitive::GeometricPrimitive(std::shared_ptr<Shape> shape,
                                           std::shared_ptr<Mat::Material> material,
                                           std::shared_ptr<Luz::AreaLight> areaLight)
    : shape(shape), material(material), areaLight(areaLight){};

    bool GeometricPrimitive::intersect(const Ray &r, SurfaceInteraction *sf) const 
    {
        float tHit;
        if (!shape->intersect(r, &tHit, sf)) 
            return false;

        r.tMax = tHit;        
        sf->primitive = this;

        return true;
    }
    
    bool GeometricPrimitive::intersectP(const Ray &r) const 
    {
        return this->shape->intersectP(r);
    }
    
    Bounds3f GeometricPrimitive::objectBound() const 
    {
        return this->shape->objectBound();
    }

    void GeometricPrimitive::setMaterial(const std::shared_ptr<Mat::Material> &m)
    {
        this->material = m;
    }
    
    
    const std::shared_ptr<Mat::Material> GeometricPrimitive::getMaterial() const 
    { 
        return this->material;
    }
    
    const std::shared_ptr<Luz::AreaLight> GeometricPrimitive::getAreaLight() const 
    { 
        return this->areaLight;
    }
    

    GeometricRelativisticPrimitive::GeometricRelativisticPrimitive(std::shared_ptr<Primitive> prim3D, const Geo::Metric& metric, const Vec4& velocity)
    : prim3D(prim3D), metric(metric), velocity(velocity) {}

    bool GeometricRelativisticPrimitive::intersect(const GeodesicStep& step, SpaceTimeInteraction* isect) const 
    {
        
        Point3 p1 = metric.toCartesian(step.prevRay.x);
        Point3 p2 = metric.toCartesian(step.currRay.x);

        Vec3 delta = p2 - p1;
        float dist = ::length(delta);

        if(dist <= EPSILON_8) 
            return false;

        Vec3 dir = delta / dist;
        Ray ray3D(p1, dir, 0.f, dist);
        Geo::SurfaceInteraction sf;

        if(!prim3D->intersect(ray3D, &sf)) 
            return false;
        

        if(isect) 
        {
            float t = ray3D.tMax / dist;
            t = std::clamp(t, 0.f, 1.f);

            isect->isect3d = sf;
            isect->velocity = velocity;
            isect->p = ::lerp(t, step.prevRay.p, step.currRay.p);
            isect->x = ::lerp(t, step.prevRay.x, step.currRay.x);
            isect->tStep = t;
        }

        return true;
    }

    bool GeometricRelativisticPrimitive::intersectP(const GeodesicStep& step) const
    {
        if(!prim3D) return 
            false;

        Point3 p1 = metric.toCartesian(step.prevRay.x);
        Point3 p2 = metric.toCartesian(step.currRay.x);

        Vec3 delta = p2 - p1;
        float dist = ::length(delta);

        if(dist <= EPSILON_8) 
            return false;

        Vec3 dir = delta / dist;
        Ray ray3D(p1, dir, 0.f, dist);

        return prim3D->intersectP(ray3D);
    }

};