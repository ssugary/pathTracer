#include "SamplerIntegrator.hpp"
#include "Utils/Spectrum/XYZ.hpp"
#include <iostream>

namespace Itg 
{

    void SamplerIntegrator::render(const Scene& scene)
    {
        if (!camera || !camera->film || !sampler) 
        {
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

                    float uLambda = sampler->get1D();
                    ssrt::SampledWavelengths lambdas = ssrt::SampledWavelengths::sampleUniform(uLambda, 360.f, 830.f);
                    
                    auto tempL = li(ray, scene, *sampler, lambdas);

                    Color finalColor{0.f};
                    
                    if (tempL.has_value()) 
                    {
                        SampledSpectrum L = tempL.value();
                        SampledSpectrum pdf = lambdas.PDF(); 

                        float X{0.f};
                        float Y{0.f};
                        float Z{0.f};

                        for (int w = 0; w < 4; ++w) 
                            if (pdf[w] > 0.f) 
                            {
                                float x_val = ssrt::cieX(lambdas[w]); 
                                float y_val = ssrt::cieY(lambdas[w]);
                                float z_val = ssrt::cieZ(lambdas[w]);

                                X += L[w] * x_val / pdf[w];
                                Y += L[w] * y_val / pdf[w];
                                Z += L[w] * z_val / pdf[w];
                            }
                        
                        
                        X /= (ssrt::CIE_Y_INTEGRAL * 4.f); 
                        Y /= (ssrt::CIE_Y_INTEGRAL * 4.f); 
                        Z /= (ssrt::CIE_Y_INTEGRAL * 4.f);

                        finalColor = ssrt::XYZToRGB(ssrt::XYZ(X, Y, Z)); 

                        finalColor.r = std::max(0.f, finalColor.r);
                        finalColor.g = std::max(0.f, finalColor.g);
                        finalColor.b = std::max(0.f, finalColor.b);
                    }
                    else 
                    {
                        auto u = float(i) / float(w);
                        auto v = 1.0f - float(j) / float(h);
                        finalColor = scene.background->sample(::normalize(ray.d), u, v);
                    }   

                    camera->film->addSample(Point2(i, j), finalColor);


                }
                while(sampler->startNextSample());


            }
        }
        camera->film->writeImage();


    };

}; 