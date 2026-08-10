#include "PixelSampler.hpp"

namespace Sam 
{
    void PixelSampler::startPixel(const Point2i& p) 
    {
        for (std::size_t i = 0; i < samples1D.size(); ++i) {
            for (std::size_t j = 0; j < samples1D[i].size(); ++j) {
                samples1D[i][j] = rng.uniformFloat();
            }
        }
        for (std::size_t i = 0; i < samples2D.size(); ++i) {
            for (std::size_t j = 0; j < samples2D[i].size(); ++j) {
                samples2D[i][j] = Point2(rng.uniformFloat(), rng.uniformFloat());
            }
        }
        Sampler::startPixel(p);
    }

    bool PixelSampler::startNextSample()
    {
        current1DDimension = 0;
        current2DDimension = 0;

        return Sampler::startNextSample();
    }

    bool PixelSampler::setSampleNumber(int32_t sampleNum)
    {
        current1DDimension = 0;
        current2DDimension = 0;

        return Sampler::setSampleNumber(sampleNum);
    }
    float PixelSampler::get1D()
    {
        if (current1DDimension < samples1D.size() && 
            !samples1D[current1DDimension].empty() && 
            static_cast<float>(currentPixIndex) < samples1D[current1DDimension].size()) 
        {
            return samples1D[current1DDimension++][currentPixIndex];
        }
        return rng.uniformFloat();
    }
    Point2 PixelSampler::get2D()
    {
        if (current2DDimension < samples2D.size() && 
        !samples2D[current2DDimension].empty() && 
        static_cast<float>(currentPixIndex) < samples2D[current2DDimension].size()) 
        {
            return samples2D[current2DDimension++][currentPixIndex];
        }
        return Point2(rng.uniformFloat(), rng.uniformFloat());
    }

    std::unique_ptr<Sampler> PixelSampler::clone(int seed)
    {
        PixelSampler *clone = new PixelSampler(*this);
    
        clone->rng.setSequence(seed); 
        
        return std::unique_ptr<Sampler>(clone);
    }
};