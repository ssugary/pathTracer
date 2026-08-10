#include "Spectrum.hpp"
#include <cmath>
#include "Utils/common.hpp"
namespace ssrt 
{

    SampledSpectrum::SampledSpectrum(float c) {values.fill(c);}
    SampledSpectrum::SampledSpectrum(std::span<const float> v)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
            values[i] = v[i];
        }
    }
    
    float  SampledSpectrum::operator[](const std::size_t i) const
    {
        return this->values[i];
    }
    float& SampledSpectrum::operator[](const std::size_t i)      
    {
        return this->values[i];
    }
    
    SampledSpectrum::operator bool() const
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
            if(values[i] != 0.f)
                return true;
        }
        return false;
    }

    bool SampledSpectrum::operator==(const SampledSpectrum& ss)const
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
            if(values[i] != ss[i])
                return false;
        }
        return true;
    }
    bool SampledSpectrum::operator!=(const SampledSpectrum& ss)const
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
            if(values[i] != ss[i])
                return true;
        }
        return false;
    }
    
    SampledSpectrum& SampledSpectrum::operator=(const SampledSpectrum& ss)
    {
        
        for(std::size_t i{0}; i < 4; ++i)
        {
            values[i] = ss[i];
        }

        return *this;
    }
    SampledSpectrum SampledSpectrum::operator-() const
    {
        SampledSpectrum ss;

        for(std::size_t i{0}; i < 4; ++i)
        {
           ss[i] = -values[i];
        }
        
        return ss;
    }
    
    SampledSpectrum SampledSpectrum::operator+(const SampledSpectrum& ss) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] += ss[i];
        }
        
        return res;
    }
    SampledSpectrum SampledSpectrum::operator-(const SampledSpectrum& ss) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] -= ss[i];
        }
        
        return res;
    }
    SampledSpectrum SampledSpectrum::operator*(const SampledSpectrum& ss) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] *= ss[i];
        }
        
        return res;
    }
    SampledSpectrum SampledSpectrum::operator/(const SampledSpectrum& ss) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] /= ss[i];
        }
        
        return res;
    }
    SampledSpectrum SampledSpectrum::operator*(const float f) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] *= f;
        }
        
        return res;
    }
    SampledSpectrum SampledSpectrum::operator/(const float f) const
    {
        SampledSpectrum res = *this;

        for(std::size_t i{0}; i < 4; ++i)
        {
           res[i] /= f;
        }
        
        return res;
    }
    
    SampledSpectrum& SampledSpectrum::operator+=(const SampledSpectrum& ss)
    {

        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] += ss[i];
        }
        
        return *this;
    }
    SampledSpectrum& SampledSpectrum::operator-=(const SampledSpectrum& ss)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] -= ss[i];
        }
        
        return *this;
    }
    SampledSpectrum& SampledSpectrum::operator*=(const SampledSpectrum& ss)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] *= ss[i];
        }
        
        return *this;
    }
    SampledSpectrum& SampledSpectrum::operator/=(const SampledSpectrum& ss)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] /= ss[i];
        }
        
        return *this;
    }
    SampledSpectrum& SampledSpectrum::operator*=(const float f)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] *= f;
        }
        
        return *this;
    }
    SampledSpectrum& SampledSpectrum::operator/=(const float f)
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
           values[i] /= f;
        }
        
        return *this;
    }
    
    bool SampledSpectrum::hasNaNs() const
    {
        for(std::size_t i{0}; i < 4; ++i)
        {
            if(std::isnan(values[i]))
                return true;
        }
        return false;
    }

    SampledWavelengths SampledWavelengths::sampleUniform(float u, float lambdaMin, float lambdaMax)
    {
        SampledWavelengths swl;
        swl.lambda[0] = ::lerp(u, lambdaMin, lambdaMax);
        float delta = (lambdaMax - lambdaMin) / 4;

        for(std::size_t i{1}; i < 4; ++i)
        {
            swl.lambda[i] = swl.lambda[i - 1] + delta;
            if(swl.lambda[i] > lambdaMax)
                swl.lambda[i] = lambdaMin + (swl.lambda[i] - lambdaMax); //< circular
        }
        for(std::size_t i{0}; i < 4; ++i)
            swl.pdf[i] = 1.f/(lambdaMax - lambdaMin);

        return swl;
    }

    float  SampledWavelengths::operator[](const std::size_t i) const
    {
        return lambda[i];
    }
    float& SampledWavelengths::operator[](const std::size_t i)
    {
        return lambda[i];
    }
    SampledSpectrum SampledWavelengths::PDF() const
    {
        return SampledSpectrum(pdf);
    }
    void SampledWavelengths::terminateSecundary()
    {
        if(secondaryTerminated())
            return;

        for(std::size_t i{1}; i < 4; ++i)
            pdf[i] = 0.f;

        pdf[0] /= 4.f;
    }
    bool SampledWavelengths::secondaryTerminated() const 
    {
        for (std::size_t i{1}; i < 4; ++i)
            if (pdf[i] != 0.f)
                return false;

        return true;
    }

    SampledSpectrum Spectrum::sample(const SampledWavelengths& lambda) const
    {
        SampledSpectrum ss;

        for(std::size_t i{0}; i < 4; ++i)
            ss[i] = (*this)(lambda[i]);

        return ss;
    }
}