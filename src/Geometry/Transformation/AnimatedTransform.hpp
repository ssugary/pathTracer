#pragma once

#ifndef ANIMATED_TRANSFORM_HPP
#define ANIMATED_TRANSFORM_HPP

#include "Intervals.hpp"
#include "Geometry/Bounds/Bounds3.hpp"
#include "Transform.hpp"
#include "Quaternion.hpp"
#include <cstddef>

namespace Geo 
{
    class AnimatedTransform 
    {
        private:
            struct DerivativeTerm 
            {
                float kc;
                float kx;
                float ky;
                float kz;
                DerivativeTerm(float c=0.f, float x=0.f, float y=0.f, float z=0.f)
                : kc(c), kx(x), ky(y), kz(z) {};

                float eval(const Point3& p) const
                {
                    return kc + kx * p.x + ky * p.y + kz * p.z;
                }
            };
        
            DerivativeTerm  c1[3], c2[3], c3[3], c4[3], c5[3];
            const Transform *startTransform, *endTransform;
            const float startTime, endTime;
            const bool actuallyAnimated;
            Vec3 t[2];
            Quaternion r[2];
            Mat4 s[2];
            bool hasRotation;


        public:

            AnimatedTransform(const Transform *startTransform,
                              float startTime, const Transform *endTransform, 
                              float endTime)
            : startTransform(startTransform), endTransform(endTransform),
              startTime(startTime), endTime(endTime),
              actuallyAnimated(*startTransform != *endTransform)
            
