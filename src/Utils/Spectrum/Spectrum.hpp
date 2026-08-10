#pragma once

#include <string>
#ifndef SPECTRUM_HPP
#define SPECTRUM_HPP

#include <array>
#include <cstdlib>
#include <span>

namespace ssrt 
{
    static constexpr float CIE_Y_integral = 106.856895;

    class SampledSpectrum
    {
    
        private:

            std::array<float, 4> values;

        public:

            SampledSpectrum() = default;
            SampledSpectrum(const SampledSpectrum&) = default;
            SampledSpectrum(SampledSpectrum&&) = default;
            ~SampledSpectrum() = default;

            SampledSpectrum(float c);
            SampledSpectrum(std::span<const float> v);
            
            float operator[](const std::size_t i) const;
            float&operator[](const std::size_t i)      ;
            
            explicit operator bool() const;
            bool operator==(const SampledSpectrum& ss)const;
            bool operator!=(const SampledSpectrum& ss)const;
            
            SampledSpectrum& operator=(const SampledSpectrum& ss);
            SampledSpectrum operator-() const;
            
            SampledSpectrum operator+(const SampledSpectrum& ss) const;
            SampledSpectrum operator-(const SampledSpectrum& ss) const;
            SampledSpectrum operator*(const SampledSpectrum& ss) const;
            SampledSpectrum operator/(const SampledSpectrum& ss) const;
            SampledSpectrum operator*(const float f) const;
            SampledSpectrum operator/(const float f) const;
            
            SampledSpectrum& operator+=(const SampledSpectrum& ss);
            SampledSpectrum& operator-=(const SampledSpectrum& ss);
            SampledSpectrum& operator*=(const SampledSpectrum& ss);
            SampledSpectrum& operator/=(const SampledSpectrum& ss);
            SampledSpectrum& operator*=(const float f);
            SampledSpectrum& operator/=(const float f);
            
            bool hasNaNs() const;
            
            friend SampledSpectrum operator* (const float f, const SampledSpectrum& ss)
            {                    
                return ss * f;
            }
            friend SampledSpectrum operator- (const float f, const SampledSpectrum& ss) 
            {
                SampledSpectrum res;

                for(std::size_t i{0}; i < 4; ++i)
                {
                    res[i] = f - ss[i];
                }
                return res;
            }
        };
        
        class SampledWavelengths
        {
            private:

                std::array<float, 4> lambda;
                std::array<float, 4> pdf;

            public:

                static SampledWavelengths sampleUniform(float u, float lambdaMin = 360.f, float lambdaMax = 830.f);
                float operator[](const std::size_t i) const;
                float&operator[](const std::size_t i);
                SampledSpectrum PDF() const;

                bool secondaryTerminated() const;
                void terminateSecundary();
        };

        class Spectrum 
        {
            public:

                virtual float operator()(float lambda) const = 0;
                virtual SampledSpectrum sample(const SampledWavelengths& lambda) const;
        };
    };
    
    using ssrt::Spectrum;
    using ssrt::SampledSpectrum;

#endif //< SPECTRUM_HPP