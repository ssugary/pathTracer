#include "SamplerUtils.hpp"

namespace Sam 
{
  

  void stratifiedSample1D(float* samp, int nSamples, RNG& rng, bool jitter)
  {
    float invS = 1.f/static_cast<float>(nSamples);
    for(int i{0}; i < nSamples; ++i)
    {
      float delta = jitter ? rng.uniformFloat() : 0.5f;
      samp[i] = std::min((i + delta) * invS, ONE_MINUS_EPSILON);
    }
  }

  void stratifiedSample2D(Point2* samp, int nx, int ny, RNG& rng, bool jitter)
  {
  
    float dx = 1.f/static_cast<float>(nx);
    float dy = 1.f/static_cast<float>(ny);
    
    for(int j{0}; j < ny; ++j)
    {
      for(int i{0}; i < nx; ++i)
      {
        float jx = jitter ? rng.uniformFloat() : 0.5f;
        float jy = jitter ? rng.uniformFloat() : 0.5f;
        samp->x = std::min((i + jx) * dx, ONE_MINUS_EPSILON);
        samp->y = std::min((j + jy) * dy, ONE_MINUS_EPSILON);
        ++samp;
      }
    }

  }

  void latinHypercube(float* samples, int nSamples, int nDimensions, RNG& rng)
  {
    float invNSamples = 1.f/static_cast<float>(nSamples);
    
    for(int i{0}; i < nSamples; ++i)
    {
      for(int j{0}; j < nDimensions; ++j)
      {
        float sj = (static_cast<float>(i) + rng.uniformFloat()) * invNSamples;
        samples[nDimensions * i + j] = std::min(sj, ONE_MINUS_EPSILON);
      }
    }

    for(int i{0}; i < nDimensions; ++i)
    {
        for(int j{0}; j < nSamples; ++j)
        {
            int other = (j + rng.uniformUInt32(nSamples - j));
            std::swap(samples[nDimensions * j + i], samples[nDimensions * other + i]);
        }
    }
  }
  std::vector<uint16_t> computeRadicalInversePermutations(RNG& rng)
  {
      std::vector<uint16_t> perms;
      int permArraySize = 0;
      
      for (int i{0}; i < PRIME_TABLE_SIZE; ++i)
          permArraySize += PRIMES[i];

      perms.resize(permArraySize);
      uint16_t *p = &perms[0];

      for (int i{0}; i < PRIME_TABLE_SIZE; ++i) 
      {
            for (int j{0}; j < PRIMES[i]; ++j)
                p[j] = j;
            shuffle(p, PRIMES[i], 1, rng);

          p += PRIMES[i];
      }
      return perms;
  }

  float radicalInverse(int base, uint64_t a) 
  {
      float digitSum{0.0};
      float invBase = 1.0 / base;
      float invBaseM{invBase};
      
      while (a > 0) 
      {
          uint64_t next = a / base;
          uint64_t digit = a - next * base;
          digitSum += digit * invBaseM;    
          invBaseM *= invBase;            
          a = next;
      }

      return digitSum;
  }

  float scrambledRadicalInverse(int base, uint64_t a, const uint16_t *perm) 
  {
    float digitSum{0.0};
    float invBase = 1.0 / base;
    float invBaseM{invBase};
    
    while (a > 0) 
    {
        uint64_t next = a / base;
        uint64_t digit = a - next * base;
        
        digitSum += perm[digit] * invBaseM;
        
        invBaseM *= invBase;
        a = next;
    }
    return digitSum;
  }
}