            {
                decompose(startTransform->getTMat(), &t[0], &r[0], &s[0]);
                decompose(endTransform->getTMat(), &t[1], &r[1], &s[1]);
                if (dot(r[0], r[1]) < 0)
                    r[1] = -r[1];
                hasRotation = dot(r[0], r[1]) < 0.9995f;
                if (hasRotation) 
                {
                    float cosTheta = dot(r[0], r[1]);
                    float theta = std::acos(clamp(cosTheta, -1, 1));
                    Quaternion qperp = normalize(r[1] - r[0] * cosTheta);
                
                    float t0x = t[0].x;
                    float t0y = t[0].y;
                    float t0z = t[0].z;
                    float t1x = t[1].x;
                    float t1y = t[1].y;
                    float t1z = t[1].z;
                    float q1x = r[0].v.x;
                    float q1y = r[0].v.y;
                    float q1z = r[0].v.z;
                    float q1w = r[0].w;
                    float qperpx = qperp.v.x;
                    float qperpy = qperp.v.y;
                    float qperpz = qperp.v.z;
                    float qperpw = qperp.w;
                    float s000 = s[0][0][0];
                    float s001 = s[0][0][1];
                    float s002 = s[0][0][2];
                    float s010 = s[0][1][0];
                    float s011 = s[0][1][1];
                    float s012 = s[0][1][2];
                    float s020 = s[0][2][0];
                    float s021 = s[0][2][1];
                    float s022 = s[0][2][2];
                    float s100 = s[1][0][0];
                    float s101 = s[1][0][1];
                    float s102 = s[1][0][2];
                    float s110 = s[1][1][0];
                    float s111 = s[1][1][1];
                    float s112 = s[1][1][2];
                    float s120 = s[1][2][0];
                    float s121 = s[1][2][1];
                    float s122 = s[1][2][2];
                
                    c1[0] =
                        DerivativeTerm(-t0x + t1x,
                                (-1 + q1y*q1y + q1z*q1z + qperpy*qperpy + qperpz*qperpz)*s000 + 
                                q1w*q1z*s010 - qperpx*qperpy*s010 + qperpw*qperpz*s010 - q1w*q1y*s020 - 
                                qperpw*qperpy*s020 - qperpx*qperpz*s020 + s100 - q1y*q1y*s100 - 
                                q1z*q1z*s100 - qperpy*qperpy*s100 - qperpz*qperpz*s100 - q1w*q1z*s110 + 
                                qperpx*qperpy*s110 - qperpw*qperpz*s110 + q1w*q1y*s120 + qperpw*qperpy*s120 + 
                                qperpx*qperpz*s120 + q1x*(-(q1y*s010) - q1z*s020 + q1y*s110 + q1z*s120),
                                (-1 + q1y*q1y + q1z*q1z + qperpy*qperpy + qperpz*qperpz)*s001 + 
                                q1w*q1z*s011 - qperpx*qperpy*s011 + qperpw*qperpz*s011 - q1w*q1y*s021 - 
                                qperpw*qperpy*s021 - qperpx*qperpz*s021 + s101 - q1y*q1y*s101 - 
                                q1z*q1z*s101 - qperpy*qperpy*s101 - qperpz*qperpz*s101 - q1w*q1z*s111 + 
                                qperpx*qperpy*s111 - qperpw*qperpz*s111 + q1w*q1y*s121 + qperpw*qperpy*s121 + 
                                qperpx*qperpz*s121 + q1x*(-(q1y*s011) - q1z*s021 + q1y*s111 + q1z*s121),
                                (-1 + q1y*q1y + q1z*q1z + qperpy*qperpy + qperpz*qperpz)*s002 + 
                                q1w*q1z*s012 - qperpx*qperpy*s012 + qperpw*qperpz*s012 - q1w*q1y*s022 - 
                                qperpw*qperpy*s022 - qperpx*qperpz*s022 + s102 - q1y*q1y*s102 - 
                                q1z*q1z*s102 - qperpy*qperpy*s102 - qperpz*qperpz*s102 - q1w*q1z*s112 + 
                                qperpx*qperpy*s112 - qperpw*qperpz*s112 + q1w*q1y*s122 + qperpw*qperpy*s122 + 
                                qperpx*qperpz*s122 + q1x*(-(q1y*s012) - q1z*s022 + q1y*s112 + q1z*s122));
                
                    c2[0] = DerivativeTerm(0.,
                                -(qperpy*qperpy*s000) - qperpz*qperpz*s000 + qperpx*qperpy*s010 - 
                                qperpw*qperpz*s010 + qperpw*qperpy*s020 + qperpx*qperpz*s020 + 
                                q1y*q1y*(s000 - s100) + q1z*q1z*(s000 - s100) + qperpy*qperpy*s100 + 
                                qperpz*qperpz*s100 - qperpx*qperpy*s110 + qperpw*qperpz*s110 - 
                                qperpw*qperpy*s120 - qperpx*qperpz*s120 + 2*q1x*qperpy*s010*theta - 
                                2*q1w*qperpz*s010*theta + 2*q1w*qperpy*s020*theta + 2*q1x*qperpz*s020*theta + 
                                q1y*(q1x*(-s010 + s110) + q1w*(-s020 + s120) + 
                                    2*(-2*qperpy*s000 + qperpx*s010 + qperpw*s020)*theta) + 
                                q1z*(q1w*(s010 - s110) + q1x*(-s020 + s120) - 
                                    2*(2*qperpz*s000 + qperpw*s010 - qperpx*s020)*theta),
                                -(qperpy*qperpy*s001) - qperpz*qperpz*s001 + qperpx*qperpy*s011 - 
                                qperpw*qperpz*s011 + qperpw*qperpy*s021 + qperpx*qperpz*s021 + 
                                q1y*q1y*(s001 - s101) + q1z*q1z*(s001 - s101) + qperpy*qperpy*s101 + 
                                qperpz*qperpz*s101 - qperpx*qperpy*s111 + qperpw*qperpz*s111 - 
                                qperpw*qperpy*s121 - qperpx*qperpz*s121 + 2*q1x*qperpy*s011*theta - 
                                2*q1w*qperpz*s011*theta + 2*q1w*qperpy*s021*theta + 2*q1x*qperpz*s021*theta + 
                                q1y*(q1x*(-s011 + s111) + q1w*(-s021 + s121) + 
                                    2*(-2*qperpy*s001 + qperpx*s011 + qperpw*s021)*theta) + 
                                q1z*(q1w*(s011 - s111) + q1x*(-s021 + s121) - 
                                    2*(2*qperpz*s001 + qperpw*s011 - qperpx*s021)*theta),
                                -(qperpy*qperpy*s002) - qperpz*qperpz*s002 + qperpx*qperpy*s012 - 
                                qperpw*qperpz*s012 + qperpw*qperpy*s022 + qperpx*qperpz*s022 + 
                                q1y*q1y*(s002 - s102) + q1z*q1z*(s002 - s102) + qperpy*qperpy*s102 + 
                                qperpz*qperpz*s102 - qperpx*qperpy*s112 + qperpw*qperpz*s112 - 
                                qperpw*qperpy*s122 - qperpx*qperpz*s122 + 2*q1x*qperpy*s012*theta - 
                                2*q1w*qperpz*s012*theta + 2*q1w*qperpy*s022*theta + 2*q1x*qperpz*s022*theta + 
                                q1y*(q1x*(-s012 + s112) + q1w*(-s022 + s122) + 
                                    2*(-2*qperpy*s002 + qperpx*s012 + qperpw*s022)*theta) + 
                                q1z*(q1w*(s012 - s112) + q1x*(-s022 + s122) - 
                                    2*(2*qperpz*s002 + qperpw*s012 - qperpx*s022)*theta));
                
                    c3[0] = DerivativeTerm(0.,
                                -2*(q1x*qperpy*s010 - q1w*qperpz*s010 + q1w*qperpy*s020 + q1x*qperpz*s020 - 
                                    q1x*qperpy*s110 + q1w*qperpz*s110 - q1w*qperpy*s120 - q1x*qperpz*s120 + 
                                    q1y*(-2*qperpy*s000 + qperpx*s010 + qperpw*s020 + 2*qperpy*s100 - qperpx*s110 - 
                                        qperpw*s120) + q1z*(-2*qperpz*s000 - qperpw*s010 + qperpx*s020 + 
                                                            2*qperpz*s100 + qperpw*s110 - qperpx*s120))*theta,
                                -2*(q1x*qperpy*s011 - q1w*qperpz*s011 + q1w*qperpy*s021 + q1x*qperpz*s021 - 
                                    q1x*qperpy*s111 + q1w*qperpz*s111 - q1w*qperpy*s121 - q1x*qperpz*s121 + 
                                    q1y*(-2*qperpy*s001 + qperpx*s011 + qperpw*s021 + 2*qperpy*s101 - qperpx*s111 - 
                                        qperpw*s121) + q1z*(-2*qperpz*s001 - qperpw*s011 + qperpx*s021 + 
                                                            2*qperpz*s101 + qperpw*s111 - qperpx*s121))*theta,
                                -2*(q1x*qperpy*s012 - q1w*qperpz*s012 + q1w*qperpy*s022 + q1x*qperpz*s022 - 
                                    q1x*qperpy*s112 + q1w*qperpz*s112 - q1w*qperpy*s122 - q1x*qperpz*s122 + 
                                    q1y*(-2*qperpy*s002 + qperpx*s012 + qperpw*s022 + 2*qperpy*s102 - qperpx*s112 - 
                                        qperpw*s122) + q1z*(-2*qperpz*s002 - qperpw*s012 + qperpx*s022 + 
                                                            2*qperpz*s102 + qperpw*s112 - qperpx*s122))*theta);
                
                    c4[0] = DerivativeTerm(0.,
                                -(q1x*qperpy*s010) + q1w*qperpz*s010 - q1w*qperpy*s020 - q1x*qperpz*s020 + 
                                q1x*qperpy*s110 - q1w*qperpz*s110 + q1w*qperpy*s120 + q1x*qperpz*s120 + 
                                2*q1y*q1y*s000*theta + 2*q1z*q1z*s000*theta - 
                                2*qperpy*qperpy*s000*theta - 2*qperpz*qperpz*s000*theta + 
                                2*qperpx*qperpy*s010*theta - 2*qperpw*qperpz*s010*theta + 
                                2*qperpw*qperpy*s020*theta + 2*qperpx*qperpz*s020*theta + 
                                q1y*(-(qperpx*s010) - qperpw*s020 + 2*qperpy*(s000 - s100) + qperpx*s110 + 
                                    qperpw*s120 - 2*q1x*s010*theta - 2*q1w*s020*theta) + 
                                q1z*(2*qperpz*s000 + qperpw*s010 - qperpx*s020 - 2*qperpz*s100 - qperpw*s110 + 
                                    qperpx*s120 + 2*q1w*s010*theta - 2*q1x*s020*theta),
                                -(q1x*qperpy*s011) + q1w*qperpz*s011 - q1w*qperpy*s021 - q1x*qperpz*s021 + 
                                q1x*qperpy*s111 - q1w*qperpz*s111 + q1w*qperpy*s121 + q1x*qperpz*s121 + 
                                2*q1y*q1y*s001*theta + 2*q1z*q1z*s001*theta - 
                                2*qperpy*qperpy*s001*theta - 2*qperpz*qperpz*s001*theta + 
                                2*qperpx*qperpy*s011*theta - 2*qperpw*qperpz*s011*theta + 
                                2*qperpw*qperpy*s021*theta + 2*qperpx*qperpz*s021*theta + 
                                q1y*(-(qperpx*s011) - qperpw*s021 + 2*qperpy*(s001 - s101) + qperpx*s111 + 
                                    qperpw*s121 - 2*q1x*s011*theta - 2*q1w*s021*theta) + 
                                q1z*(2*qperpz*s001 + qperpw*s011 - qperpx*s021 - 2*qperpz*s101 - qperpw*s111 + 
                                    qperpx*s121 + 2*q1w*s011*theta - 2*q1x*s021*theta),
                                -(q1x*qperpy*s012) + q1w*qperpz*s012 - q1w*qperpy*s022 - q1x*qperpz*s022 + 
                                q1x*qperpy*s112 - q1w*qperpz*s112 + q1w*qperpy*s122 + q1x*qperpz*s122 + 
                                2*q1y*q1y*s002*theta + 2*q1z*q1z*s002*theta - 
                                2*qperpy*qperpy*s002*theta - 2*qperpz*qperpz*s002*theta + 
                                2*qperpx*qperpy*s012*theta - 2*qperpw*qperpz*s012*theta + 
                                2*qperpw*qperpy*s022*theta + 2*qperpx*qperpz*s022*theta + 
                                q1y*(-(qperpx*s012) - qperpw*s022 + 2*qperpy*(s002 - s102) + qperpx*s112 + 
                                    qperpw*s122 - 2*q1x*s012*theta - 2*q1w*s022*theta) + 
                                q1z*(2*qperpz*s002 + qperpw*s012 - qperpx*s022 - 2*qperpz*s102 - qperpw*s112 + 
                                    qperpx*s122 + 2*q1w*s012*theta - 2*q1x*s022*theta));
                
                
                    c5[0] = DerivativeTerm(0.,
                                2*(qperpy*qperpy*s000 + qperpz*qperpz*s000 - qperpx*qperpy*s010 + 
                                    qperpw*qperpz*s010 - qperpw*qperpy*s020 - qperpx*qperpz*s020 - 
                                    qperpy*qperpy*s100 - qperpz*qperpz*s100 + q1y*q1y*(-s000 + s100) + 
                                    q1z*q1z*(-s000 + s100) + qperpx*qperpy*s110 - qperpw*qperpz*s110 + 
                                    q1y*(q1x*(s010 - s110) + q1w*(s020 - s120)) + qperpw*qperpy*s120 + 
                                    qperpx*qperpz*s120 + q1z*(-(q1w*s010) + q1x*s020 + q1w*s110 - q1x*s120))*theta,
                                2*(qperpy*qperpy*s001 + qperpz*qperpz*s001 - qperpx*qperpy*s011 + 
                                    qperpw*qperpz*s011 - qperpw*qperpy*s021 - qperpx*qperpz*s021 - 
                                    qperpy*qperpy*s101 - qperpz*qperpz*s101 + q1y*q1y*(-s001 + s101) + 
                                    q1z*q1z*(-s001 + s101) + qperpx*qperpy*s111 - qperpw*qperpz*s111 + 
                                    q1y*(q1x*(s011 - s111) + q1w*(s021 - s121)) + qperpw*qperpy*s121 + 
                                    qperpx*qperpz*s121 + q1z*(-(q1w*s011) + q1x*s021 + q1w*s111 - q1x*s121))*theta,
                                2*(qperpy*qperpy*s002 + qperpz*qperpz*s002 - qperpx*qperpy*s012 + 
                                    qperpw*qperpz*s012 - qperpw*qperpy*s022 - qperpx*qperpz*s022 - 
                                    qperpy*qperpy*s102 - qperpz*qperpz*s102 + q1y*q1y*(-s002 + s102) + 
                                    q1z*q1z*(-s002 + s102) + qperpx*qperpy*s112 - qperpw*qperpz*s112 + 
                                    q1y*(q1x*(s012 - s112) + q1w*(s022 - s122)) + qperpw*qperpy*s122 + 
                                    qperpx*qperpz*s122 + q1z*(-(q1w*s012) + q1x*s022 + q1w*s112 - q1x*s122))*theta);
                
                
                    c1[1] = DerivativeTerm(-t0y + t1y,
                                -(qperpx*qperpy*s000) - qperpw*qperpz*s000 - s010 + q1z*q1z*s010 + 
                                qperpx*qperpx*s010 + qperpz*qperpz*s010 - q1y*q1z*s020 + qperpw*qperpx*s020 - 
                                qperpy*qperpz*s020 + qperpx*qperpy*s100 + qperpw*qperpz*s100 + 
                                q1w*q1z*(-s000 + s100) + q1x*q1x*(s010 - s110) + s110 - q1z*q1z*s110 - 
                                qperpx*qperpx*s110 - qperpz*qperpz*s110 + 
                                q1x*(q1y*(-s000 + s100) + q1w*(s020 - s120)) + q1y*q1z*s120 - qperpw*qperpx*s120 + 
                                qperpy*qperpz*s120,
                                -(qperpx*qperpy*s001) - qperpw*qperpz*s001 - s011 + q1z*q1z*s011 + 
                                qperpx*qperpx*s011 + qperpz*qperpz*s011 - q1y*q1z*s021 + qperpw*qperpx*s021 - 
                                qperpy*qperpz*s021 + qperpx*qperpy*s101 + qperpw*qperpz*s101 + 
                                q1w*q1z*(-s001 + s101) + q1x*q1x*(s011 - s111) + s111 - q1z*q1z*s111 - 
                                qperpx*qperpx*s111 - qperpz*qperpz*s111 + 
                                q1x*(q1y*(-s001 + s101) + q1w*(s021 - s121)) + q1y*q1z*s121 - qperpw*qperpx*s121 + 
                                qperpy*qperpz*s121,
                                -(qperpx*qperpy*s002) - qperpw*qperpz*s002 - s012 + q1z*q1z*s012 + 
                                qperpx*qperpx*s012 + qperpz*qperpz*s012 - q1y*q1z*s022 + qperpw*qperpx*s022 - 
                                qperpy*qperpz*s022 + qperpx*qperpy*s102 + qperpw*qperpz*s102 + 
                                q1w*q1z*(-s002 + s102) + q1x*q1x*(s012 - s112) + s112 - q1z*q1z*s112 - 
                                qperpx*qperpx*s112 - qperpz*qperpz*s112 + 
                                q1x*(q1y*(-s002 + s102) + q1w*(s022 - s122)) + q1y*q1z*s122 - 
                                qperpw*qperpx*s122 + qperpy*qperpz*s122);
                
                    c2[1] = DerivativeTerm(0.,
                                qperpx*qperpy*s000 + qperpw*qperpz*s000 + q1z*q1z*s010 - qperpx*qperpx*s010 - 
                                qperpz*qperpz*s010 - q1y*q1z*s020 - qperpw*qperpx*s020 + qperpy*qperpz*s020 - 
                                qperpx*qperpy*s100 - qperpw*qperpz*s100 + q1x*q1x*(s010 - s110) - 
                                q1z*q1z*s110 + qperpx*qperpx*s110 + qperpz*qperpz*s110 + q1y*q1z*s120 + 
                                qperpw*qperpx*s120 - qperpy*qperpz*s120 + 2*q1z*qperpw*s000*theta + 
                                2*q1y*qperpx*s000*theta - 4*q1z*qperpz*s010*theta + 2*q1z*qperpy*s020*theta + 
                                2*q1y*qperpz*s020*theta + q1x*(q1w*s020 + q1y*(-s000 + s100) - q1w*s120 + 
                                                                2*qperpy*s000*theta - 4*qperpx*s010*theta - 2*qperpw*s020*theta) + 
                                q1w*(-(q1z*s000) + q1z*s100 + 2*qperpz*s000*theta - 2*qperpx*s020*theta),
                                qperpx*qperpy*s001 + qperpw*qperpz*s001 + q1z*q1z*s011 - qperpx*qperpx*s011 - 
                                qperpz*qperpz*s011 - q1y*q1z*s021 - qperpw*qperpx*s021 + qperpy*qperpz*s021 - 
                                qperpx*qperpy*s101 - qperpw*qperpz*s101 + q1x*q1x*(s011 - s111) - 
                                q1z*q1z*s111 + qperpx*qperpx*s111 + qperpz*qperpz*s111 + q1y*q1z*s121 + 
                                qperpw*qperpx*s121 - qperpy*qperpz*s121 + 2*q1z*qperpw*s001*theta + 
                                2*q1y*qperpx*s001*theta - 4*q1z*qperpz*s011*theta + 2*q1z*qperpy*s021*theta + 
                                2*q1y*qperpz*s021*theta + q1x*(q1w*s021 + q1y*(-s001 + s101) - q1w*s121 + 
                                                                2*qperpy*s001*theta - 4*qperpx*s011*theta - 2*qperpw*s021*theta) + 
                                q1w*(-(q1z*s001) + q1z*s101 + 2*qperpz*s001*theta - 2*qperpx*s021*theta),
                                qperpx*qperpy*s002 + qperpw*qperpz*s002 + q1z*q1z*s012 - qperpx*qperpx*s012 - 
                                qperpz*qperpz*s012 - q1y*q1z*s022 - qperpw*qperpx*s022 + qperpy*qperpz*s022 - 
                                qperpx*qperpy*s102 - qperpw*qperpz*s102 + q1x*q1x*(s012 - s112) - 
                                q1z*q1z*s112 + qperpx*qperpx*s112 + qperpz*qperpz*s112 + q1y*q1z*s122 + 
                                qperpw*qperpx*s122 - qperpy*qperpz*s122 + 2*q1z*qperpw*s002*theta + 
                                2*q1y*qperpx*s002*theta - 4*q1z*qperpz*s012*theta + 2*q1z*qperpy*s022*theta + 
                                2*q1y*qperpz*s022*theta +
                                q1x*(q1w*s022 + q1y*(-s002 + s102) - q1w*s122 + 
                                    2*qperpy*s002*theta - 4*qperpx*s012*theta - 2*qperpw*s022*theta) + 
                                q1w*(-(q1z*s002) + q1z*s102 + 2*qperpz*s002*theta - 2*qperpx*s022*theta));
                
                    c3[1] = DerivativeTerm(0.,
                                2*(-(q1x*qperpy*s000) - q1w*qperpz*s000 + 2*q1x*qperpx*s010 + q1x*qperpw*s020 + 
                                    q1w*qperpx*s020 + q1x*qperpy*s100 + q1w*qperpz*s100 - 2*q1x*qperpx*s110 - 
                                    q1x*qperpw*s120 - q1w*qperpx*s120 + 
                                    q1z*(2*qperpz*s010 - qperpy*s020 + qperpw*(-s000 + s100) - 2*qperpz*s110 + 
                                        qperpy*s120) + 
                                    q1y*(-(qperpx*s000) - qperpz*s020 + qperpx*s100 + qperpz*s120))* theta,
                                2*(-(q1x*qperpy*s001) - q1w*qperpz*s001 + 2*q1x*qperpx*s011 + q1x*qperpw*s021 + 
                                    q1w*qperpx*s021 + q1x*qperpy*s101 + q1w*qperpz*s101 - 2*q1x*qperpx*s111 - 
                                    q1x*qperpw*s121 - q1w*qperpx*s121 + 
                                    q1z*(2*qperpz*s011 - qperpy*s021 + qperpw*(-s001 + s101) - 2*qperpz*s111 + 
                                        qperpy*s121) +
                                    q1y*(-(qperpx*s001) - qperpz*s021 + qperpx*s101 + qperpz*s121))*theta,
                                2*(-(q1x*qperpy*s002) - q1w*qperpz*s002 + 2*q1x*qperpx*s012 + q1x*qperpw*s022 + 
                                    q1w*qperpx*s022 + q1x*qperpy*s102 + q1w*qperpz*s102 - 2*q1x*qperpx*s112 - 
                                    q1x*qperpw*s122 - q1w*qperpx*s122 + 
                                    q1z*(2*qperpz*s012 - qperpy*s022 + qperpw*(-s002 + s102) - 2*qperpz*s112 + 
                                        qperpy*s122) +
                                    q1y*(-(qperpx*s002) - qperpz*s022 + qperpx*s102 + qperpz*s122))*theta);
                
                    c4[1] = DerivativeTerm(0.,
                                -(q1x*qperpy*s000) - q1w*qperpz*s000 + 2*q1x*qperpx*s010 + q1x*qperpw*s020 + 
                                q1w*qperpx*s020 + q1x*qperpy*s100 + q1w*qperpz*s100 - 2*q1x*qperpx*s110 - 
                                q1x*qperpw*s120 - q1w*qperpx*s120 + 2*qperpx*qperpy*s000*theta + 
                                2*qperpw*qperpz*s000*theta + 2*q1x*q1x*s010*theta + 
                                2*q1z*q1z*s010*theta - 2*qperpx*qperpx*s010*theta - 
                                2*qperpz*qperpz*s010*theta + 2*q1w*q1x*s020*theta - 2*qperpw*qperpx*s020*theta + 
                                2*qperpy*qperpz*s020*theta + q1y*
                                (-(qperpx*s000) - qperpz*s020 + qperpx*s100 + qperpz*s120 - 2*q1x*s000*theta) + 
                                q1z*(2*qperpz*s010 - qperpy*s020 + qperpw*(-s000 + s100) - 2*qperpz*s110 + 
                                    qperpy*s120 - 2*q1w*s000*theta - 2*q1y*s020*theta),
                                -(q1x*qperpy*s001) - q1w*qperpz*s001 + 2*q1x*qperpx*s011 + q1x*qperpw*s021 + 
                                q1w*qperpx*s021 + q1x*qperpy*s101 + q1w*qperpz*s101 - 2*q1x*qperpx*s111 - 
                                q1x*qperpw*s121 - q1w*qperpx*s121 + 2*qperpx*qperpy*s001*theta + 
                                2*qperpw*qperpz*s001*theta + 2*q1x*q1x*s011*theta + 
                                2*q1z*q1z*s011*theta - 2*qperpx*qperpx*s011*theta - 
                                2*qperpz*qperpz*s011*theta + 2*q1w*q1x*s021*theta - 2*qperpw*qperpx*s021*theta + 
                                2*qperpy*qperpz*s021*theta + q1y*
                                (-(qperpx*s001) - qperpz*s021 + qperpx*s101 + qperpz*s121 - 2*q1x*s001*theta) + 
                                q1z*(2*qperpz*s011 - qperpy*s021 + qperpw*(-s001 + s101) - 2*qperpz*s111 + 
                                    qperpy*s121 - 2*q1w*s001*theta - 2*q1y*s021*theta),
                                -(q1x*qperpy*s002) - q1w*qperpz*s002 + 2*q1x*qperpx*s012 + q1x*qperpw*s022 + 
                                q1w*qperpx*s022 + q1x*qperpy*s102 + q1w*qperpz*s102 - 2*q1x*qperpx*s112 - 
                                q1x*qperpw*s122 - q1w*qperpx*s122 + 2*qperpx*qperpy*s002*theta + 
                                2*qperpw*qperpz*s002*theta + 2*q1x*q1x*s012*theta + 
                                2*q1z*q1z*s012*theta - 2*qperpx*qperpx*s012*theta - 
                                2*qperpz*qperpz*s012*theta + 2*q1w*q1x*s022*theta - 2*qperpw*qperpx*s022*theta + 
                                2*qperpy*qperpz*s022*theta + q1y*
                                (-(qperpx*s002) - qperpz*s022 + qperpx*s102 + qperpz*s122 - 2*q1x*s002*theta) + 
                                q1z*(2*qperpz*s012 - qperpy*s022 + qperpw*(-s002 + s102) - 2*qperpz*s112 + 
                                    qperpy*s122 - 2*q1w*s002*theta - 2*q1y*s022*theta));
                
                    c5[1] = DerivativeTerm(0.,
                                -2*(qperpx*qperpy*s000 + qperpw*qperpz*s000 + q1z*q1z*s010 - 
                                    qperpx*qperpx*s010 - qperpz*qperpz*s010 - q1y*q1z*s020 - qperpw*qperpx*s020 + 
                                    qperpy*qperpz*s020 - qperpx*qperpy*s100 - qperpw*qperpz*s100 + 
                                    q1w*q1z*(-s000 + s100) + q1x*q1x*(s010 - s110) - q1z*q1z*s110 + 
                                    qperpx*qperpx*s110 + qperpz*qperpz*s110 + 
                                    q1x*(q1y*(-s000 + s100) + q1w*(s020 - s120)) + q1y*q1z*s120 + 
                                    qperpw*qperpx*s120 - qperpy*qperpz*s120)*theta,
                                -2*(qperpx*qperpy*s001 + qperpw*qperpz*s001 + q1z*q1z*s011 - 
                                    qperpx*qperpx*s011 - qperpz*qperpz*s011 - q1y*q1z*s021 - qperpw*qperpx*s021 + 
                                    qperpy*qperpz*s021 - qperpx*qperpy*s101 - qperpw*qperpz*s101 + 
                                    q1w*q1z*(-s001 + s101) + q1x*q1x*(s011 - s111) - q1z*q1z*s111 + 
                                    qperpx*qperpx*s111 + qperpz*qperpz*s111 + 
                                    q1x*(q1y*(-s001 + s101) + q1w*(s021 - s121)) + q1y*q1z*s121 + 
                                    qperpw*qperpx*s121 - qperpy*qperpz*s121)*theta,
                                -2*(qperpx*qperpy*s002 + qperpw*qperpz*s002 + q1z*q1z*s012 - 
                                    qperpx*qperpx*s012 - qperpz*qperpz*s012 - q1y*q1z*s022 - qperpw*qperpx*s022 + 
                                    qperpy*qperpz*s022 - qperpx*qperpy*s102 - qperpw*qperpz*s102 + 
                                    q1w*q1z*(-s002 + s102) + q1x*q1x*(s012 - s112) - q1z*q1z*s112 + 
                                    qperpx*qperpx*s112 + qperpz*qperpz*s112 + 
                                    q1x*(q1y*(-s002 + s102) + q1w*(s022 - s122)) + q1y*q1z*s122 + 
                                    qperpw*qperpx*s122 - qperpy*qperpz*s122)*theta);
                
                    c1[2] = DerivativeTerm(-t0z + t1z,
                                (qperpw*qperpy*s000 - qperpx*qperpz*s000 - q1y*q1z*s010 - 
                                qperpw*qperpx*s010 - qperpy*qperpz*s010 - s020 + q1y*q1y*s020 + 
                                qperpx*qperpx*s020 + qperpy*qperpy*s020 - qperpw*qperpy*s100 + 
                                qperpx*qperpz*s100 + q1x*q1z*(-s000 + s100) + q1y*q1z*s110 + 
                                qperpw*qperpx*s110 + qperpy*qperpz*s110 + 
                                q1w*(q1y*(s000 - s100) + q1x*(-s010 + s110)) + q1x*q1x*(s020 - s120) + 
                                s120 - q1y*q1y*s120 - qperpx*qperpx*s120 - qperpy*qperpy*s120),
                                (qperpw*qperpy*s001 - qperpx*qperpz*s001 - q1y*q1z*s011 - qperpw*qperpx*s011 - 
                                qperpy*qperpz*s011 - s021 + q1y*q1y*s021 + qperpx*qperpx*s021 + 
                                qperpy*qperpy*s021 - qperpw*qperpy*s101 + qperpx*qperpz*s101 + 
                                q1x*q1z*(-s001 + s101) + q1y*q1z*s111 + qperpw*qperpx*s111 + 
                                qperpy*qperpz*s111 + q1w*(q1y*(s001 - s101) + q1x*(-s011 + s111)) + 
                                q1x*q1x*(s021 - s121) + s121 - q1y*q1y*s121 - qperpx*qperpx*s121 - 
                                qperpy*qperpy*s121),
                                (qperpw*qperpy*s002 - qperpx*qperpz*s002 - 
                                q1y*q1z*s012 - qperpw*qperpx*s012 - qperpy*qperpz*s012 - s022 + 
                                q1y*q1y*s022 + qperpx*qperpx*s022 + qperpy*qperpy*s022 - 
                                qperpw*qperpy*s102 + qperpx*qperpz*s102 + q1x*q1z*(-s002 + s102) + 
                                q1y*q1z*s112 + qperpw*qperpx*s112 + qperpy*qperpz*s112 + 
                                q1w*(q1y*(s002 - s102) + q1x*(-s012 + s112)) + q1x*q1x*(s022 - s122) + 
                                s122 - q1y*q1y*s122 - qperpx*qperpx*s122 - qperpy*qperpy*s122));
                
                    c2[2] = DerivativeTerm(0.,
                                (q1w*q1y*s000 - q1x*q1z*s000 - qperpw*qperpy*s000 + qperpx*qperpz*s000 - 
                                q1w*q1x*s010 - q1y*q1z*s010 + qperpw*qperpx*s010 + qperpy*qperpz*s010 + 
                                q1x*q1x*s020 + q1y*q1y*s020 - qperpx*qperpx*s020 - 
                                qperpy*qperpy*s020 - q1w*q1y*s100 + q1x*q1z*s100 + qperpw*qperpy*s100 - 
                                qperpx*qperpz*s100 + q1w*q1x*s110 + q1y*q1z*s110 - qperpw*qperpx*s110 - 
                                qperpy*qperpz*s110 - q1x*q1x*s120 - q1y*q1y*s120 + 
                                qperpx*qperpx*s120 + qperpy*qperpy*s120 - 2*q1y*qperpw*s000*theta + 
                                2*q1z*qperpx*s000*theta - 2*q1w*qperpy*s000*theta + 2*q1x*qperpz*s000*theta + 
                                2*q1x*qperpw*s010*theta + 2*q1w*qperpx*s010*theta + 2*q1z*qperpy*s010*theta + 
                                2*q1y*qperpz*s010*theta - 4*q1x*qperpx*s020*theta - 4*q1y*qperpy*s020*theta),
                                (q1w*q1y*s001 - q1x*q1z*s001 - qperpw*qperpy*s001 + qperpx*qperpz*s001 - 
                                q1w*q1x*s011 - q1y*q1z*s011 + qperpw*qperpx*s011 + qperpy*qperpz*s011 + 
                                q1x*q1x*s021 + q1y*q1y*s021 - qperpx*qperpx*s021 - 
                                qperpy*qperpy*s021 - q1w*q1y*s101 + q1x*q1z*s101 + qperpw*qperpy*s101 - 
                                qperpx*qperpz*s101 + q1w*q1x*s111 + q1y*q1z*s111 - qperpw*qperpx*s111 - 
                                qperpy*qperpz*s111 - q1x*q1x*s121 - q1y*q1y*s121 + 
                                qperpx*qperpx*s121 + qperpy*qperpy*s121 - 2*q1y*qperpw*s001*theta + 
                                2*q1z*qperpx*s001*theta - 2*q1w*qperpy*s001*theta + 2*q1x*qperpz*s001*theta + 
                                2*q1x*qperpw*s011*theta + 2*q1w*qperpx*s011*theta + 2*q1z*qperpy*s011*theta + 
                                2*q1y*qperpz*s011*theta - 4*q1x*qperpx*s021*theta - 4*q1y*qperpy*s021*theta),
                                (q1w*q1y*s002 - q1x*q1z*s002 - qperpw*qperpy*s002 + qperpx*qperpz*s002 - 
                                q1w*q1x*s012 - q1y*q1z*s012 + qperpw*qperpx*s012 + qperpy*qperpz*s012 + 
                                q1x*q1x*s022 + q1y*q1y*s022 - qperpx*qperpx*s022 - 
                                qperpy*qperpy*s022 - q1w*q1y*s102 + q1x*q1z*s102 + qperpw*qperpy*s102 - 
                                qperpx*qperpz*s102 + q1w*q1x*s112 + q1y*q1z*s112 - qperpw*qperpx*s112 - 
                                qperpy*qperpz*s112 - q1x*q1x*s122 - q1y*q1y*s122 + 
                                qperpx*qperpx*s122 + qperpy*qperpy*s122 - 2*q1y*qperpw*s002*theta + 
                                2*q1z*qperpx*s002*theta - 2*q1w*qperpy*s002*theta + 2*q1x*qperpz*s002*theta + 
                                2*q1x*qperpw*s012*theta + 2*q1w*qperpx*s012*theta + 2*q1z*qperpy*s012*theta + 
                                2*q1y*qperpz*s012*theta - 4*q1x*qperpx*s022*theta - 4*q1y*qperpy*s022*theta));
                
                    c3[2] = DerivativeTerm(0.,
                                -2*(-(q1w*qperpy*s000) + q1x*qperpz*s000 + q1x*qperpw*s010 + q1w*qperpx*s010 - 
                                    2*q1x*qperpx*s020 + q1w*qperpy*s100 - q1x*qperpz*s100 - q1x*qperpw*s110 - 
                                    q1w*qperpx*s110 +
                                    q1z*(qperpx*s000 + qperpy*s010 - qperpx*s100 - qperpy*s110) + 
                                    2*q1x*qperpx*s120 +
                                    q1y*(qperpz*s010 - 2*qperpy*s020 + qperpw*(-s000 + s100) - 
                                        qperpz*s110 + 2*qperpy*s120))*theta,
                                -2*(-(q1w*qperpy*s001) + q1x*qperpz*s001 + q1x*qperpw*s011 + q1w*qperpx*s011 - 
                                    2*q1x*qperpx*s021 + q1w*qperpy*s101 - q1x*qperpz*s101 - q1x*qperpw*s111 - 
                                    q1w*qperpx*s111 +
                                    q1z*(qperpx*s001 + qperpy*s011 - qperpx*s101 - qperpy*s111) + 
                                    2*q1x*qperpx*s121 +
                                    q1y*(qperpz*s011 - 2*qperpy*s021 + qperpw*(-s001 + s101) - 
                                        qperpz*s111 + 2*qperpy*s121))*theta,
                                -2*(-(q1w*qperpy*s002) + q1x*qperpz*s002 + q1x*qperpw*s012 + q1w*qperpx*s012 - 
                                    2*q1x*qperpx*s022 + q1w*qperpy*s102 - q1x*qperpz*s102 - q1x*qperpw*s112 - 
                                    q1w*qperpx*s112 +
                                    q1z*(qperpx*s002 + qperpy*s012 - qperpx*s102 - qperpy*s112) + 
                                    2*q1x*qperpx*s122 +
                                    q1y*(qperpz*s012 - 2*qperpy*s022 + qperpw*(-s002 + s102) - 
                                        qperpz*s112 + 2*qperpy*s122))*theta);
                
                    c4[2] = DerivativeTerm(0.,
                                q1w*qperpy*s000 - q1x*qperpz*s000 - q1x*qperpw*s010 - q1w*qperpx*s010 + 
                                2*q1x*qperpx*s020 - q1w*qperpy*s100 + q1x*qperpz*s100 + q1x*qperpw*s110 + 
                                q1w*qperpx*s110 - 2*q1x*qperpx*s120 - 2*qperpw*qperpy*s000*theta + 
                                2*qperpx*qperpz*s000*theta - 2*q1w*q1x*s010*theta + 2*qperpw*qperpx*s010*theta + 
                                2*qperpy*qperpz*s010*theta + 2*q1x*q1x*s020*theta + 
                                2*q1y*q1y*s020*theta - 2*qperpx*qperpx*s020*theta - 
                                2*qperpy*qperpy*s020*theta + q1z*
                                (-(qperpx*s000) - qperpy*s010 + qperpx*s100 + qperpy*s110 - 2*q1x*s000*theta) + 
                                q1y*(-(qperpz*s010) + 2*qperpy*s020 + qperpw*(s000 - s100) + qperpz*s110 - 
                                    2*qperpy*s120 + 2*q1w*s000*theta - 2*q1z*s010*theta),
                                q1w*qperpy*s001 - q1x*qperpz*s001 - q1x*qperpw*s011 - q1w*qperpx*s011 + 
                                2*q1x*qperpx*s021 - q1w*qperpy*s101 + q1x*qperpz*s101 + q1x*qperpw*s111 + 
                                q1w*qperpx*s111 - 2*q1x*qperpx*s121 - 2*qperpw*qperpy*s001*theta + 
                                2*qperpx*qperpz*s001*theta - 2*q1w*q1x*s011*theta + 2*qperpw*qperpx*s011*theta + 
                                2*qperpy*qperpz*s011*theta + 2*q1x*q1x*s021*theta + 
                                2*q1y*q1y*s021*theta - 2*qperpx*qperpx*s021*theta - 
                                2*qperpy*qperpy*s021*theta + q1z*
                                (-(qperpx*s001) - qperpy*s011 + qperpx*s101 + qperpy*s111 - 2*q1x*s001*theta) + 
                                q1y*(-(qperpz*s011) + 2*qperpy*s021 + qperpw*(s001 - s101) + qperpz*s111 - 
                                    2*qperpy*s121 + 2*q1w*s001*theta - 2*q1z*s011*theta),
                                q1w*qperpy*s002 - q1x*qperpz*s002 - q1x*qperpw*s012 - q1w*qperpx*s012 + 
                                2*q1x*qperpx*s022 - q1w*qperpy*s102 + q1x*qperpz*s102 + q1x*qperpw*s112 + 
                                q1w*qperpx*s112 - 2*q1x*qperpx*s122 - 2*qperpw*qperpy*s002*theta + 
                                2*qperpx*qperpz*s002*theta - 2*q1w*q1x*s012*theta + 2*qperpw*qperpx*s012*theta + 
                                2*qperpy*qperpz*s012*theta + 2*q1x*q1x*s022*theta + 
                                2*q1y*q1y*s022*theta - 2*qperpx*qperpx*s022*theta - 
                                2*qperpy*qperpy*s022*theta + q1z*
                                (-(qperpx*s002) - qperpy*s012 + qperpx*s102 + qperpy*s112 - 2*q1x*s002*theta) + 
                                q1y*(-(qperpz*s012) + 2*qperpy*s022 + qperpw*(s002 - s102) + qperpz*s112 - 
                                    2*qperpy*s122 + 2*q1w*s002*theta - 2*q1z*s012*theta));
                
                    c5[2] = DerivativeTerm(0.,
                                2*(qperpw*qperpy*s000 - qperpx*qperpz*s000 + q1y*q1z*s010 - qperpw*qperpx*s010 - 
                                    qperpy*qperpz*s010 - q1y*q1y*s020 + qperpx*qperpx*s020 + 
                                    qperpy*qperpy*s020 + q1x*q1z*(s000 - s100) - qperpw*qperpy*s100 + 
                                    qperpx*qperpz*s100 + q1w*(q1y*(-s000 + s100) + q1x*(s010 - s110)) - 
                                    q1y*q1z*s110 + qperpw*qperpx*s110 + qperpy*qperpz*s110 + q1y*q1y*s120 - 
                                    qperpx*qperpx*s120 - qperpy*qperpy*s120 + q1x*q1x*(-s020 + s120))*theta,
                                2*(qperpw*qperpy*s001 - qperpx*qperpz*s001 + q1y*q1z*s011 - qperpw*qperpx*s011 - 
                                    qperpy*qperpz*s011 - q1y*q1y*s021 + qperpx*qperpx*s021 + 
                                    qperpy*qperpy*s021 + q1x*q1z*(s001 - s101) - qperpw*qperpy*s101 + 
                                    qperpx*qperpz*s101 + q1w*(q1y*(-s001 + s101) + q1x*(s011 - s111)) - 
                                    q1y*q1z*s111 + qperpw*qperpx*s111 + qperpy*qperpz*s111 + q1y*q1y*s121 - 
                                    qperpx*qperpx*s121 - qperpy*qperpy*s121 + q1x*q1x*(-s021 + s121))*theta,
                                2*(qperpw*qperpy*s002 - qperpx*qperpz*s002 + q1y*q1z*s012 - qperpw*qperpx*s012 - 
                                    qperpy*qperpz*s012 - q1y*q1y*s022 + qperpx*qperpx*s022 + 
                                    qperpy*qperpy*s022 + q1x*q1z*(s002 - s102) - qperpw*qperpy*s102 + 
                                    qperpx*qperpz*s102 + q1w*(q1y*(-s002 + s102) + q1x*(s012 - s112)) - 
                                    q1y*q1z*s112 + qperpw*qperpx*s112 + qperpy*qperpz*s112 + q1y*q1y*s122 - 
                                    qperpx*qperpx*s122 - qperpy*qperpy*s122 + q1x*q1x*(-s022 + s122))*theta);
                }
            }

