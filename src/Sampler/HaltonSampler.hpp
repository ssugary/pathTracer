#pragma once 

#ifndef HALTON_SAMPLER_HPP
#define HALTON_SAMPLER_HPP

#include "GlobalSampler.hpp"

namespace Sam 
{
    static constexpr int K_MAX_RESOLUTION = 128;

    class HaltonSampler : public GlobalSampler
    {
        private: 
            static inline std::vector<uint16_t> radicalInversePermutations;
            Point2i baseScales;
            Point2i baseExponents;
            int sampleStride;
            int multInverse[2];
            mutable Point2i pixelForOffset = Point2i(std::numeric_limits<int>::max(),std::numeric_limits<int>::max());
            mutable int64_t offsetForCurrentPixel;

            const uint16_t* permutationForDimension(int dim) const 
            {
                if (dim >= PRIME_TABLE_SIZE)
                    return 0;
                return &radicalInversePermutations[PRIME_SUMS[dim]];
            }
        public:

            HaltonSampler(int64_t samplesPerPixel, const Bounds2i& sampleBounds)
            : GlobalSampler(samplesPerPixel)
            {
                if (radicalInversePermutations.size() == 0) 
                {
                    RNG rng;
                    radicalInversePermutations = computeRadicalInversePermutations(rng);
                }

                Vec2i res = sampleBounds.pMax - sampleBounds.pMin;
                for (int i{0}; i < 2; ++i) 
                {
                    int base = (i == 0) ? 2 : 3;
                    int scale = 1, exp = 0;
                    while (scale < std::min(res[i], K_MAX_RESOLUTION)) 
                    {
                        scale *= base;
                        ++exp;
                    }
                    baseScales[i] = scale;
                    baseExponents[i] = exp;
                }

                sampleStride = baseScales[0] * baseScales[1];
                multInverse[0] = multiplicativeInverse(baseScales[1], baseScales[0]);
                multInverse[1] = multiplicativeInverse(baseScales[0], baseScales[1]);
            }

            int64_t getIndexForSample(int64_t sampleNum) const override
            {
                if (currentPix != pixelForOffset) 
                {
                    offsetForCurrentPixel = 0;
                    if (sampleStride > 1) 
                    {
                        Point2i pm(currentPix[0] % K_MAX_RESOLUTION,currentPix[1] % K_MAX_RESOLUTION);
                        for (int i{0}; i < 2; ++i) 
                        {
                            uint64_t dimOffset = (i == 0) ? inverseRadicalInverse<2>(pm[i], baseExponents[i]) : inverseRadicalInverse<3>(pm[i], baseExponents[i]);
                            offsetForCurrentPixel += dimOffset * (sampleStride / baseScales[i]) * multInverse[i];
                        }
                        offsetForCurrentPixel %= sampleStride;
                    }
                    pixelForOffset = currentPix;
                }
                return offsetForCurrentPixel + sampleNum * sampleStride;
            }

            float sampleDimension(int64_t index, int dim) const override
            {
                if (dim == 0)
                    return radicalInverse(2, index >> baseExponents[0]);
                else if (dim == 1)
                    return radicalInverse(3, index / baseScales[1]);
                else
                {
                    if (dim >= PRIME_TABLE_SIZE) 
                        return radicalInverse(2, index); 
        
                    
                    return scrambledRadicalInverse(PRIMES[dim], index,permutationForDimension(dim));
                }
            }

            std::unique_ptr<Sampler> clone(int) override 
            {
                return std::make_unique<HaltonSampler>(*this);
            }
    };
}

#endif //< HALTON_SAMPLER_HPP