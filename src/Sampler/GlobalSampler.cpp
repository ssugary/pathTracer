#include "GlobalSampler.hpp"
#include <memory>
namespace Sam 
{

    void GlobalSampler::startPixel(const Point2i& p)
    {
        Sampler::startPixel(p);
        dimension = 0;
        intervalSampleIndex = getIndexForSample(0);
        arrayEndDim = arrayStartDim + sampleArray1D.size() + 2 * sampleArray2D.size();

        for (std::size_t i{0}; i < samplesArraySizes1D.size(); ++i) 
        {
           int nSamples = samplesArraySizes1D[i] * samplesPerPixel;
           for (int j{0}; j < nSamples; ++j) 
           {
               int64_t index = getIndexForSample(j);
               sampleArray1D[i][j] = sampleDimension(index, arrayStartDim + i);
           }
       }

       int dim = arrayStartDim + samplesArraySizes1D.size();

       for (std::size_t i{0}; i < samplesArraySizes2D.size(); ++i) 
       {
           int nSamples = samplesArraySizes2D[i] * samplesPerPixel;

           for (int j{0}; j < nSamples; ++j) 
           {
               int64_t idx = getIndexForSample(j);
               sampleArray2D[i][j].x = sampleDimension(idx, dim);
               sampleArray2D[i][j].y = sampleDimension(idx, dim+1);
           }

           dim += 2;
       }
       assert(dim == arrayEndDim);
    }

    bool GlobalSampler::startNextSample()
    {
        dimension = 0;
        intervalSampleIndex = getIndexForSample(currentPixIndex + 1);
        return Sampler::startNextSample();
    }

    bool GlobalSampler::setSampleNumber(int64_t sample) 
    {
        dimension = 0;
        intervalSampleIndex = getIndexForSample(sample);
        return Sampler::setSampleNumber(sample);
    }

    float GlobalSampler::get1D()
    {
        if(dimension >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        return sampleDimension(intervalSampleIndex, dimension++);
    }
    Point2 GlobalSampler::get2D() 
    {
        if (dimension + 1 >= arrayStartDim && dimension < arrayEndDim)
            dimension = arrayEndDim;
        Point2 p(sampleDimension(intervalSampleIndex, dimension),
                 sampleDimension(intervalSampleIndex, dimension + 1));
        dimension += 2;
        return p;
    }
    

};