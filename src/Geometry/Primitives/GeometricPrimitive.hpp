#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "Primitive.hpp"
#include "Geometry/Shapes/Shape.hpp"

using namespace Geo;
namespace Prim 
{
    class GeometricPrimitive : public Primitive 
    {
        private:
            std::shared_ptr<Shape> shape;
            std::shared_ptr<Mat::Material> material;
        public:
            GeometricPrimitive() = default;
            GeometricPrimitive(std::shared_ptr<Shape> shape,
                                std::shared_ptr<Mat::Material> material);

            bool intersect(const Ray&, SurfaceInteraction *) const override;
            bool intersectP(const Ray&) const override;
            void setMaterial(const std::shared_ptr<Mat::Material>&);
            const std::shared_ptr<Mat::Material> getMaterial() const override;
            Bounds3f objectBound() const override;

    };
}; //< namespace Prim 

#endif //< GEOMETRIC_PRIMITIVE_HPP