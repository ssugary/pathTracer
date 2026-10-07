#include "SamplerIntegrator.hpp"
#include "Sampler/Sampler.hpp"
#include "Utils/common.hpp"
#include <algorithm>
#include <iostream>
#include <thread>

namespace Itg 
{

    void SamplerIntegrator::render(const Scene& scene)
    {
        if (!camera || !camera->film || !sampler) {
        std::cerr << "[Erro Render] Camera, Film ou Sampler não foram inicializados!\n";
        return;
    }

        preprocess(scene);

        Bounds2i bounds = camera->film->getSampleBounds();

        auto w = bounds.pMax.x;
        auto h = bounds.pMax.y;
        unsigned int nThreads = std::min((unsigned int)std::max(1, h - bounds.pMin.y), 
                                   std::max(1u, std::thread::hardware_concurrency()));
        std::vector<std::thread> workers;

        auto renderRows = [&](int rowStart, int rowEnd, int threadSeed)
        {
            std::unique_ptr<Sam::Sampler> localSampler = sampler->clone(threadSeed);

            for(int j = rowStart; j < rowEnd; ++j)
                for(int i = bounds.pMin.x; i < w; ++i)
                {
                    localSampler->startPixel(Point2i(i, j));
                    do
                    {
                        Point2 jitter = localSampler->get2D();
                        float px = i + jitter.x;
                        float py = j + jitter.y;
                        RayDifferential ray = camera->generateRayDifferential(px, py);

                        auto u = float(i) / float(w);
                        auto v = 1.0f - float(j) / float(h);

                        auto tempL = li(ray, scene, *localSampler);
                        Color L = tempL.has_value() ? tempL.value() : scene.background->sample(::normalize(ray.d), u, v);  

                        camera->film->addSample(Point2(i, j), L);
                    }
                    while(localSampler->startNextSample());
                }
        };

        int rowsPerThread = std::max(1, (h - bounds.pMin.y) / (int)nThreads);
        int row = bounds.pMin.y;
        for(unsigned int t = 0; t < nThreads; ++t)
        {
            int rowEnd = (t == nThreads - 1) ? h : std::min(h, row + rowsPerThread);
            workers.emplace_back(renderRows, row, rowEnd, (int)t);
            row = rowEnd;
        }
        
        for(auto& w : workers) 
            w.join();
        // for(int j{bounds.pMin.y}; j < h; ++j)
        // {
        //     for(int i{bounds.pMin.x}; i < w; ++i)
        //     {

        //         sampler->startPixel(Point2i(i, j));

        //         do 
        //         {
        //             Point2 jitter = sampler->get2D();

        //             float px = i  + jitter.x;
        //             float py = j + jitter.y;

        //             Ray ray = camera->generateRay(px, py);
                    
        //             auto tempL = li(ray, scene, *sampler);
        //             auto u = float(i) / float(w);
        //             auto v = 1.0f - float(j) / float(h);

        //             Color L = (tempL.has_value()) ?  tempL.value() : scene.background->sample(normalize(ray.d), u, v);


        //             camera->film->addSample(Point2(i, j), L);

        //         }
        //         while(sampler->startNextSample());


        //     }
        // }
        camera->film->writeImage();


    };

}; 