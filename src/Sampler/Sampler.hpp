#pragma once

#ifndef SAMPLER_HPP 
#define SAMPLER_HPP

#include "Utils/common.hpp"
#include <memory>
#include <sys/types.h>
#include <vector>

namespace Sam 
{
    class Sampler 
    {
        private:
            
            size_t arrayOffset1D{0};
            size_t arrayOffset2D{0};

        protected:
        
            Point2i currentPix{0, 0};
            int64_t currentPixIndex{0};
            std::vector<int> samplesArraySizes1D;
            std::vector<int> samplesArraySizes2D;
            std::vector<std::vector<float>> sampleArray1D;
            std::vector<std::vector<Point2>> sampleArray2D;

        public:

            int64_t samplesPerPixel{0};

            virtual ~Sampler() = default;
            virtual int RoundCount(int n) const 
            {
                return n;
            }
            Sampler(int64_t samplesPerPixel) 
            : samplesPerPixel(samplesPerPixel) {};
            
            virtual void startPixel(const Point2i&);
            virtual float get1D() = 0;
            virtual Point2 get2D() = 0;
            virtual bool startNextSample();
            virtual bool setSampleNumber(int64_t sampleNum);
            virtual std::unique_ptr<Sampler> clone(int seed) = 0;
            
            CameraSample getCameraSample(const Point2i&);
            void request1DArray(int n);
            void request2DArray(int n);
            const float* get1DArray(int n);
            const Point2* get2DArray(int n);
    };
};

#endif //< SAMPLER_HPP