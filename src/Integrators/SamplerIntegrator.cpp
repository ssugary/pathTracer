#include "SamplerIntegrator.hpp"
#include "Sampler/Sampler.hpp"
#include "Utils/common.hpp"
#include <algorithm>
#include <iostream>

namespace Itg 
{

    void SamplerIntegrator::render(const Scene& scene)
    {
        if (!camera || !camera->film || !sampler) {
        std::cerr << "[Erro Render] Camera, Film ou Sampler não foram inicializados!\n";
        return;
    }

        // preprocess(scene);

        Bounds2i bounds = camera->film->getSampleBounds();

        auto w = bounds.pMax.x;
        auto h = bounds.pMax.y;
        
        for(int j{bounds.pMin.y}; j < h; ++j)
        {
            for(int i{bounds.pMin.x}; i < w; ++i)
            {

                sampler->startPixel(Point2i(i, j));

                do 
                {
                    Point2 jitter = sampler->get2D();

                    float px = i  + jitter.x;
                    float py = j + jitter.y;

                    Ray ray = camera->generateRay(px, py);
                    
                    auto tempL = li(ray, scene, *sampler);
                    auto u = float(i) / float(w);
                    auto v = 1.0f - float(j) / float(h);

                    Color L = (tempL.has_value()) ?  tempL.value() : scene.background->sample(normalize(ray.d), u, v);


                    camera->film->addSample(Point2(i, j), L);

                }
                while(sampler->startNextSample());


            }
        }
        camera->film->writeImage();


    };

}; 