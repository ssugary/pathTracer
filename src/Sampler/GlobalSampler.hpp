#pragma once

#ifndef GLOBAL_SAMPLER_HPP
#define GLOBAL_SAMPLER_HPP

#include "Sampler.hpp"
#include "SamplerUtils.hpp"
#include "Utils/RNG.hpp"

namespace Sam 
{
    class GlobalSampler : public Sampler
    {
        protected:

            int dimension;
            int64_t intervalSampleIndex;
            const int arrayStartDim{5};
            int arrayEndDim;

        public:
            GlobalSampler(int64_t samplesPerPixel)
            : Sampler(samplesPerPixel) {};

            bool startNextSample() override;
            bool setSampleNumber(int64_t sampleNum) override;
            float get1D() override;
            Point2 get2D() override;
            virtual void startPixel(const Point2i& p) override;
            virtual int64_t getIndexForSample(int64_t sampleNum) const = 0;
            virtual float sampleDimension(int64_t index, int dimension) const = 0;
            virtual std::unique_ptr<Sampler> clone(int seed) override = 0;


    };

};


#endif //< GLOBAL_SAMPLER_HPP