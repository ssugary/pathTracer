#pragma once

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
            std::shared_ptr<Luz::Light> envLight;
        
            UniformLightSampler() = default;
            UniformLightSampler(const std::vector<std::shared_ptr<Luz::Light>>& lights);

            std::shared_ptr<Luz::Light> sample(float u, float* outPmf) const;
            float pmf() const;
            bool empty() const;
            std::vector<std::shared_ptr<Luz::Light>> get() const;


    };

} //< namespace Sam


#endif //< UNIFORM_LIGHT_SAMPLER_HPP