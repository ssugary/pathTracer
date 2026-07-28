#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "Geometry/Scene.hpp"

namespace Itg
{

    class Integrator 
    {
        public:

            virtual ~Integrator() = default;
            virtual void render(const Geo::Scene& scene) = 0;
    };

}; //< namespace Itg

#endif 