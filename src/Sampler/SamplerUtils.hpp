#ifndef SAMPLER_UTILS_HPP
#define SAMPLER_UTILS_HPP

#include <Utils/common.hpp>
#include <Utils/RNG.hpp>
#include <algorithm>

namespace Sam
 {
   
  template<typename T>
  void shuffle(T* samp, int count, int nD, RNG& rng)
  {
    for(int i{0}; i < count; ++i)
    {
      int other = i + rng.uniformUInt32(count - i);
      for(int j{0}; j < nD; ++j)
      {
        std::swap(samp[nD * i + j], samp[nD * other + j]);
      }
    }
  }
  
  void stratifiedSample1D(float* samp, int nSamples, RNG& rng, bool jitter);
  void stratifiedSample2D(Point2* samp, int nx, int ny, RNG& rng, bool jitter);
  void latinHypercube(float* samples, int nSamples, int nDimensions, RNG& rng);

 } // namespace Sam

#endif // SAMPLER_UTILS_HPP
