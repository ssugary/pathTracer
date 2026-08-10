#include "OtherSpectrum.hpp"
#include "Utils/common.hpp"
#include <cmath>

namespace ssrt 
{

    ConstSpectrum::ConstSpectrum(float c) : c(c) {};

    float ConstSpectrum::operator()(float) const{return c;}


    DenselySampledSpectrum::DenselySampledSpectrum(Spectrum* spec, int lambdaMin, int lambdaMax)
    : lambdaMin(lambdaMin), lambdaMax(lambdaMax)
    {   
        values.resize(lambdaMax - lambdaMin + 1);
        if(spec)
        {
            for(int lambda{lambdaMin}; lambda <= lambdaMax; ++lambda)
                values[lambda - lambdaMin] = (*spec)(lambda);
        }
    }

    float DenselySampledSpectrum::operator()(float lambda) const 
    {
        std::size_t offset = static_cast<std::size_t>(std::lround(lambda) - lambdaMin);

        if(offset >= values.size())
            return 0.f;

        return values[offset];
    }

    PiecewiseSpectrum::PiecewiseSpectrum(std::span<float> lambdas, std::span<float> values)
    {
        this->lambdas.assign(lambdas.begin(), lambdas.end());
        this->values.assign(values.begin(), values.end());
    }

    float PiecewiseSpectrum::operator()(float lambda) const 
    {
        if (lambdas.empty()) 
            return 0.f;
        
        if (lambda <= lambdas.front()) 
            return values.front();

        if (lambda >= lambdas.back())  
            return values.back();

        auto it = std::lower_bound(lambdas.begin(), lambdas.end(), lambda);
        
        std::size_t idx_high = std::distance(lambdas.begin(), it);
        std::size_t idx_low  = idx_high - 1;
        float t = (lambda - lambdas[idx_low]) / (lambdas[idx_high] - lambdas[idx_low]);

        return ::lerp(t, values[idx_low], values[idx_high]);
    }


    BlackbodySpectrum::BlackbodySpectrum(float t)
    : t(t)
    {
        float lambdaMax = 2.8977721e-3f / t;
        normalizationFactor = 1.f / ::planck(lambdaMax, t);
    }

    float BlackbodySpectrum::operator()(float lambda) const 
    {
        return ::planck(lambda * EPSILON_9, t) * normalizationFactor;
    }

    RGBAlbedoSpectrum::RGBAlbedoSpectrum(const Color& rgb)
    : c0(rgb.r), c1(rgb.g), c2(rgb.b) {};
    
    RGBAlbedoSpectrum::RGBAlbedoSpectrum(float r, float g, float b)
    : c0(r), c1(g), c2(b) {};
    
    float RGBAlbedoSpectrum::operator()(float lambda) const
    {
        float b = c2 * std::exp(-std::pow(lambda - 450.f, 2.f) / 3000.f);
        float g = c1 * std::exp(-std::pow(lambda - 540.f, 2.f) / 3000.f);
        float r = c0 * std::exp(-std::pow(lambda - 630.f, 2.f) / 3000.f);
        
        return r + g + b;
    }


    RGBIlluminationSpectrum::RGBIlluminationSpectrum(const Color& rgb)
    : c0(rgb.r), c1(rgb.g), c2(rgb.b) {};
    
    RGBIlluminationSpectrum::RGBIlluminationSpectrum(float r, float g, float b)
    : c0(r), c1(g), c2(b) {};

    float RGBIlluminationSpectrum::operator()(float lambda) const 
    {
        float b = c2 * std::exp(-std::pow(lambda - 450.f, 2.f) / 3000.f);
        float g = c1 * std::exp(-std::pow(lambda - 540.f, 2.f) / 3000.f);
        float r = c0 * std::exp(-std::pow(lambda - 630.f, 2.f) / 3000.f);
        
        return std::max(0.f, r + g + b);
    }



};