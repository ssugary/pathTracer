#pragma once

#include <Utils/common.hpp>
#ifndef OTHER_PECTRUM_HPP
#define OTHER_PECTRUM_HPP

#include "Spectrum.hpp"

#include <vector>

namespace ssrt 
{
    class ConstSpectrum : public Spectrum
    {
        private:
            float c;
        public:
            ConstSpectrum(float c);
            float operator()(float) const override;
    };


    class DenselySampledSpectrum : public Spectrum
    {
        private:
            int lambdaMin;
            int lambdaMax;
            std::vector<float> values;
        public:

            DenselySampledSpectrum(Spectrum* spec, int lambdaMin = 360, int lambdaMax = 830);
            float operator()(float lambda) const override;
            
    };

    class PiecewiseSpectrum : public Spectrum 
    {   
        private:
            std::vector<float> lambdas;
            std::vector<float> values;
        public:

            PiecewiseSpectrum(std::span<float> lambdas, std::span<float> values);
            float operator()(float lambda) const override;

    };

    class BlackbodySpectrum : public Spectrum
    {
        private:

            float t;
            float normalizationFactor;

        public:

            BlackbodySpectrum(float t);
            float operator()(float lambda) const override;
    };

    class RGBAlbedoSpectrum : public Spectrum
    {
        private:

            float c0;
            float c1;
            float c2;

        public:

            RGBAlbedoSpectrum(const Color& rgb);
            RGBAlbedoSpectrum(float r, float g, float b);
            float operator()(float lambda) const override;
    };  

    class RGBIlluminationSpectrum : public Spectrum 
    {
        private:

            float c0;
            float c1;
            float c2;

        public:

            RGBIlluminationSpectrum(const Color& rgb);
            RGBIlluminationSpectrum(float r, float g, float b);
            float operator()(float lambda) const override;
    };
};


#endif //< OTHER_PECTRUM_HPP