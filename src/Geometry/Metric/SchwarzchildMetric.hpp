#pragma once
#ifndef SCHWARZCHILD_METRIC_HPP
#define SCHWARZCHILD_METRIC_HPP

#include "Metric.hpp"

namespace Geo
{

    class SchwarzchildMetric : public Metric
    {
      private:
        float M;
      public:

        SchwarzchildMetric(float M=1.f);
      
        PhaseSpaceDerivatives evaluate(const Point4& x, const Vec4& p) const override;
        float eventHorizonRadius() const override; 
        float getMass() const override;
        bool isInsideHorizon(const Point4& x) const override;
        Mat4 g(const Point4& x) const override;
        Point3 toCartesian(const Point4& x) const override;
        
    };

} // namespace Geo


#endif // SCHWARZCHILD_METRIC_HPP 
