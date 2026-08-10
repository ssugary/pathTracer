#pragma once

#ifndef PRIMITIVE_LIST_HPP
#define PRIMITIVE_LIST_HPP

#include "AggregatePrimitive.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include <vector>

namespace Prim {

class PrimitiveList : public AggregatePrimitive 
{
        private:

            std::vector<std::shared_ptr<Primitive>> primitives;

        public:

            PrimitiveList() = default; 
            PrimitiveList(std::vector<std::shared_ptr<Primitive>> prim);

            void add(const std::shared_ptr<Primitive> &primitive);
            bool intersect(const Ray &ray, SurfaceInteraction *isect) const override;
            bool intersectP(const Ray &ray) const override;
            Bounds3f objectBound() const override;

            const std::vector<std::shared_ptr<Primitive>> &getPrimitives() const;
    };
} // namespace Prim

#endif //< PRIMITIVE_LIST_HPP