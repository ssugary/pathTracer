#include "PrimitiveList.hpp"

namespace Prim 
{

    PrimitiveList::PrimitiveList(std::vector<std::shared_ptr<Primitive>> prim)
    : primitives(prim) {};

    void PrimitiveList::add(const std::shared_ptr<Primitive> &object) 
    {
        primitives.push_back(object);
    }


    bool PrimitiveList::intersect(const Ray &ray, SurfaceInteraction *isect) const 
    {
        bool hit = false;
        for (const auto &object : primitives) 
        {
            if (object->intersect(ray, isect)) 
            {
                hit = true;
            }
        }
        return hit;
    }

    bool PrimitiveList::intersectP(const Ray &ray) const 
    {
        for (const auto &object : primitives) 
        {
            if (object->intersectP(ray)) 
            {
                return true;
            }
        }
        return false;
    }

    const std::vector<std::shared_ptr<Primitive>> &PrimitiveList::getPrimitives() const 
    {
        return primitives;
    }

    Bounds3f PrimitiveList::objectBound() const 
{
    if (primitives.empty()) return Bounds3f();
    
    Bounds3f bounds = primitives[0]->objectBound();
    
    for (size_t i = 1; i < primitives.size(); ++i) 
    {
        
         bounds = boundUnion(bounds, primitives[i]->objectBound());
    }
    
    return bounds;
}
};