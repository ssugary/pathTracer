#include "SimplePathIntegrator.hpp"

#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include "Materials/BlinnPhongMaterial.hpp"
#include "Light/AreaLight.hpp"
#include "Light/VisibilityTester.hpp"
namespace Itg 
{
    float powerHeuristic(float pdfA, float pdfB)
    {
        float a2 = pdfA * pdfA;
        float b2 = pdfB * pdfB;
        float d = a2 + b2;

        return (d > 0.f) ? a2 / d : 0.f;
    }

    void SimplePathIntegrator::preprocess(const Scene& scene)  
    {
        this->lightSampler = Sam::UniformLightSampler(scene.lights);
    }
    
    std::optional<Color> SimplePathIntegrator::li(const RayDifferential& ray, const Scene& scene, Sam::Sampler& sampler) const
    {
        Color L;
        Color beta(1.f);
        bool found{false};
        RayDifferential r = ray;
        SurfaceInteraction prevIsect;
        float prevBsdfPdf{0.f};
        bool specularBounce{true};


        for(int i{0}; i < maxDepth; ++i)
        {
            SurfaceInteraction isect;

            found = scene.intersect(r, &isect);

            
            if(!found)
            {
                if(i == 0 || specularBounce)
                    L += beta * scene.background->sample(r.d);
                else if(!lightSampler.empty() && lightSampler.envLight)
                {
                    float envPdf = lightSampler.envLight->pdf(prevIsect, r.d) / lightSampler.get().size();

                    if (envPdf > 0.f)
                        L += beta * powerHeuristic(prevBsdfPdf, envPdf) * scene.background->sample(r.d);   
                }

                break;
            }

            isect.computeDifferentials(r);
            
            if(dot(r.d, isect.n) > 0.f)
                isect.n = -isect.n;
            
            isect.shading.n = faceFoward(isect.shading.n, isect.n);

            auto areaLight = isect.primitive->getAreaLight();

            if(areaLight)
            {
                Color emit = areaLight->L(isect, -r.d);
                if(i == 0 || specularBounce) 
                    L += beta * emit; 
                else if(!lightSampler.empty())
                {

                    float lightPdf = areaLight->pdf(prevIsect, r.d) / lightSampler.get().size();
                    float weight = powerHeuristic(prevBsdfPdf, lightPdf);
                    
                    L += beta * weight * emit;
                }
            }

            auto bsdfOwner = isect.primitive->getMaterial()->computeBSDF(isect);
            isect.bsdf = bsdfOwner.get();
            auto& bsdf = isect.bsdf;

            if(!bsdf->isSpecular() && !lightSampler.empty())
            {
                const auto& lights = lightSampler.get();
                int lightIdx = std::min(static_cast<int>(sampler.get1D() * lights.size()), static_cast<int>(lights.size() - 1));

                auto& light = lights[lightIdx];

                Vec3 wiLight;
                float lightPdf = 0.f;
                Luz::VisibilityTester vis;
                Point2 uLight = sampler.get2D();

                Color Li = light->sampleLi(isect, uLight, &wiLight, &lightPdf, &vis);

                lightPdf /= lights.size();

                if(lightPdf > 0.f && (Li.r != 0.f || Li.g != 0.f || Li.b != 0.f)) 
                {
                    float cosThetaL = std::max(0.f, dot(isect.shading.n, wiLight));
                    Color f = bsdf->f(-r.d, wiLight);

                    if((f.r != 0.f || f.g != 0.f || f.b != 0.f) && vis.unoccluded(scene))
                    {
                        float weight = 1.f;
                        if(light->flag == Luz::LightFlag::AREA || light->flag == Luz::LightFlag::ENVIRONMENT)
                        {
                            float bsdfPdf = bsdf->pdf(-r.d, wiLight);
                            weight = powerHeuristic(lightPdf, bsdfPdf);
                        }

                        L += beta * (f * Li * cosThetaL * weight) / lightPdf;
                    }
                }

            }

            Vec3 wo{-r.d};
            Vec3 wi;
            float pdf;
            Point2 u = sampler.get2D();
            Color brdf = bsdf->sampleF(wo, u, &wi, &pdf);

            if (pdf <= EPSILON_3 || (brdf.r == 0.f && brdf.g == 0.f && brdf.b == 0.f)) 
                break;

            float cosTheta = std::max(0.f, dot(isect.shading.n, wi));
            beta *= (brdf * cosTheta) / pdf;

            prevIsect = isect;
            prevBsdfPdf = pdf; 
            specularBounce = bsdf->isSpecular();

            RayDifferential ray = isect.spawnRay(wi);;

            if(r.hasDifferentials)
            {
                ray.hasDifferentials = true;
                ray.rxOrigin = isect.p + isect.dpdx;
                ray.ryOrigin = isect.p + isect.dpdy;
                Normal3 dndx = isect.shading.dndu * isect.dudx + isect.shading.dndv * isect.dvdx;
                Normal3 dndy = isect.shading.dndu * isect.dudy + isect.shading.dndv * isect.dvdy;
                Vec3 dwodx = -r.rxDirection - wo;
                Vec3 dwody = -r.ryDirection - wo;
                float dDNdx = ::dot(dwodx, isect.shading.n) + ::dot(wo, dndx);
                float dDNdy = ::dot(dwody, isect.shading.n) + ::dot(wo, dndy);
                ray.rxDirection = wi - dwodx + 2.f * Vec3(::dot(wo, isect.shading.n) * dndx + dDNdx * isect.shading.n);
                ray.ryDirection = wi - dwody + 2.f * Vec3(::dot(wo, isect.shading.n) * dndy + dDNdy * isect.shading.n);
            }
            
            r = ray;

            if(i > 3)
            {
                float q = std::max(0.05f, 1.f - std::max({beta.r, beta.g, beta.b}));
                if (sampler.get1D() < q) 
                    break;
                beta /= std::max(EPSILON, 1.f - q);
            }
        }

        return std::optional(L);
    }
}