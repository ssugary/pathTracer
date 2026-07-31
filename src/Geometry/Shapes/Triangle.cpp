#include "Triangle.hpp"
#include <Utils/common.hpp>

namespace Geo
{

    Triangle::Triangle(bool reverseOrientation, bool tSwapHandedness, 
                     const std::shared_ptr<TriangleMesh> &mesh, int triNumber)
                     : Shape(reverseOrientation, tSwapHandedness), mesh(mesh)
    {
        v = &mesh->vertexIndices[3 * triNumber];
    }
            
    bool Triangle::intersect(const Ray &r, float *tHit, SurfaceInteraction *sf, bool) const 
    {
        auto p0 = this->mesh->p[v[0]];
        auto p1 = this->mesh->p[v[1]];
        auto p2 = this->mesh->p[v[2]];
        
        Vec3 o = (r.o - p0); //< Vector (o - v0)
        Vec3 d = r.d;        //< Ray direction
        Vec3 v20 = p2 - p0;  //< Vector (v2 - v0)
        Vec3 v10 = p1 - p0;  //< Vector (v1 - v0)

        Vec3 C = cross(v20, d);
        float det = dot(v10, C);

        if (tSwapHandedness) 
            if (det > -EPSILON_8) 
                return false;

        if (std::abs(det) < EPSILON_8) 
            return false;
        
            

        float invdet = 1.0f / det;

        float U = invdet * dot(C, o); //< U = det(O, v10, v20) / det(v20, v10, d)
        if(U < -EPSILON || U > 1.0f + EPSILON)               //< u >= 0 e v >= 0, e u + v <= 1
            return false;
        
        Vec3 vcross = cross(v10, o);
        float V = invdet * dot(vcross, d); //< V = det(-d, O, v20) / det(v20, v10, d)

        if(V < -EPSILON || U + V > 1.0f + EPSILON) //< u >= 0 e v >= 0, e u + v <= 1
            return false;

        float t = invdet * dot(v20, vcross); //< t = det(-d, v10, O) / det(v20, v10, d)
        if(t < r.tMin || t > r.tMax)                 //< t in range
            return false;
        
    
        if(tHit)
            *tHit = t;
        
        if (sf) 
        {
            sf->time = t;
            
            sf->p = p0 * (1.0 - U - V) + p1 * U + p2 * V;
            
            Point3 pAbs = abs(p0 * (1.0 - U - V)) + abs(p1 * U) + abs(p2 * V);

            sf->pError = SHADOW_EPSILON * static_cast<Vec3>(pAbs);
            
            sf->wo = -r.d;
            sf->shape = this;

            if(mesh->n)
            {  
                Normal3 n0 = mesh->n[v[0]];
                Normal3 n1 = mesh->n[v[1]];
                Normal3 n2 = mesh->n[v[2]];
                
                sf->n = normalize(n0 * (1.0 - U - V) + n1 * U + n2 * V);
            
            }
            else 
                sf->n = static_cast<Normal3>(normalize(cross(v10, v20)));
            
            Point2 uv[3];
            getUVs(uv);

            sf->uv = uv[0] * (1.f - U - V) + uv[1] * U + uv[2] * V;

            if (reverseOrientation ^ tSwapHandedness) 
                sf->n = -sf->n;
            

        }
        
        return true;

    }

    bool Triangle::intersectP(const Ray &r, bool) const 
    {
        auto p0 = this->mesh->p[v[0]];
        auto p1 = this->mesh->p[v[1]];
        auto p2 = this->mesh->p[v[2]];
        
        Vec3 o = (r.o - p0); //< Vector (o - v0)
        Vec3 d = r.d;        //< Ray direction
        Vec3 v20 = p2 - p0;  //< Vector (v2 - v0)
        Vec3 v10 = p1 - p0;  //< Vector (v1 - v0)

        Vec3 C = cross(v20, d);

        float det = dot(v10, C);

        if (tSwapHandedness) 
            if (det > -EPSILON_8) 
                return false;
            
        if (std::abs(det) < EPSILON_8 ) 
            return false;

        float invdet = 1.0f / det;

        float U = invdet * dot(C, o); //< U = det(O, v10, v20) / det(v20, v10, d)
        if(U < -EPSILON|| U > 1.0 + EPSILON)                                     //< u >= 0 e v >= 0, e u + v <= 1
            return false;
        
        Vec3 vcross = cross(v10, o);
        float V = invdet * dot(vcross, d); //< V = det(-d, O, v20) / det(v20, v10, d)

        if(V < -EPSILON || U + V > 1.0 + EPSILON)                                      //< u >= 0 e v >= 0, e u + v <= 1
            return false;

        float t = invdet * dot(v20, vcross); //< t = det(-d, v10, O) / det(v20, v10, d)
        if(t < r.tMin || t > r.tMax)                                    //< t in range
            return false;

        return true;
    }

    Interaction Triangle::sample(const Point2& u, float* pdf) const 
    {
        float su0 = std::sqrt(u.x);
        float b0 = 1.0f - su0;
        float b1 = u.y * su0;
        float b2 = 1.0f - b0 - b1;

        auto p0 = mesh->p[v[0]];
        auto p1 = mesh->p[v[1]];
        auto p2 = mesh->p[v[2]];

        Interaction it;
        it.p = p0 * b0 + p1 * b1 + p2 * b2;

        if (mesh->n)
        {
            Normal3 n0 = mesh->n[v[0]];
            Normal3 n1 = mesh->n[v[1]];
            Normal3 n2 = mesh->n[v[2]];
            it.n = normalize(n0 * b0 + n1 * b1 + n2 * b2);
        }
        else
        {
            it.n = normalize(static_cast<Normal3>(cross(p1 - p0, p2 - p0)));
        }

        if (reverseOrientation ^ tSwapHandedness)
            it.n = -it.n;

        *pdf = 1.0f / area(); 

        return it;
    }

    Bounds3f Triangle::objectBound() const 
    {
        const Point3 &p0 = mesh->p[v[0]];
        const Point3 &p1 = mesh->p[v[1]];
        const Point3 &p2 = mesh->p[v[2]];

        return boundUnion(Bounds3f(p0,p1) ,p2);
    }

    float Triangle::area() const
    {
        const Point3 &p0 = mesh->p[v[0]];
        const Point3 &p1 = mesh->p[v[1]];
        const Point3 &p2 = mesh->p[v[2]];

        return 0.5f * length(cross(p1 - p0, p2 - p0));
    }

    void Triangle::getUVs(Point2* uv) const
    {
        if (mesh->uv) 
        {
            uv[0] = mesh->uv[v[0]];
            uv[1] = mesh->uv[v[1]];
            uv[2] = mesh->uv[v[2]];
        } 
        else 
        {
            uv[0] = Point2(0, 0);
            uv[1] = Point2(1, 0);
            uv[2] = Point2(1, 1);
        }
    }
};