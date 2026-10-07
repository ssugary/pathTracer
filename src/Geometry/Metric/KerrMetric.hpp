#pragma once
#ifndef KERR_METRIC_HPP
#define KERR_METRIC_HPP

#include "Metric.hpp"

namespace Geo
{

    class KerrMetric : public Metric 
    {
      private:

        float M; //< mass 
        float a; //< spin

      public:

        KerrMetric(float M=1.f, float a=1-EPSILON);
      
        PhaseSpaceDerivatives evaluate(const Point4& x, const Vec4& p) const override;
        float eventHorizonRadius() const override; 
        bool isInsideHorizon(const Point4& x) const override;
        float getSpin() const override;
        float getMass() const override;
        Mat4 g(const Point4& x) const override;
        Point3 toCartesian(const Point4& x) const override;
        
            
    };

} // namespace Geo


#endif // KERR_METRIC_HPP
