#pragma once

#ifndef STRATIFIED_SAMPLER_HPP
#define STRATIFIED_SAMPLER_HPP

#include "PixelSampler.hpp"

namespace Sam 
{
    class StratifiedSampler : public PixelSampler
    {
        private:
            int xPixSamples;
            int yPixSamples;
            bool jitterSamples;

        public:

            StratifiedSampler(int xPixSamples, int yPixSamples, bool jitterSamples, int nSampledDimensions)
            : PixelSampler(xPixSamples * yPixSamples, nSampledDimensions), xPixSamples(xPixSamples), yPixSamples(yPixSamples), jitterSamples(jitterSamples) {};

            void startPixel(const Point2i&) override;
            std::unique_ptr<Sampler> clone(int seed) override;


    };
}; //< namespace Sam


#endif //< STRATIFIED_SAMPLER_HPP