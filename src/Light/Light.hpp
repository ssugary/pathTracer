#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Geometry/Interactions/Interaction.hpp"
#include "Utils/common.hpp"

namespace Luz 
{
    class VisibilityTester;

    enum class LightFlag 
    {
        POINT=0,
        DIRECTIONAL,
        AMBIENT, 
        SPOT,
        ENVIRONMENT,
        AREA,
    };
    class Light 
    {
        protected:

            Color intensity;
            Color scale;
            // Geo::Transform lightToWorld;
            // Geo::Transform worldToLight;
            
        public:
            
            LightFlag flag;

            Light(const Color& intensity, const Color& scale)
            : intensity(intensity), scale(scale) {};

            virtual ~Light() = default;
            virtual Color sampleLi(const Geo::Interaction& ref, 
                                    const Point2& u, 
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const = 0;
            virtual float pdf(const Geo::Interaction& ref, const Vec3& wi) const = 0;

    };
}; //< namespace Luz


#endif //< LIGHT_HPP