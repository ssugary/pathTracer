#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "Light.hpp"
#include "VisibilityTester.hpp"
#include <Geometry/Interactions/Interaction.hpp>

namespace Luz 
{

    class SpotLight : public Light
    {
        private:

            Point3 from{};
            Vec3 axis{};
            float cutoff{0.f};
            float falloff{0.f};

        public:


            SpotLight(Color intensity, Color scale, Point3 from, Point3 to, float cutoff, float fallof)
            : Light(intensity, scale), from(from), axis(::normalize(to - from)), cutoff(cutoff), falloff(fallof)
            {};

            Color sampleLi(const Geo::Interaction& hit, 
                                    const Point2&, 
                                    Vec3* wi, float* pdf, VisibilityTester* vis) const override
            {
                
                Vec3 dir = from - hit.p;
                Interaction lpos; lpos.p = from;

                *vis = VisibilityTester(hit, lpos);
                *wi = normalize(dir);
                *pdf = 1.0f;

                float angle = std::acos(dot(-(*wi), axis)) * (180.0 / M_PI);

                double spot = 1.0;

                if(angle >= cutoff)
                {
                    return Color();
                }
                else if(angle >= falloff)
                {
                    spot = (cutoff - angle) / (cutoff - falloff);
                }                

                return intensity * scale * spot;
            }
    };

}

#endif //< SPOT_LIGHT_HPP