            static void decompose(const Mat4& m, Vec3*T, Quaternion* R, Mat4* S)
            {
                T->x = m[0][3];
                T->y = m[1][3];
                T->z = m[2][3];
            
                Mat4 newM = m;
                for (std::size_t i = 0; i < 3; ++i)
                    newM[i][3] = newM[3][i] = 0.f;

                newM[3][3] = 1.f;

                float norm;
                std::size_t count = 0;
                Mat4 r = m;
                do {
                    Mat4 rnext;
                    Mat4 rit = ::inverse(::transpose(r));
                    for (std::size_t i = 0; i < 4; ++i)
                        for (std::size_t j = 0; j < 4; ++j)
                            rnext[i][j] = 0.5f * (r[i][j] + rit[i][j]);
                    norm = 0;
                    for (std::size_t i = 0; i < 3; ++i) 
                    {
                        float n =   std::abs(r[i][0] - rnext[i][0]) + 
                                    std::abs(r[i][1] - rnext[i][1]) + 
                                    std::abs(r[i][2] - rnext[i][2]);
                        norm    =   std::max(norm, n);
                    }

                    r = rnext;
                } 
                while (++count < 100 && norm > .0001);

                *R = Quaternion(Transform(r));
                *S = ::inverse(r) * newM;
            }

