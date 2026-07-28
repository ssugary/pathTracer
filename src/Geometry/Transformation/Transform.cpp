#include "Transform.hpp"
#include "Utils/common.hpp"
#include "ssmath3/ssmath3.hpp"
#include "Geometry/Interactions/SurfaceInteraction.hpp"
#include <string>

namespace Geo 
{
    inline float gamma(int n) {
        float epsilon = std::numeric_limits<float>::epsilon() * 0.5;
        return (n * epsilon) / (1.0 - n * epsilon);
    }   

    Transform::Transform() : transMat(), invTransMat() {};
    Transform::Transform(const Mat4& m) : transMat(m), invTransMat(::inverse(transMat)) {};
    Transform::Transform(const Mat4& m, const Mat4& m_inv) : transMat(m), invTransMat(m_inv) {};
    
    bool Transform::swapHandedness() const
    {
        return det(static_cast<Mat3>(invTransMat)) < 0;
    }
    Point4 Transform::operator()(const Point4& p) const
    {
        return transMat * p;
    }
    Point3 Transform::operator()(const Point3& p, Vec3* err) const
    {
        if(err) 
            {
                float xErr = (std::abs(transMat[0][0] * p.x) + std::abs(transMat[0][1] * p.y) + std::abs(transMat[0][2] * p.z) + std::abs(transMat[0][3]));
                float yErr = (std::abs(transMat[1][0] * p.x) + std::abs(transMat[1][1] * p.y) + std::abs(transMat[1][2] * p.z) + std::abs(transMat[1][3]));
                float zErr = (std::abs(transMat[2][0] * p.x) + std::abs(transMat[2][1] * p.y) + std::abs(transMat[2][2] * p.z) + std::abs(transMat[2][3]));
                *err = Vec3(xErr, yErr, zErr) * gamma(3);
            }
        return static_cast<Point3>(transMat * Point4(p));
    }
    Point3 Transform::operator()(const Point3& p, const Vec3& pError, Vec3* transError) const
    {
        float x = p.x, y = p.y, z = p.z;

        float xErr = (std::abs(transMat[0][0] * x) + std::abs(transMat[0][1] * y) + std::abs(transMat[0][2] * z) + std::abs(transMat[0][3]));
        float yErr = (std::abs(transMat[1][0] * x) + std::abs(transMat[1][1] * y) + std::abs(transMat[1][2] * z) + std::abs(transMat[1][3]));
        float zErr = (std::abs(transMat[2][0] * x) + std::abs(transMat[2][1] * y) + std::abs(transMat[2][2] * z) + std::abs(transMat[2][3]));
        *transError = Vec3(xErr, yErr, zErr) * gamma(3);
        *transError = *transError + Vec3(
            std::abs(transMat[0][0]) * pError.x + std::abs(transMat[0][1]) * pError.y + std::abs(transMat[0][2]) * pError.z,
            std::abs(transMat[1][0]) * pError.x + std::abs(transMat[1][1]) * pError.y + std::abs(transMat[1][2]) * pError.z,
            std::abs(transMat[2][0]) * pError.x + std::abs(transMat[2][1]) * pError.y + std::abs(transMat[2][2]) * pError.z
        );

        return static_cast<Point3>(transMat * Point4(p));
    }

    Vec3 Transform::operator()(const Vec3& p) const
    {
        return static_cast<Vec3>(transMat * static_cast<Vec4>(p));
    }
    Vec4 Transform::operator()(const Vec4& p) const
    {
        return transMat * p;
    }
    Normal3 Transform::operator()(const Normal3& n) const
    {
        return static_cast<Normal3>(::transpose(invTransMat) * static_cast<Normal4>(n));
    }
    Ray Transform::operator()(const Ray& r) const
    {
        Vec3 error;
        auto o = (*this) (r.o, &error);
        auto d =   (*this) (r.d);
        float sqrL = sqrLength(d);
        float tmax = r.tMax;
        if(sqrL > 0)
        {
            float dt = dot(abs(d), error) / sqrL;
            o += d * dt;
            tmax -= dt;
        }
        
        return Ray(o, d, r.tMin, tmax, r.time, r.medium);
    }

