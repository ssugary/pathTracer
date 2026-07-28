#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "Light.hpp"
#include "VisibilityTester.hpp"

namespace Luz 
{

    class DirectionalLight : public Light
    {
        private:

            Vec3 dir;
            float worldRadius;

        public:


            DirectionalLight(Color intensity, Color scale, Vec3 dir, float worldRadius)
            : Light(intensity, scale), dir(dir), worldRadius(worldRadius)
            {
                flag = LightFlag::DIRECTIONAL;
            };

            Color sampleLi(const Geo::Interaction& hit, 
                            // const Point2& u, 
                            Vec3* wi, VisibilityTester* vis) const override
            {
                *wi = normalize(-dir);
               
                
                Interaction lpos;
                lpos.p = hit.p + (*wi) * worldRadius;
                    
                *vis = VisibilityTester(hit, lpos);
                

                return intensity * scale;
            }
    };

}

#endif //< DIRECTIONAL_LIGHT_HPP