#include "Sampler.hpp"
#include "Utils/common.hpp"


namespace Sam 
{

    void Sampler::startPixel(const Point2i& p) 
    {
        currentPix = p;
        currentPixIndex = 0;
        arrayOffset1D = 0;
        arrayOffset2D = 0;
    }
    
    bool Sampler::startNextSample()
    {
        arrayOffset1D = 0;
        arrayOffset2D = 0;

        return ++currentPixIndex < samplesPerPixel;
    }

    bool Sampler::setSampleNumber(int64_t sampleNum)
    {
        arrayOffset1D = 0;
        arrayOffset2D = 0;

        currentPixIndex = sampleNum;
        return currentPixIndex < samplesPerPixel;
    }
    CameraSample Sampler::getCameraSample(const Point2i& p)
    {
        CameraSample cs;
        Point2 offset = get2D();
        cs.pFilm = Point2(static_cast<float>(p[0] + offset[0]), 
                          static_cast<float>(p[1] + offset[1]));
        cs.time = get1D();
        cs.pLens = get2D(); 

        return cs;
    }

    void Sampler::request1DArray(int n)
    {
        samplesArraySizes1D.push_back(RoundCount(n));
        sampleArray1D.push_back(std::vector<float>(RoundCount(n) * samplesPerPixel));
    }

    void Sampler::request2DArray(int n)
    {
        samplesArraySizes2D.push_back(RoundCount(n));
        sampleArray2D.push_back(std::vector<Point2>(RoundCount(n) * samplesPerPixel));
    }

    const float *Sampler::get1DArray(int n) 
    {
        if (arrayOffset1D == sampleArray1D.size()) 
            return nullptr;
        
        return &sampleArray1D[arrayOffset1D++][currentPixIndex * RoundCount(n)];
    }

    const Point2 *Sampler::get2DArray(int n) 
    {
        if (arrayOffset2D == sampleArray2D.size()) 
            return nullptr;

        return &sampleArray2D[arrayOffset2D++][currentPixIndex * RoundCount(n)];
    }


};