    Bounds3f Transform::operator()(const Bounds3f& b) const
    {

        Bounds3f ret((*this)(b.pMin));  

        ret = boundUnion(ret,(*this)(Point3(b.pMin.x, b.pMin.y, b.pMin.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMax.x, b.pMin.y, b.pMin.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMin.x, b.pMax.y, b.pMin.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMin.x, b.pMin.y, b.pMax.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMin.x, b.pMax.y, b.pMax.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMax.x, b.pMax.y, b.pMin.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMax.x, b.pMin.y, b.pMax.z)));
        ret = boundUnion(ret,(*this)(Point3(b.pMax.x, b.pMax.y, b.pMax.z)));            

        return ret; 

    }
    SurfaceInteraction Transform::operator()(const SurfaceInteraction& s) const
    {
        SurfaceInteraction sf = s;

        sf.p = (*this) (s.p, s.pError, &sf.pError);
        sf.n = ::normalize((*this) (s.n));
        sf.wo= ::normalize((*this) (s.wo));
        sf.time = s.time;
        sf.mediumInterface = s.mediumInterface;
        sf.uv = s.uv;
        sf.shape = s.shape;
        sf.dndu = (*this)(s.dndu);
        sf.dndv = (*this)(s.dndv);
        sf.dpdu = (*this)(s.dpdu);
        sf.dpdv = (*this)(s.dpdv);
        sf.shading.n    =::normalize((*this)(s.shading.n));
        sf.shading.dpdu =  (*this)(s.shading.dpdu);
        sf.shading.dpdv =  (*this)(s.shading.dpdv);
        sf.shading.dndu =  (*this)(s.shading.dndu);
        sf.shading.dndv =  (*this)(s.shading.dndv);
        sf.dudx = s.dudx;
        sf.dvdx = s.dvdx;
        sf.dudy = s.dudy;
        sf.dvdy = s.dvdy;
        sf.dpdx = (*this)(s.dpdx);
        sf.dpdy = (*this)(s.dpdy);
        sf.bsdf = s.bsdf;
        sf.bssrdf=s.bssrdf;
        sf.primitive = s.primitive;
        // sf.n         = ::faceFoward(sf.n, sf.shading.n);
        sf.shading.n = ::faceFoward(sf.shading.n, sf.n);

        return sf;

    }
    Transform Transform::operator()(const Transform& t) const
    {
        return Transform(transMat * t.getTMat(),t.getTMatInv() * invTransMat);
    }

    bool Transform::operator==(const Transform& t) const
    {
        return transMat == t.getTMat();
    }
    bool Transform::operator!=(const Transform& t) const
    {
        return transMat != t.getTMat();
    }

    std::string Transform::toString() const
    {
        std::string name{""};
        for(int i{0}; i < 4; ++i)
        {
            for(int j{0}; j < 4; ++j)
            {
                name += std::to_string(transMat[i][j]);
            }
        }
        return name;
    }

    Transform Transform::inverse(const Transform& t)
    {
        return Transform(t.getTMatInv(), t.getTMat());
    }
    Transform Transform::transpose(const Transform& t)
    {
        return Transform(::transpose(t.getTMat()), ::transpose(t.getTMatInv()));
    }
    Transform Transform::translate(const Point3& delta)
    {
        Mat4    m{1, 0, 0, delta.x,
                  0, 1, 0, delta.y,
                  0, 0, 1, delta.z,
                  0, 0, 0, 1};

        Mat4 minv{1, 0, 0, -delta.x,
                  0, 1, 0, -delta.y,
                  0, 0, 1, -delta.z,
                  0, 0, 0, 1};

        return Transform(m, minv);
    }
    Transform Transform::translate(const Vec3& delta)
    {
        Mat4    m{1, 0, 0, delta.x,
                  0, 1, 0, delta.y,
                  0, 0, 1, delta.z,
                  0, 0, 0, 1};

        Mat4 minv{1, 0, 0, -delta.x,
                  0, 1, 0, -delta.y,
                  0, 0, 1, -delta.z,
                  0, 0, 0, 1};

        return Transform(m, minv);
    }

