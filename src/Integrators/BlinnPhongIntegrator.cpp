#include "Integrators/BlinnPhongIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/VisibilityTester.hpp"
#include <Utils/common.hpp>
#include <optional>


namespace Itg 
{
    std::optional<Color> BlinnPhongIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler) const {
        Color L(0.f, 0.f, 0.f);
        Ray r = ray;
        bool found{false};
        Color km;

        for(int i{0}; i < maxDepth; ++i)
        {
            SurfaceInteraction isect;

            if(!scene.intersect(r, &isect))
            {
                if(found)
                {
                    L = L + km * scene.background->sample(r.d);
                }

                break;
            }
            
            found = true;
            isect.n.normalize();

            if (dot(r.d, isect.n) > 0) 
                isect.n = -isect.n;
            
            std::shared_ptr<Mat::BlinnPhongMaterial> fm{nullptr};

            fm = std::dynamic_pointer_cast<Mat::BlinnPhongMaterial>( isect.primitive->getMaterial());
            if(!fm)
                break;

            Color ka = fm->ka();
            km = fm->km();

            auto n = isect.n;
            auto v = normalize(-r.d);

            for(auto& light : scene.lights)
            {
                Luz::VisibilityTester vis;
                Vec3 wi;
                float pdf;
                Point2 u = sampler.get2D();
                auto Li =  light->sampleLi(isect, u, &wi, &pdf, &vis);
                
                if(light->flag == Luz::LightFlag::AMBIENT)
                {
                    L = L + ka * Li;
                    continue;
                }

                if(!vis.unoccluded(scene))
                    continue;
                
        
                float cosThetaI = std::max(0.f, dot(n, wi));
                if (cosThetaI <= 0.f) 
                    continue;


                L = L + Li * fm->f(v, wi, n) * cosThetaI / pdf;
            }

        
            if(km.r > 0.f || km.g > 0.f || km.b > 0.f)
            {
                Vec3 rd = normalize(-v + n * 2 * dot(n, v) );
                r = isect.spawnRay(rd);                
            }
            else 
                break;
        }
        return found ? std::optional(L) : std::nullopt;
    }

};