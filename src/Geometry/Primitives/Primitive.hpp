#pragma once

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Materials/Material.hpp"
#include "Geometry/Rays/Ray.hpp"
#include <memory>

namespace Geo 
{
    class SurfaceInteraction;
}
namespace Luz 
{
    class AreaLight;
}
namespace Prim 
{

    class Primitive 
    {
        public:
            virtual ~Primitive() = default;
            virtual Bounds3f objectBound() const = 0;
            virtual bool intersect(const Geo::Ray &r, Geo::SurfaceInteraction *sf) const = 0;
            virtual bool intersectP(const Geo::Ray &r) const = 0;
            virtual const std::shared_ptr<Mat::Material> getMaterial() const = 0;
            virtual const std::shared_ptr<Luz::AreaLight> getAreaLight() const = 0;
    };
}


#endif