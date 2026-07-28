#include "Scene.hpp"


namespace Geo 
{
    bool Scene::intersect(const Ray& ray, SurfaceInteraction* isect) const
    {
        return aggregate ? aggregate->intersect(ray, isect) : false;
    }
    bool Scene::intersectP(const Ray& ray) const
    {
        return aggregate ? aggregate->intersectP(ray) : false;
    }
    const Bounds3f& Scene::worldBound() const
    {
        return worldBounds;
    }


};