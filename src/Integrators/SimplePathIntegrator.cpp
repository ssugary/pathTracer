#include "SimplePathIntegrator.hpp"

#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/VisibilityTester.hpp"

namespace Itg 
{
    std::optional<Color> SimplePathIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler) const
    {
        Color L;
        Color tp(1.f);
        bool found{false};
        Ray r = ray;

        for(int i{0}; i < maxDepth; ++i)
        {
            SurfaceInteraction isect;

            if(!scene.intersect(r, &isect))
                break;

            found = true;
            isect.n.normalize();

            if(dot(r.d, isect.n) > 0.f)
                isect.n = -isect.n;

            auto fm = std::dynamic_pointer_cast<Mat::BlinnPhongMaterial>(isect.primitive->getMaterial());
            if (!fm) 
                break;

            Vec3 v = normalize(-r.d);
            
            L += tp * isect.Le(-r.d);

            for(auto& light : scene.lights)
            {
                Luz::VisibilityTester vis;
                Vec3 wi;
                float pdf;
                Point2 u = sampler.get2D();
                Color Li = light->sampleLi(isect, u, &wi, &pdf, &vis);
                if (light->flag == Luz::LightFlag::AMBIENT)
                {
                    L += tp * fm->ka() * Li;
                    continue;
                }

                if (!vis.unoccluded(scene))    
                    continue;

                float cosThetaI = std::max(0.f, dot(isect.n, wi));
                if (cosThetaI <= 0.f) continue;

                Color fr = fm->f(v, wi, isect.n);

                L += tp * fr * Li * cosThetaI / pdf;
            }

            Vec3 wi;
            float pdf;
            Point2 u = sampler.get2D();
            Color fr = fm->sampleF(v, isect.n, u, &wi, &pdf);
        
            if(pdf <= 0.f)
                break;

            float cosThetaI = std::max(0.f, dot(isect.n, wi));
            tp = tp * fr * cosThetaI / pdf;
            r = isect.spawnRay(wi);

            if(i > 3)
            {
                float q = std::max(0.05f, 1.f - std::max({tp.r, tp.g, tp.b}));
                if (sampler.get1D() < q) break;
                tp = tp / (1.f - q);
            }
        
        }
        return found ? std::optional(L) : std::nullopt;
    }
}