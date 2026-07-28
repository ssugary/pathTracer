#include "GeometricPrimitive.hpp"


namespace Prim 
{

    GeometricPrimitive::GeometricPrimitive(std::shared_ptr<Shape> shape,
                                std::shared_ptr<Mat::Material> material)
    : shape(shape), material(material) {};

    bool GeometricPrimitive::intersect(const Ray &r, SurfaceInteraction *sf) const 
    {
        float tHit;
        if (!shape->intersect(r, &tHit, sf)) 
            return false;

        sf->primitive = this;
        r.tMax = tHit;        

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
        return material;
    }
    
    
};