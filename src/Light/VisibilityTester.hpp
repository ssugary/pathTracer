#ifndef VISIBILITY_TESTER_HPP
#define VISIBILITY_TESTER_HPP

#include "Geometry/Interactions/Interaction.hpp"
#include "Geometry/Scene.hpp"
#include "Utils/common.hpp"

namespace Luz 
{
    class VisibilityTester 
    {
        private:

            Interaction p0;
            Interaction p1;
        public:

            VisibilityTester() = default;
            VisibilityTester(Interaction p0, Interaction p1)
            : p0(p0), p1(p1) {};

            bool unoccluded(const Geo::Scene& scene) const
            {
                Ray shadowRay = p0.spawnRayTo(p1);
                return !scene.intersectP(shadowRay);
            }
    };
};

#endif //< VISIBILITY_TESTER_HPP