            void interpolate(const float time, Transform* trans) const
            {
                if (!actuallyAnimated || time <= startTime) { 
                    *trans = *startTransform;
                    return; 
                }
                if (time >= endTime) { 
                    *trans = *endTransform;
                    return; 
                }

                float dt = (time - startTime) / (endTime - startTime);

                Vec3 transform = ::lerp(dt, t[0], t[1]);
                Quaternion rotate = slerp(dt, r[0], r[1]);
                Mat4 scale;

                for (int i = 0; i < 3; ++i)
                    for (int j = 0; j < 3; ++j)
                        scale[i][j] = lerp(dt, s[0][i][j], s[1][i][j]);

                *trans = Transform::translate(transform) (toTransform(rotate)(Transform(scale)));
            }

            Ray operator()(const Ray &r) const
            {
                Transform t;
                interpolate(r.time, &t); 
                return t(r);
            }
            // RayDifferential operator()(const RayDifferential &ray) const
            // {
            //     Transform transform;
            //     interpolate(ray.time, &t);

            //     RayDifferential tray(transform(ray)); 
                
            //     tray.hasDifferentials = ray.hasDifferentials;
                
            //     if (tray.hasDifferentials) {
            //         tray.rxOrigin    = t(ray.rxOrigin);
            //         tray.ryOrigin    = t(ray.ryOrigin);
            //         tray.rxDirection = t(ray.rxDirection);
            //         tray.ryDirection = t(ray.ryDirection);
            //     }

