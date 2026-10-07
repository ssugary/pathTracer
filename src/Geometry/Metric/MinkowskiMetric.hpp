#pragma once 
#ifndef MINKOWSKI_METRIC_HPP
#define MINKOWSKI_METRIC_HPP

#include <Geometry/Metric/Metric.hpp>

namespace Geo 
{

    class MinkowskiMetric : public Metric
    {
        public:
            MinkowskiMetric();

            PhaseSpaceDerivatives evaluate(const Point4& x, const Vec4& p) const override;
            float eventHorizonRadius() const override; 
            bool isInsideHorizon(const Point4& x) const override;
            float getMass() const override;
            Mat4 g(const Point4& x) const override;
            Point3 toCartesian(const Point4& x) const override;
    };

} // namespace Geo


#endif // MINKOWSKI_METRIC_HPP  
