#include "GeometricPrimitive.hpp"


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
    

    GeometricRelativisticPrimitive::GeometricRelativisticPrimitive(std::shared_ptr<Primitive> prim3D, const Point4& center, float spin)
    : prim3D(prim3D), centerSpaceTime(center), spin(spin) {}

    bool GeometricRelativisticPrimitive::intersect(const GeodesicStep& step, SpaceTimeInteraction* isect) const 
    {
        Point3 p1 = transformToLocal3d(step.prevRay.x, centerSpaceTime, spin);
        Point3 p2 = transformToLocal3d(step.currRay.x, centerSpaceTime, spin);
        
    }

    bool GeometricRelativisticPrimitive::intersectP(const GeodesicStep& step) const
    {

    }

};