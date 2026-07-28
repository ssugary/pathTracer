#include "Integrators/BlinnPhongIntegrator.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/VisibilityTester.hpp"
#include <Utils/common.hpp>
#include <optional>


namespace Itg 
{
    std::optional<Color> BlinnPhongIntegrator::li(const Ray& ray, const Scene& scene, Sam::Sampler& sampler ,int depth) const {
        Color L(0.f, 0.f, 0.f);

        SurfaceInteraction isect;

        if(!scene.intersect(ray, &isect))
            return std::nullopt;
        
        isect.n.normalize();

        if (dot(ray.d, isect.n) > 0) 
           isect.n = -isect.n;
        

        std::shared_ptr<Mat::BlinnPhongMaterial> fm{nullptr};

        fm = std::dynamic_pointer_cast<Mat::BlinnPhongMaterial>( isect.primitive->getMaterial());
        if(!fm)
            return std::nullopt;

        Color ka = fm->ka();
        Color kd = fm->kd();
        Color ks = fm->ks();
        Color km = fm->km();
        float gg = fm->gg();

        auto n = isect.n;
        auto v = normalize(-ray.d);

        for(auto& light : scene.lights)
        {
            Luz::VisibilityTester vis;
            Vec3 wi;
            auto Li =  light->sampleLi(isect, &wi, &vis);
            
            if(light->flag == Luz::LightFlag::AMBIENT)
            {
                L = L + ka * Li;
                continue;
            }

            if(!vis.unoccluded(scene))
                continue;
            
            
            auto l = normalize(wi);
            auto h = normalize(v + l);

            float diff_factor = std::max(0.f, dot(n, l));
            auto diffuse = kd * diff_factor;

            Color specular{0.f, 0.f, 0.f};

            if (diff_factor > 0.0 && gg != 0) 
                specular = ks * std::pow(std::max(0.f, dot(n, h)), gg);
            

            L = L + Li * (diffuse + specular);
            
        }

    
        if(depth < maxDepth && (km.r > 0.f || km.g > 0.f || km.b > 0.f))
        {
            Vec3 rd = normalize(-v + n * 2 * dot(n, v) );
            Ray reflected_ray = isect.spawnRay(rd);
            auto tempL = this->li(reflected_ray, scene, sampler, depth + 1);
            Color bounce = tempL.has_value() ? tempL.value() : scene.background->sample(rd);

            L = L + km * bounce;
            
        }

        return L;
    }

};