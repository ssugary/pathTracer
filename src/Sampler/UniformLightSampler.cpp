#include "UniformLightSampler.hpp"

namespace Sam 
{
    UniformLightSampler::UniformLightSampler(const std::vector<std::shared_ptr<Luz::Light>>& lights)
    : lights(lights) {};

    std::shared_ptr<Luz::Light> UniformLightSampler::sample(float u, float* outPmf) const 
    {
        if(lights.empty())
        {
            *outPmf = 0.f;
            return nullptr;
        }

        int idx = std::clamp(static_cast<int>(u * lights.size()), 0, static_cast<int>(lights.size() - 1));

        *outPmf = pmf();
        return lights[idx];
    }

    float UniformLightSampler::pmf() const 
    {
        return lights.empty() ? 0.f : 1.f / static_cast<float>(lights.size());
    }

    bool UniformLightSampler::empty() const 
    {
        return lights.empty();
    }
    std::vector<std::shared_ptr<Luz::Light>> UniformLightSampler::get() const
    {
        return this->lights;
    }
}
