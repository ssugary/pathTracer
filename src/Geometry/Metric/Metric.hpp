#pragma once
#ifndef METRIC_HPP
#define METRIC_HPP

#include <Utils/common.hpp>

namespace Geo 
{
    struct PhaseSpaceDerivatives 
    {
      Vec4 dxdl;
      Vec4 dpdl;
    };

    class Metric 
    {
        public:

            Metric() = default;
            virtual ~Metric() = default;
            virtual PhaseSpaceDerivatives evaluate(const Point4& x, const Vec4& p) const = 0;
            virtual float eventHorizonRadius() const = 0;
            virtual bool isInsideHorizon(const Point4& x) const = 0;
            virtual float getSpin() const;
            virtual float getMass() const;
            
    };

} // namespace Geo


#endif // METRIC_HPP    