    Transform Transform::rotate(const float& angle, const Point3& delta)
    {
        float c = std::cos((M_PI / 180.0) * angle);
        float s = std::sin((M_PI / 180.0) * angle);
        float c1 = 1 - c;

        Vec3 k(delta[0], delta[1], delta[2]);
        k = normalize(k);
        float kx = k.x;
        float ky = k.y;
        float kz = k.z; 

        Mat4 m{   c + c1 * kx * kx,      c1 * kx * ky - s * kz, c1 * kx * kz + s * ky, 0, 
                  c1 * kx * ky + s * kz, c + c1 * ky * ky,      c1 * ky * kz - s * kx, 0,
                  c1 * kx * kz - s * ky, c1 * ky * kz + s * kx, c + c1 * kz * kz,      0,
                  0, 0, 0, 1};


        return Transform(m, ::transpose(m));
    }

    Transform Transform::rotateX(const float& angle)
    {
        float sinT = std::sin((M_PI / 180.0) * angle);
        float cosT = std::cos((M_PI / 180.0) * angle);

        Mat4 m{1, 0,    0,     0,
               0, cosT, -sinT, 0,
               0, sinT, cosT,  0, 
               0, 0,    0,     1};

        return Transform(m, ::transpose(m));
    }
    Transform Transform::rotateY(const float& angle)
    {
        float sinT = std::sin((M_PI / 180.0) * angle);
        float cosT = std::cos((M_PI / 180.0) * angle);

        Mat4 m{cosT, 0, sinT, 0,
               0,    1, 0,     0,
               -sinT, 0, cosT,  0, 
               0,    0, 0,     1};

        return Transform(m, ::transpose(m));

    }
    Transform Transform::rotateZ(const float& angle)
    {
        float sinT = std::sin((M_PI / 180.0) * angle);
        float cosT = std::cos((M_PI / 180.0) * angle);

        Mat4 m{cosT, -sinT,0, 0,
               sinT, cosT, 0, 0,
               0,    0,    1, 0, 
               0,    0,    0, 1};

        return Transform(m, ::transpose(m));
    }
    Transform Transform::scale(const float x, const float y, const float z)
    {
        Mat4 m{   x, 0, 0, 0,
                  0, y, 0, 0,
                  0, 0, z, 0,
                  0, 0, 0, 1};

        Mat4 mInv{1/x, 0, 0, 0,
                  0, 1/y, 0, 0,
                  0, 0, 1/z, 0,
                  0, 0, 0, 1};

        return Transform(m, mInv);
    }

    Transform Transform::scale(const Point3& scales)
    {
        Mat4 m{   scales.x, 0, 0, 0,
                  0, scales.y, 0, 0,
                  0, 0, scales.z, 0,
                  0, 0, 0, 1};

        Mat4 minv{1/scales.x, 0, 0, 0,
                  0, 1/scales.y, 0, 0,
                  0, 0, 1/scales.z, 0,
                  0, 0, 0, 1};

        return Transform(m, minv);
    }
    Transform Transform::perspective(float fov, float n, float f)
    {
        Mat4 p{ 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, f / (n - f), (f * n) / (n - f),
                0, 0, -1, 0
              }; 

        float cotg = 1 / std::tan((M_PI / 360.0) * (fov));
        return scale(-cotg, cotg, 1)(Transform(p));
    }
    Transform Transform::orthographic(float znear, float zfar)
    {
        return scale(-1, 1, 1/(znear - zfar))(translate(Point3(0, 0, znear)));
    }

    Transform Transform::lookAt(const Point3& look_from, const Point3& look_at, const Vec3& vup)
    {
        Vec3 w = normalize(look_from - look_at); //< Create the W axis aligned with vpn.

        Vec3 u = normalize(cross(::normalize(vup), w)); //< Create the U axis perpendicular to the vup and W.

        Vec3 v = cross(w, u); //< Create the V axis perpendicular to W and U .

        Mat4 cameraToWorld{ //< Creates the transformation matrix responsible for the linear
                            //< transformation from camera to image.
            u[0], v[0], w[0],  look_from[0], 
            u[1], v[1], w[1],  look_from[1], 
            u[2], v[2], w[2],look_from[2], 
            0.0, 0.0, 0.0,         1.0};


        return Transform(cameraToWorld, ::inverse(cameraToWorld));
    }

}