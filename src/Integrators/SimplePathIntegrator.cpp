#include "SimplePathIntegrator.hpp"

#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/AreaLight.hpp"
#include "Light/VisibilityTester.hpp"

namespace Itg 
{
    std::optional<SampledSpectrum> SimplePathIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler, ssrt::SampledWavelengths lambdas) const
    {
        SampledSpectrum L(0.f);
        SampledSpectrum tp(1.f);
        bool found{false};
        Ray r = ray;

        for(int i{0}; i < maxDepth; ++i)
        {
            SurfaceInteraction  isect;

            if(!scene.intersect(r, &isect))
                break;

            found = true;
            isect.n.normalize();

            if(dot(r.d, isect.n) > 0.f)
                isect.n = -isect.n;
            
            Vec3 v = normalize(-r.d);
            auto emission = isect.Le(v, lambdas);

            auto fm = std::dynamic_pointer_cast<Mat::BlinnPhongMaterial>(isect.primitive->getMaterial());
            if (!fm) 
                break;

            if (i == 0 && bool(emission))
            {
                L += tp * emission;    
            }

            for(auto& light : scene.lights)
            {
                auto isectAreaLight = isect.primitive->getAreaLight();
                if (isectAreaLight && isectAreaLight.get() == light.get())
                    continue;

                Luz::VisibilityTester vis;
                Vec3 wi;
                float pdf;
                Point2 u = sampler.get2D();
                auto Li = light->sampleLi(isect, u, lambdas, &wi, &pdf, &vis);

                if (pdf <= SHADOW_EPSILON) 
                    continue;

                if (light->flag == Luz::LightFlag::AMBIENT)
                {
                    if (fm->ka()) 
                        L += tp * fm->ka()->sample(lambdas) * Li;

                    continue;
                }

                if (!vis.unoccluded(scene))    
                    continue;

                float cosThetaI = std::max(0.f, dot(isect.n, wi));

                if (cosThetaI <= EPSILON) 
                    continue;

                
                auto fr = fm->f(v, wi, isect.n, lambdas);

                auto Lt = tp * fr * Li * cosThetaI / pdf;

                
                
                L += Lt;
            }

            Vec3 wi;
            float pdf;
            Point2 u = sampler.get2D();
            auto fr = fm->sampleF(v, isect.n, u, lambdas, &wi, &pdf);
        
            if(pdf <= SHADOW_EPSILON)
                break;

            float cosThetaI = std::max(0.f, dot(isect.n, wi));
            tp = tp * fr * cosThetaI / pdf;
            r = isect.spawnRay(wi);

            if(i > 3)
            {
                float q = std::max(0.05f, 1.f - std::max({tp[0], tp[1], tp[2], tp[3]}));
                if (sampler.get1D() < q) 
                    break;

                tp = tp / (1.f - q);
            }
        
        }
        return found ? std::optional(L) : std::nullopt;
    }
}