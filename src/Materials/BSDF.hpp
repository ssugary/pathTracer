#pragma once

#ifndef BSDF_HPP
#define BSDF_HPP

#include "BxDF.hpp"

#include <vector>
#include <memory>

namespace Mat 
{
    class BSDF 
    {
        private:

            std::vector<std::shared_ptr<BxDF>> bxdfs;
            Normal3 ns;
        public:

            BSDF(const Normal3& ns);

            void add(std::shared_ptr<BxDF> b);

            Color f(const Vec3& wo, const Vec3& wi, BxDFType flags = BSDF_ALL) const;
            Color sampleF(const Vec3& wo, const Point2& u, Vec3* wi, float* pdf,
                        BxDFType* sampledType = nullptr, BxDFType flags = BSDF_ALL) const;
            float pdf(const Vec3& wo, const Vec3& wi, BxDFType flags = BSDF_ALL) const;
            bool isSpecular() const;
    };

    class BSSRDF 
    {
        protected:
            /*TODO*/
        public:
            /*TODO*/
    };
}

#endif