#pragma once


#ifndef AGGREGATE_PRIMITIVE_HPP
#define AGGREGATE_PRIMITIVE_HPP

#include "Materials/Material.hpp"
#include "Primitive.hpp"

using namespace Geo;

namespace Prim 
{

    class AggregatePrimitive : public Primitive
    {
        const std::shared_ptr<Mat::Material> getMaterial() const override
        {            
            return nullptr;
        }
        const std::shared_ptr<Luz::AreaLight> getAreaLight() const override
        {
            return nullptr;
        }
    };
}; //< namespace Prim

#endif //< AGGREGATE_PRIMITIVE_HPP