            //     return tray;
            // }
            Point3 operator()(float time, const Point3 &p) const
            {
                if (!actuallyAnimated || time <= startTime) 
                    return (*startTransform)(p);
                
                if (time >= endTime) 
                    return (*endTransform)(p);

                Transform t;
                interpolate(time, &t);
                return t(p); 
            }
            Vec3 operator()(float time, const Vec3 &v) const
            {
                if (!actuallyAnimated || time <= startTime) 
                    return (*startTransform)(v);
                
                if (time >= endTime) 
                    return (*endTransform)(v);

                Transform t;
                interpolate(time, &t);
                return t(v); 
            }

            Bounds3f motionBounds(const Bounds3f& b) const 
            {
                if (!actuallyAnimated) 
                    return (*startTransform)(b);
                
                if (hasRotation == false) 
                    return boundUnion((*startTransform)(b),(*endTransform)(b));

                Bounds3f bound;
                for(std::size_t i{0}; i < 8; ++i)
                    bound = boundUnion(bound, boundPointMotion(b.corner(i)));
                return bound;
            }

            Bounds3f boundPointMotion(const Point3& p) const 
            {
                Bounds3f bound((*startTransform)(p),(*endTransform)(p));
                float cosTheta = ::dot(r[0], r[1]);
                float theta = std::acos(clamp(cosTheta, -1, 1));
                for(std::size_t i{0}; i < 3; ++i)
                {
                    float zeros[4];
                    std::size_t nzeros{0};
                    intervalFindZeros(c1[i].eval(p), c2[i].eval(p), c3[i].eval(p), c4[i].eval(p), c5[i].eval(p), theta, Interval(0., 1), zeros, &nzeros);
            
                    for(std::size_t j{0}; j < nzeros; ++j)
                    {
                        Point3 pz = (*this)(::lerp(zeros[j], startTime, endTime), p);
                        bound = boundUnion(bound, pz);
                    }
                }
                return bound;
            }
    };
}


#endif