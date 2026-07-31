#ifndef AREA_LIGHT_HPP
#define AREA_LIGHT_HPP

#include "Geometry/Interactions/Interaction.hpp"
#include "Geometry/Transformation/Transform.hpp"
#include "Light.hpp"
#include "Geometry/Shapes/Shape.hpp"
#include <memory>
#include "Utils/common.hpp"
namespace Luz 
{
    class AreaLight : public Light
    {

        public:
        
            std::shared_ptr<Geo::Shape> shape;

            AreaLight(const Color& intensity, const Color& scale)
            : Light(intensity, scale) {flag = LightFlag::AREA;};

            virtual Color L(const Interaction &intr, const Vec3 &w) const = 0;
                                                
    };
}; //< namespace Luz


#endif //< LIGHT_HPP