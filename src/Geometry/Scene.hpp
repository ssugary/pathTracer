#ifndef SCENE_HPP
#define SCENE_HPP

#include "Primitives/AggregatePrimitive.hpp"
#include "Backgrounds/Background.hpp"
#include "Light/Light.hpp"
#include <memory>
#include <vector>

using namespace Prim;

namespace Geo 
{

    class Scene 
    {
        private:
            Bounds3f worldBounds;

        public:
            std::vector<std::shared_ptr<Luz::Light>> lights;
            std::shared_ptr<Background> background;
            std::shared_ptr<AggregatePrimitive> aggregate;

            ~Scene() = default;
            Scene(std::shared_ptr<AggregatePrimitive> aggregate, std::shared_ptr<Background> background, std::vector<std::shared_ptr<Luz::Light>> lights)
            : lights(lights), background(std::move(background)), aggregate(std::move(aggregate)) 
            {
                worldBounds = aggregate->objectBound();
                // for(const auto& light : lights)
                //     light->preprocess(*this);
            };
            bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
            bool intersectP(const Ray& ray) const;

            const Bounds3f& worldBound() const;
    };  

}; //< namespace Geo


#endif //< SCENE_HPP