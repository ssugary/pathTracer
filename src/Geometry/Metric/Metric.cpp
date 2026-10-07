#include "Metric.hpp"

namespace Geo 
{
    float Metric::getSpin() const
    {
        return 0.f;
    }

    float Metric::getMass() const
    {
        return 1.f;
    }

    float Metric::hamiltonian(const Point4& x, const Vec4& p) const
    {
        PhaseSpaceDerivatives derivs = this->evaluate(x, p);
        return 0.5f * (derivs.dxdl.x * p.x + 
                       derivs.dxdl.y * p.y + 
                       derivs.dxdl.z * p.z + 
                       derivs.dxdl.w * p.w);
    }
    
}; //< namespace Geo