#include "StratifiedSampler.hpp"
#include "SamplerUtils.hpp"
#include <cstdint>

namespace Sam 
{

  void StratifiedSampler::startPixel(const Point2i& p )
  {
    int ns = xPixSamples * yPixSamples;
    for(std::size_t i{0}; i < samples1D.size(); ++i)
    {
      stratifiedSample1D(&samples1D[i][0], ns, rng, jitterSamples);
      shuffle(&samples1D[i][0], ns, 1, rng);
    }

    for(std::size_t i{0}; i < samples2D.size(); ++i)
    {
      stratifiedSample2D(&samples2D[i][0], xPixSamples, yPixSamples, rng, jitterSamples);
      shuffle(&samples2D[i][0], ns, 1, rng);
    }

    for(std::size_t i{0}; i < samplesArraySizes1D.size(); ++i)
    {
      for(int64_t j{0}; j < samplesPerPixel; ++j)
      {
        int count = samplesArraySizes1D[i];
        stratifiedSample1D(&sampleArray1D[i][j * count], count, rng, jitterSamples);
        shuffle(&sampleArray1D[i][j * count], count, 1, rng);
      }
    }

    for(std::size_t i{0}; i < samplesArraySizes2D.size(); ++i)
    {
      for(int64_t j{0}; j < samplesPerPixel; ++j)
      {
        int count = samplesArraySizes2D[i];
        latinHypercube(&sampleArray2D[i][j * count][0], count, 2, rng);

      }
    }

    PixelSampler::startPixel(p);
  }


  std::unique_ptr<Sampler> StratifiedSampler::clone(int seed)
  {
      StratifiedSampler *clone = new StratifiedSampler(*this);
      clone->rng.setSequence(seed); 
      return std::unique_ptr<Sampler>(clone);
  }
  
};
