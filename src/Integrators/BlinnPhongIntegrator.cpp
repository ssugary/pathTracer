#include "Integrators/BlinnPhongIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/VisibilityTester.hpp"
#include <Utils/common.hpp>
#include <optional>


namespace Itg 
{
    std::optional<SampledSpectrum> BlinnPhongIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler, ssrt::SampledWavelengths lambdas) const {
        SampledSpectrum L(0.f);
        SampledSpectrum km(0.f);
        Ray r = ray;
        bool found{false};

        for(int i{0}; i < maxDepth; ++i)
        {
            SurfaceInteraction isect;

            if(!scene.intersect(r, &isect))
                break;
            
            found = true;
            isect.n.normalize();

            if (dot(r.d, isect.n) > 0.f) 
                isect.n = -isect.n;
            
            std::shared_ptr<Mat::BlinnPhongMaterial> fm{nullptr};

            fm = std::dynamic_pointer_cast<Mat::BlinnPhongMaterial>( isect.primitive->getMaterial());
            if(!fm)
                break;

            SampledSpectrum ka(0.f);
            if (fm->ka()) 
                ka = fm->ka()->sample(lambdas);

            km = SampledSpectrum(0.f);
            if (fm->km()) 
                km = fm->km()->sample(lambdas);

            auto n = isect.n;
            auto v = normalize(-r.d);

            for(auto& light : scene.lights)
            {
                Luz::VisibilityTester vis;
                Vec3 wi;
                float pdf;
                Point2 u = sampler.get2D();
                auto Li =  light->sampleLi(isect, u, lambdas, &wi, &pdf, &vis);
                
                if(light->flag == Luz::LightFlag::AMBIENT)
                {
                    L = L + ka * Li;
                    continue;
                }

                if(!vis.unoccluded(scene))
                    continue;
                
        
                float cosThetaI = std::max(0.f, dot(n, wi));
                


                L = L + Li * fm->f(v, wi, n, lambdas) * cosThetaI / pdf;
            }

        
            if(bool(km))
            {
                Vec3 rd = normalize(-v + n * 2 * dot(n, v) );
                r = isect.spawnRay(rd);                
            }
            else 
                break;
        }

        if (L.hasNaNs())
            L = SampledSpectrum(0.f);

        return found ? std::optional(L) : std::nullopt;
    }

};