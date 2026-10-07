#pragma once

#include <Geometry/Metric/Metric.hpp>
#ifndef GEOMETRIC_PRIMITIVE_HPP
#define GEOMETRIC_PRIMITIVE_HPP

#include "Primitive.hpp"
#include "Geometry/Shapes/Shape.hpp"
#include "RelativisticPrimitive.hpp"

using namespace Geo;
namespace Prim 
{
    class GeometricPrimitive : public Primitive 
    {
        private:
            std::shared_ptr<Shape> shape;
            std::shared_ptr<Mat::Material> material;
            std::shared_ptr<Luz::AreaLight> areaLight;
        public:
            GeometricPrimitive() = default;
            GeometricPrimitive(std::shared_ptr<Shape> shape,
                               std::shared_ptr<Mat::Material> material,
                               std::shared_ptr<Luz::AreaLight> areaLight = nullptr);

            bool intersect(const Ray&, SurfaceInteraction *) const override;
            bool intersectP(const Ray&) const override;
            void setMaterial(const std::shared_ptr<Mat::Material>&);
            const std::shared_ptr<Mat::Material> getMaterial() const override;
            const std::shared_ptr<Luz::AreaLight> getAreaLight() const override;
            Bounds3f objectBound() const override;

    };

    class GeometricRelativisticPrimitive : public RelativisticPrimitive
    {
        private:
            
            std::shared_ptr<Primitive> prim3D;
            const Geo::Metric& metric;
            Vec4 velocity;

        public:

            GeometricRelativisticPrimitive(std::shared_ptr<Primitive> prim3D, const Geo::Metric& metric, const Vec4& velocity = {1.f, 0.f, 0.f, 0.f});
            bool intersect(const GeodesicStep&, SpaceTimeInteraction*) const override;
            bool intersectP(const GeodesicStep&) const override;

    };
}; //< namespace Prim 

#endif //< GEOMETRIC_PRIMITIVE_HPP