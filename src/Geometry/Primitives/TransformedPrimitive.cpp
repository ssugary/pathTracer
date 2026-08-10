#include "TransformedPrimitive.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"

namespace Prim 
{
    TransformedPrimitive::TransformedPrimitive(const Transform* O2W, const Transform* W2O, const std::shared_ptr<Primitive>& prim)
    : prim(prim), O2W(O2W), W2O(W2O) {};

            

    Bounds3f TransformedPrimitive::objectBound() const 
    {
        return (*O2W)(prim->objectBound());
    }

    bool TransformedPrimitive::intersect(const Geo::Ray &r, Geo::SurfaceInteraction *sf) const 
    {
        auto ray = (*W2O)(r);

        if (!prim->intersect(ray, sf))
            return false;

        // r.tMin = ray.tMin;
        // r.tMax = ray.tMax;

        *sf = (*O2W)(*sf);
        sf->primitive = this;

        return true;

    }

    bool TransformedPrimitive::intersectP(const Geo::Ray &r) const 
    {
        Ray ray = (*W2O)(r);
        
        return prim->intersectP(ray);

    }
    const std::shared_ptr<Mat::Material> TransformedPrimitive::getMaterial() const 
    {
        return prim->getMaterial();
    }
    const std::shared_ptr<Luz::AreaLight> TransformedPrimitive::getAreaLight() const 
    {
        return prim->getAreaLight();
    }

};