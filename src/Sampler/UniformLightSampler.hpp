#ifndef UNIFORM_LIGHT_SAMPLER_HPP
#define UNIFORM_LIGHT_SAMPLER_HPP

#include <vector>
#include <memory>

#include "Light/Light.hpp"


namespace Sam 
{
    class UniformLightSampler 
    {
        private:
            std::vector<std::shared_ptr<Luz::Light>> lights;

        public:
        

    };

} //< namespace Sam


#endif //< UNIFORM_LIGHT_SAMPLER_HPP