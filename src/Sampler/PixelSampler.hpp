#ifndef PIXEL_SAMPLER_HPP
#define PIXEL_SAMPLER_HPP

#include "Sampler.hpp"
#include "Utils/RNG.hpp"

namespace Sam 
{
    class PixelSampler : public Sampler
    {
        protected:

            std::vector<std::vector<float>> samples1D;
            std::vector<std::vector<Point2>> samples2D;
            std::size_t current1DDimension;
            std::size_t current2DDimension;

            RNG rng;

        public:
            PixelSampler(int32_t samplesPerPixel, std::size_t nSampledDimensions)
            : Sampler(samplesPerPixel), current1DDimension(0), current2DDimension(0)
            {
                samples1D.resize(nSampledDimensions, std::vector<float>(samplesPerPixel));
                samples2D.resize(nSampledDimensions, std::vector<Point2>(samplesPerPixel));
            }

            bool startNextSample() override;
            bool setSampleNumber(int32_t sampleNum) override;
            float get1D() override;
            Point2 get2D() override;
            std::unique_ptr<Sampler> clone(int seed) override;



    };

};


#endif //< PIXEL_SAMPLER_HPP