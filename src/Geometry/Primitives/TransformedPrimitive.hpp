#ifndef TRANSFORMED_PRIMITIVE_HPP
#define TRANSFORMED_PRIMITIVE_HPP

#include "Geometry/Primitives/GeometricPrimitive.hpp"
#include "Geometry/Primitives/Primitive.hpp"
#include "Geometry/Transformation/Transform.hpp"
#include "Utils/common.hpp"
#include <memory>

namespace Prim 
{
    class TransformedPrimitive : public Primitive
    {
        private:

            std::shared_ptr<Primitive> prim;
            const Transform* O2W;
            const Transform* W2O;

        public:

            TransformedPrimitive(const Transform* O2W, const Transform* W2O, const std::shared_ptr<Primitive>& prim);
            

            const std::shared_ptr<Mat::Material> getMaterial() const override;
            bool intersect(const Geo::Ray &r, Geo::SurfaceInteraction *sf) const override;
            bool intersectP(const Geo::Ray &r) const override;
            Bounds3f objectBound() const override;
            const std::shared_ptr<Luz::AreaLight> getAreaLight() const override;

    };


}; //< namespace Prim

#endif //< TRANSFORMED_PRIMITIVE_HPP