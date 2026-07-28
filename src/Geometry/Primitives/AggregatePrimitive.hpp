#ifndef AGGREGATE_PRIMITIVE_HPP
#define AGGREGATE_PRIMITIVE_HPP

#include "Materials/Material.hpp"
#include "Primitive.hpp"
#include <memory>

using namespace Geo;

namespace Prim 
{

    class AggregatePrimitive : public Primitive
    {
        const std::shared_ptr<Mat::Material> getMaterial() const override
        {
            //Area Light
            
            return nullptr;
        }
    };
}; //< namespace Prim

#endif //< AGGREGATE_PRIMITIVE_HPP