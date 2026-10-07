#include "BSDF.hpp"

namespace Mat 
{

    BSDF::BSDF(const Normal3& ns)
    : ns(ns) {};

    void BSDF::add(std::shared_ptr<BxDF> b) 
    { 
        bxdfs.push_back(b);
    }

    Color BSDF::f(const Vec3& wo, const Vec3& wi, BxDFType flags) const
    {
        Color result;
        for(auto& b : bxdfs)
            if(b->matchesFlags(flags))
                result += b->f(wo, wi, ns);
        return result;
    }
    Color BSDF::sampleF(const Vec3& wo, const Point2& u, Vec3* wi, float* pdf,
                BxDFType* sampledType, BxDFType flags) const
    {
        std::vector<BxDF*> matching;
        for(auto& b : bxdfs)
            if(b->matchesFlags(flags))
                matching.push_back(b.get());

        if(matching.empty()) 
        { 
            *pdf = 0.f; 
            return Color(); 
        }

        int idx = std::min((int)(u.x * matching.size()), (int)matching.size() - 1);

        BxDF* chosen = matching[idx];
        
        if(sampledType) 
            *sampledType = chosen->type;

        Point2 uRemapped(u.x * matching.size() - idx, u.y);

        Color f0 = chosen->sampleF(wo, ns, uRemapped, wi, pdf);

        if(*pdf <= 0.f) 
            return Color();

        if(chosen->type & BSDF_SPECULAR)
        {
            *pdf /= matching.size();
            return f0;
        }

        float pdfSum = *pdf;
        Color fSum = f0;
        for(auto* b : matching)
        {
            if(b == chosen) 
                continue;
            if(b->type & BSDF_SPECULAR) 
                continue;

            pdfSum += b->pdf(wo, *wi, ns);
            fSum += b->f(wo, *wi, ns);
        }

        *pdf = pdfSum / matching.size();
        
        return fSum;
    }
    float BSDF::pdf(const Vec3& wo, const Vec3& wi, BxDFType flags) const
    {
        int n_matching = 0;
        float sum = 0.f;
        for(auto& b : bxdfs)
            if(b->matchesFlags(flags))
            {
                sum += b->pdf(wo, wi, ns);
                ++n_matching;
            }
        

        return n_matching > 0 ? sum / n_matching : 0.f;
    }
    bool BSDF::isSpecular() const
    {
        for(auto& b : bxdfs)
            if(!(b->type & BSDF_SPECULAR))
                return false;

        return !bxdfs.empty();
    }
};