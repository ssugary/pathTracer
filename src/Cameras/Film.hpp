#pragma once

#ifndef FILM_HPP
#define FILM_HPP

#include "ImageIO.hpp"
#include "Filter/Filter.hpp"

#include <cmath>
#include <memory>

namespace Cam 
{
    class Film 
    {
      private:
        
        std::vector<Pixel> buffer;

        // static constexpr int filterTableW = 16;
        // float filterTable[filterTableW * filterTableW];
        // std::mutex mutex;
        // const float scale;

      public:
        
          const Point2i fullRes;
          // const float diagonal;
          std::unique_ptr<Fil::Filter> filter;
          const std::string filename;
          Bounds2i croppedPixelBounds;

          bool gammaC{false}; 

          ~Film() = default;
          Film(const Point2i& res, const Bounds2i& cropWindow, 
               std::unique_ptr<Fil::Filter> f, /*float diagonal*/
               const std::string& filename /*float scale*/)
          : /*scale(scale)*/ fullRes(res), /*diagonal(diagonal * 0.001)*/ filter(std::move(f)), filename(filename)
          {
            croppedPixelBounds = Bounds2i( 
                                          Point2i(std::ceil(fullRes[0] * cropWindow.pMin[0]), std::ceil(fullRes[1] * cropWindow.pMin[1])),
                                          Point2i(std::ceil(fullRes[0] * cropWindow.pMax[0]), std::ceil(fullRes[1] * cropWindow.pMax[1]))
                                        );
          
            buffer.resize(croppedPixelBounds.area());
                        
          }

          Bounds2i getSampleBounds() const 
          {
            Bounds2f b = Bounds2f(
                                  floor(Point2(croppedPixelBounds.pMin[0], croppedPixelBounds.pMin[1]) + Vec2(0.5, 0.5)), 
                                  ceil(Point2(croppedPixelBounds.pMax[0], croppedPixelBounds.pMax[1]) - Vec2(0.5, 0.5))
                                  );
            Bounds2i bi;
            bi.pMin = {static_cast<int>(b.pMin[0]), static_cast<int>(b.pMin[1])};
            bi.pMax = {static_cast<int>(b.pMax[0]), static_cast<int>(b.pMax[1])};
      
            return bi;
          }

          // Bounds2f getExtends() const 
          // {
          //   float aspect = static_cast<float>(fullRes[1]) / static_cast<float>(fullRes[0]);
          //   float x = std::sqrt(diagonal * diagonal / (1 + aspect * aspect));
          //   float y = aspect * x;
          //   return Bounds2f(Point2(-x / 2, -y / 2), Point2(x / 2, y / 2));
          // }

          Pixel &getPixel(const Point2i p)
          {
            int w = croppedPixelBounds.pMax[0] - croppedPixelBounds.pMin[0];
            int offset = (p[0] - croppedPixelBounds.pMin[0]) + (p[1] - croppedPixelBounds.pMin[1]) * w;
            return buffer[offset]; 
          }
        
          void addSample(const Point2& p, const Color& color, float sampleW=1.0)
          {
            Point2 pFilmDiscrete = p - Vec2(0.5f, 0.5f);
            Point2i p0 = static_cast<Point2i>(ceil(pFilmDiscrete - filter->radius));
            Point2i p1 = static_cast<Point2i>(floor(pFilmDiscrete + filter->radius));
            p0 = max(p0, croppedPixelBounds.pMin);
            p1 = min(p1, croppedPixelBounds.pMax - Vec2i(1, 1));


            for (int j = p0.y; j <= p1.y; ++j) 
            {
              for (int i = p0.x; i <= p1.x; ++i) 
              {
                  Point2 pPixel = Point2(i + 0.5f, j + 0.5f);
                  Vec2 offset = p - pPixel;

                  float w = filter->evaluate(static_cast<Point2>(offset));

                  Pixel& pixel = getPixel({i, j});
                  pixel.colorSum += color * w * sampleW;
                  pixel.filterWeightSum += w;
              }
          }
              
          }

          void writeImage()
          {
            std::vector<Color> colorBuffer;
            colorBuffer.reserve(fullRes[0] * fullRes[1]);
            

            for(int j{0}; j < fullRes[1]; ++j)
            {
              for(int i{0}; i < fullRes[0]; ++i)
              {
                Pixel& p = getPixel(Point2i{i, j});
                Color finalColor = p.colorSum;
              
                if (p.filterWeightSum > 0.f) 
                    finalColor = finalColor / p.filterWeightSum;
                
                
                finalColor.r = std::max(0.f, finalColor.r);
                finalColor.g = std::max(0.f, finalColor.g);
                finalColor.b = std::max(0.f, finalColor.b);

                // float maxColor = std::max({finalColor.r, finalColor.g, finalColor.b});
                // float scale = 1.0f / (maxColor + 1.0f);

                // finalColor.r *= scale;
                // finalColor.g *= scale;
                // finalColor.b *= scale;
                
                colorBuffer.push_back(finalColor);
              }
            }

            ImageIO::write(filename, fullRes[0], fullRes[1], colorBuffer, gammaC);

        }
   };
}

#endif
