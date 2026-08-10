#ifndef SSMATH_3_HPP 
#define SSMATH_3_HPP

#include "pixel.hpp"
#include "matrix.hpp"
#include "ssmath3/foward.hpp"
#include "vector.hpp"
#include "normal.hpp"
#include "point.hpp"
#include <cmath>

namespace ssmath3 
{
  /// Matrix operations:

    /// Matrix 4x4 operations:

    template<typename T>
    constexpr Matrix<T, 4>::Matrix(const Vector<T, 3>& v1, const Vector<T, 3> v2, const Vector<T, 3>& v3) noexcept
    {
        data[0][0] = v1.x             ; data[0][1] = v2.x             ; data[0][2] = v3.x             ; data[0][3] = static_cast<T>(0);
        data[1][0] = v1.y             ; data[1][1] = v2.y             ; data[1][2] = v3.y             ; data[1][3] = static_cast<T>(0);
        data[2][0] = v1.z             ; data[2][1] = v2.z             ; data[2][2] = v3.z             ; data[2][3] = static_cast<T>(0);
        data[3][0] = static_cast<T>(0); data[3][1] = static_cast<T>(0); data[3][2] = static_cast<T>(0); data[3][3] = static_cast<T>(1);
    };

    template<typename T>
    constexpr Matrix<T, 4>::Matrix(const Matrix<T, 2>& m) noexcept
    {
      data[0][0] = m[0][0]             ; data[0][1] = m[0][1]          ; data[0][2] = static_cast<T>(0); data[0][3] = static_cast<T>(0);
      data[1][0] = m[1][0]             ; data[1][1] = m[1][1]          ; data[1][2] = static_cast<T>(0); data[1][3] = static_cast<T>(0);
      data[2][0] = static_cast<T>(0)   ; data[2][1] = static_cast<T>(0); data[2][2] = static_cast<T>(1); data[2][3] = static_cast<T>(0);
      data[3][0] = static_cast<T>(0)   ; data[3][1] = static_cast<T>(0); data[3][2] = static_cast<T>(0); data[3][3] = static_cast<T>(1);

    }

    template<typename T>
    constexpr Matrix<T, 4>::Matrix(const Matrix<T, 3>& m) noexcept
    {

      data[0][0] = m[0][0]             ; data[0][1] = m[0][1]          ; data[0][2] = m[0][2]          ; data[0][3] = static_cast<T>(0);
      data[1][0] = m[1][0]             ; data[1][1] = m[1][1]          ; data[1][2] = m[1][2]          ; data[1][3] = static_cast<T>(0);
      data[2][0] = m[2][0]             ; data[2][1] = m[2][1]          ; data[2][2] = m[2][2]          ; data[2][3] = static_cast<T>(0);
      data[3][0] = static_cast<T>(0)   ; data[3][1] = static_cast<T>(0); data[3][2] = static_cast<T>(0); data[3][3] = static_cast<T>(1);

    }
    
    template<typename T>
    constexpr Vector<T, 4> Matrix<T, 4>::operator*(const Vector<T, 4>& v) const noexcept 
    {
        Vector<T, 4> res;

        for (std::size_t i{0}; i < 4; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 4; ++j)
            {
                sum += data[i][j] * v[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };
    template<typename T>
    constexpr Point<T, 4> Matrix<T, 4>::operator*(const Point<T, 4>& p) const noexcept 
    {
        Point<T, 4> res;

        for (std::size_t i{0}; i < 4; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 4; ++j)
            {
                sum += data[i][j] * p[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };
    template<typename T>
    constexpr Normal<T, 4> Matrix<T, 4>::operator*(const Normal<T, 4>& n) const noexcept 
    {
        Normal<T, 4> res;

        for (std::size_t i{0}; i < 4; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 4; ++j)
            {
                sum += data[i][j] * n[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };

    template<typename T>
    inline T det(const Matrix<T, 4>& m) noexcept
    {
      T s0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
      T s1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
      T s2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
      T s3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
      T s4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
      T s5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

      T c0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
      T c1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
      T c2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
      T c3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
      T c4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
      T c5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

      return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    }

    template<typename T>
    inline Matrix<T, 4> transpose(const Matrix<T, 4>& m) noexcept
    {
      Matrix<T, 4> res;

      for(std::size_t i{0}; i < 4; ++i)
      {
        for(std::size_t j{0}; j < 4; ++j)
        {
          res[i][j] = m[j][i];
        }
      }

      return res;
    }

    template<typename T>
    inline Matrix<T, 4> inverse(const Matrix<T, 4>& m) noexcept
    {
        T s0 = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        T s1 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
        T s2 = m[0][0] * m[1][3] - m[0][3] * m[1][0];
        T s3 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
        T s4 = m[0][1] * m[1][3] - m[0][3] * m[1][1];
        T s5 = m[0][2] * m[1][3] - m[0][3] * m[1][2];

        T c0 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
        T c1 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
        T c2 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
        T c3 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
        T c4 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
        T c5 = m[2][2] * m[3][3] - m[2][3] * m[3][2];

        T d  = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
        
        assert(d != T(0));
        

        Matrix<T, 4> inv;

        inv[0][0] = ( m[1][1] * c5 - m[1][2] * c4 + m[1][3] * c3) / d;
        inv[0][1] = (-m[0][1] * c5 + m[0][2] * c4 - m[0][3] * c3) / d;
        inv[0][2] = ( m[3][1] * s5 - m[3][2] * s4 + m[3][3] * s3) / d;
        inv[0][3] = (-m[2][1] * s5 + m[2][2] * s4 - m[2][3] * s3) / d;

        inv[1][0] = (m[1][0] * c5 + m[1][2] * c2 - m[1][3] * c1) / d;
        inv[1][1] = (m[0][0] * c5 - m[0][2] * c2 + m[0][3] * c1) / d;
        inv[1][2] = (m[3][0] * s5 + m[3][2] * s2 - m[3][3] * s1) / d;
        inv[1][3] = (m[2][0] * s5 - m[2][2] * s2 + m[2][3] * s1) / d;

        inv[2][0] = ( m[1][0] * c4 - m[1][1] * c2 + m[1][3] * c0) / d;
        inv[2][1] = (-m[0][0] * c4 + m[0][1] * c2 - m[0][3] * c0) / d;
        inv[2][2] = ( m[3][0] * s4 - m[3][1] * s2 + m[3][3] * s0) / d;
        inv[2][3] = (-m[2][0] * s4 + m[2][1] * s2 - m[2][3] * s0) / d;

        inv[3][0] = (-m[1][0] * c3 + m[1][1] * c1 - m[1][2] * c0) / d;
        inv[3][1] = ( m[0][0] * c3 - m[0][1] * c1 + m[0][2] * c0) / d;
        inv[3][2] = (-m[3][0] * s3 + m[3][1] * s1 - m[3][2] * s0) / d;
        inv[3][3] = ( m[2][0] * s3 - m[2][1] * s1 + m[2][2] * s0) / d;

        return inv;

    };
    template<typename T>
    inline Matrix<T, 4> translate(const Point<T, 3>& offset) noexcept
    {
        Matrix<T, 4> res; 
        res[0][3] = offset.x;
        res[1][3] = offset.y;
        res[2][3] = offset.z;
        return res;
    }

    template<typename T>
    inline Matrix<T, 4> scale(const Vector<T, 3>& s) noexcept
    {
        Matrix<T, 4> res;
        res[0][0] = s.x;
        res[1][1] = s.y;
        res[2][2] = s.z;
        return res;
    }

    template<typename T>
    inline Matrix<T, 4> rotateX(float thetaRadians) noexcept
    {
        Matrix<T, 4> res;
        float c = std::cos(thetaRadians);
        float s = std::sin(thetaRadians);
        res[1][1] = c; res[1][2] = -s;
        res[2][1] = s; res[2][2] = c;
        return res;
    }

    template<typename T>
    inline Matrix<T, 4> rotateY(float thetaRadians) noexcept
    {
        Matrix<T, 4> res;
        float c = std::cos(thetaRadians);
        float s = std::sin(thetaRadians);
        res[0][0] = c;  res[0][2] = s;
        res[2][0] = -s; res[2][2] = c;
        return res;
    }

    template<typename T>
    inline Matrix<T, 4> rotateZ(float thetaRadians) noexcept
    {
        Matrix<T, 4> res;
        float c = std::cos(thetaRadians);
        float s = std::sin(thetaRadians);
        res[0][0] = c; res[0][1] = -s;
        res[1][0] = s; res[1][1] = c;
        return res;
    }
    template<typename T>
    inline Matrix<T, 4> rotate(float thetaRadians, const Point<T, 3>& axis) noexcept
    {
        T c = std::cos(thetaRadians);
        T s = std::sin( thetaRadians);
        T c1 = 1 - c;

        Vector<T, 3> k = normalize(Vector<T, 3>(axis));

        T kx = k.x;
        T ky = k.y;
        T kz = k.z; 

         return Matrix<T, 4>(   
                              c + c1 * kx * kx,      c1 * kx * ky - s * kz, c1 * kx * kz + s * ky, 0, 
                              c1 * kx * ky + s * kz, c + c1 * ky * ky,      c1 * ky * kz - s * kx, 0,
                              c1 * kx * kz - s * ky, c1 * ky * kz + s * kx, c + c1 * kz * kz,      0,
                              0,                     0,                     0,                     1
                            );
    }

    /// Matrix 3x3 operations
    template<typename T>
    constexpr Matrix<T, 3>::Matrix(const Vector<T, 2> v1, const Vector<T, 2> v2) noexcept
    {
        data[0][0] = v1.x             ; data[0][1] = v2.x             ; data[0][2] = static_cast<T>(0); 
        data[1][0] = v1.y             ; data[1][1] = v2.y             ; data[1][2] = static_cast<T>(0); 
        data[2][0] = static_cast<T>(0); data[2][1] = static_cast<T>(0); data[2][2] = static_cast<T>(1);
    };


    template<typename T>
    constexpr Matrix<T, 3>::Matrix(const Vector<T, 3>& v1, const Vector<T, 3> v2, const Vector<T, 3>& v3) noexcept
    {
        data[0][0] = v1.x; data[0][1] = v2.x; data[0][2] = v3.x;
        data[1][0] = v1.y; data[1][1] = v2.y; data[1][2] = v3.y;
        data[2][0] = v1.z; data[2][1] = v2.z; data[2][2] = v3.z;
    };


    template<typename T>
    constexpr Matrix<T, 3>::Matrix(const Matrix<T, 2>& m) noexcept
    {
      data[0][0] = m[0][0]          ; data[0][1] = m[0][1]          ; data[0][2] = static_cast<T>(0);
      data[1][0] = m[1][0]          ; data[1][1] = m[1][1]          ; data[1][2] = static_cast<T>(0);
      data[2][0] = static_cast<T>(0); data[2][1] = static_cast<T>(0); data[2][2] = static_cast<T>(1);
    };
    template<typename T>
    constexpr Matrix<T, 3>::Matrix(const Matrix<T, 4>& m) noexcept
    {
      data[0][0] = m[0][0]; data[0][1] = m[0][1]; data[0][2] = m[0][2];
      data[1][0] = m[1][0]; data[1][1] = m[1][1]; data[1][2] = m[1][2];
      data[2][0] = m[2][0]; data[2][1] = m[2][1]; data[2][2] = m[2][2];
    };

    template<typename T>
    constexpr Vector<T, 3> Matrix<T, 3>::operator*(const Vector<T, 3>& v) const noexcept 
    {
        Vector<T, 3> res;

        for (std::size_t i{0}; i < 3; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 3; ++j)
            {
                sum += data[i][j] * v[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };
    template<typename T>
    constexpr Point<T, 3> Matrix<T, 3>::operator*(const Point<T, 3>& p) const noexcept 
    {
        Point<T, 3> res;

        for (std::size_t i{0}; i < 3; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 3; ++j)
            {
                sum += data[i][j] * p[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };

    template<typename T>
    constexpr Normal<T, 3> Matrix<T, 3>::operator*(const Normal<T, 3>& n) const noexcept 
    {
        Normal<T, 3> res;

        for (std::size_t i{0}; i < 3; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 3; ++j)
            {
                sum += data[i][j] * n[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };

    template<typename T>
    inline T det(const Matrix<T, 3>& m) noexcept
    {
      return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
             m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
             m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    };
    template<typename T>
    inline Matrix<T, 3> transpose(const Matrix<T, 3>& m) noexcept
    {
      Matrix<T, 3> res;
      for(std::size_t i{0}; i < 3; ++i)
      {
        for(std::size_t j{0}; j < 3; ++j)
        {
          res[i][j] = m[j][i];
        }
      }
      return res;
    };

    template<typename T>
    inline Matrix<T, 3> inverse(const Matrix<T, 3>& m) noexcept
    {
      T d = det(m);

      assert(d != static_cast<T>(0));

      Matrix<T, 3> res;

      res[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / d;
      res[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / d;
      res[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / d;

      res[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / d;
      res[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / d;
      res[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / d;

      res[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / d;
      res[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / d;
      res[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / d;
      
      return res;
    };

    /// Matrix 2x2 operations
    template<typename T>
    constexpr Matrix<T, 2>::Matrix(const Vector<T, 2>& v1, const Vector<T, 2> v2) noexcept
    {
        data[0][0] = v1.x; data[0][1] = v2.x;
        data[1][0] = v1.y; data[1][1] = v2.y;
    };

    template<typename T>
    constexpr Matrix<T, 2>::Matrix(const Matrix<T, 3>& m) noexcept
    {
      data[0][0] = m[0][0]; data[0][1] = m[0][1];
      data[1][0] = m[1][0]; data[1][1] = m[1][1];
    }

    template<typename T>
    constexpr Vector<T, 2> Matrix<T, 2>::operator*(const Vector<T, 2>& v) const noexcept 
    {
        Vector<T, 2> res;

        for (std::size_t i{0}; i < 2; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 2; ++j)
            {
                sum += data[i][j] * v[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };

    template<typename T>
    constexpr Point<T, 2> Matrix<T, 2>::operator*(const Point<T, 2>& p) const noexcept 
    {
        Point<T, 2> res;

        for (std::size_t i{0}; i < 2; ++i) 
        {
            T sum{0};

            for(std::size_t j{0}; j < 2; ++j)
            {
                sum += data[i][j] * p[j];
            }
            
            res[i] = sum;
        }
        
        return res;
    };

    template<typename T>
    constexpr T det(const Matrix<T, 2>& m) noexcept
    {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    };

    template<typename T>
    constexpr Matrix<T, 2> transpose(const Matrix<T, 2>& m) noexcept
    {
        Matrix<T, 2> res;

        for(std::size_t i{0}; i < 2; ++i)
        {
            for(std::size_t j{0}; j < 2; ++j)
            {
                res[i][j] = m[j][i];
            }
        }

        return res;
    };

    template<typename T>
    constexpr Matrix<T, 2> inverse(const Matrix<T, 2>& m) noexcept
    {
        Matrix<T, 2> t{m[1][1], -m[0][1], -m[1][0], m[0][0]};

        T d = det(m);

        assert(d != static_cast<T>(0));

        Matrix<T, 2> res = t;

        res *= 1/(d);
        
        return res;
    };


  /// Vector operations
    template<typename T, std::size_t L>
    inline Vector<T, L> operator*(T t, const Vector<T, L>& v) noexcept
    {
        return v * t;
    }

    template<typename T, std::size_t L>
    inline Vector<T, L> normalize(const Vector<T, L>& v) noexcept
    {
      T len = length(v);
      return len > 0 ? v / len : v; 
    }

    template<typename T, std::size_t L>
    inline T length(const Vector<T, L>& v) noexcept
    {
      return std::sqrt(sqrLength(v));
    }
    template<typename T, std::size_t L>
    inline T sqrLength(const Vector<T, L>& v) noexcept
    {
      return dot(v, v);
    }
    

    /// Vector 4 operations

    template<typename T>
    inline Vector<T, 4> min(const Vector<T, 4>& v1, const Vector<T, 4>& v2)
    {
      return Vector<T, 4>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w));
    }
    template<typename T>
    inline Vector<T, 4> max(const Vector<T, 4>& v1, const Vector<T, 4>& v2)
    {
      return Vector<T, 4>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w));
    }

    template<typename T>
    constexpr Vector<T, 4>::Vector(const Point<T, 4>& p) noexcept : x(p.x), y(p.y), z(p.z), w(p.w) {};
    template<typename T>
    constexpr Vector<T, 4>::Vector(const Normal<T, 4>& n) noexcept : x(n.x), y(n.y), z(n.z), w(n.w) {};
    template<typename T>
    constexpr Vector<T, 4>::Vector(const Vector<T, 3>& v) noexcept : x(v.x), y(v.y), z(v.z), w(static_cast<T>(0)) {};

    template<typename T>
    constexpr Point<T, 4> operator+(const Vector<T, 4>& v, const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(v.x + p.x, v.y + p.y, v.z + p.z, v.w + p.w);
    }
    template<typename T>
    constexpr Point<T, 4> operator-(const Vector<T, 4>& v, const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(v.x - p.x, v.y - p.y, v.z - p.z, v.w - p.w);
    }
    
    template<typename T>
    constexpr Vector<T, 4> operator+(const Vector<T, 4>& v, const Normal<T, 4>& n) noexcept
    {
      return Vector<T, 4>(v.x + n.x, v.y + n.y, v.z + n.z, v.w + n.w);
    }
    template<typename T>
    constexpr Vector<T, 4> operator-(const Vector<T, 4>& v, const Normal<T, 4>& n) noexcept
    {
      return Vector<T, 4>(v.x - n.x, v.y - n.y, v.z - n.z, v.w - n.w);
    }
    template<typename T>
    constexpr Vector<T, 4>& operator+=(Vector<T, 4>& v, const Normal<T, 4>& n) noexcept
    {
      v.x += n.x;
      v.y += n.y;
      v.z += n.z;
      v.w += n.w;
      return v;
    }
    template<typename T>
    constexpr Vector<T, 4> operator-=(Vector<T, 4>& v, const Normal<T, 4>& n) noexcept
    {
      v.x -= n.x;
      v.y -= n.y;
      v.z -= n.z;
      v.w -= n.w;
      return v;
    }

    template<typename T>
    inline Vector<T, 4> floor(const Vector<T, 4>& v) noexcept
    {
      return Vector<T, 4>(static_cast<T>(std::floor(v.x)), static_cast<T>(std::floor(v.y)), static_cast<T>(std::floor(v.z)), static_cast<T>(std::floor(v.w)));
    }
    template<typename T>
    inline Vector<T, 4> ceil(const Vector<T, 4>& v) noexcept
    {
      return Vector<T, 4>(static_cast<T>(std::ceil(v.x)), static_cast<T>(std::ceil(v.y)), static_cast<T>(std::ceil(v.z)), static_cast<T>(std::ceil(v.w)));
    }
    template<typename T>
    inline Vector<T, 4> round(const Vector<T, 4>& v) noexcept
    {
      return Vector<T, 4>(static_cast<T>(std::round(v.x)), static_cast<T>(std::round(v.y)), static_cast<T>(std::round(v.z)), static_cast<T>(std::round(v.w)));
    }
    template<typename T>
    inline Vector<T, 4> abs(const Vector<T, 4>& v) noexcept
    {
      return Vector<T, 4>(static_cast<T>(std::abs(v.x)), static_cast<T>(std::abs(v.y)), static_cast<T>(std::abs(v.z)), static_cast<T>(std::abs(v.w)));
    }

    template<typename T>
    inline T dot(const Vector<T, 4>& v1, const Vector<T, 4>& v2) noexcept
    {
      return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
    }
    template<typename T>
    inline T dot(const Vector<T, 4>& v, const Normal<T, 4>& n) noexcept
    {
      return v[0] * n[0] + v[1] * n[1] + v[2] * n[2] + v[3] * n[3];
    }
    template<typename T>
    inline bool hasNaN(const Vector<T, 4>& v) noexcept
    {
      return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w);
    }

    /// Vector 3 operations
    template<typename T>
    constexpr Vector<T, 3>::Vector(const Point<T, 3> & p) noexcept : x(p.x), y(p.y), z(p.z) {};
    template<typename T>
    constexpr Vector<T, 3>::Vector(const Normal<T, 3>& n) noexcept : x(n.x), y(n.y), z(n.z) {assert(!hasNaN(n));};
    template<typename T>
    constexpr Vector<T, 3>::Vector(const Vector<T, 4>& v) noexcept : x(v.x), y(v.y), z(v.z) {};
    template<typename T>
    constexpr Vector<T, 3>::Vector(const Vector<T, 2>& v) noexcept : x(v.x), y(v.y), z(0)   {};

    template<typename T>
    template<typename U>
    constexpr Vector<T, 3>::Vector(const Point<U, 3>& p)noexcept : x(static_cast<T>(p.x)), y(static_cast<T>(p.y)), z(static_cast<T>(p.z)) {};
    template<typename T>
    template<typename U>
    constexpr Vector<T, 3>::Vector(const Vector<U, 3>& v)noexcept : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {};
    template<typename T>
    template<typename U>
    constexpr Vector<T, 3>::Vector(const Normal<U, 3>& n)noexcept : x(static_cast<T>(n.x)), y(static_cast<T>(n.y)), z(static_cast<T>(n.z)) {};


    template<typename T>
    constexpr Point<T, 3> operator+(const Vector<T, 3>& v, const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(v.x + p.x, v.y + p.y, v.z + p.z);
    }
    template<typename T>
    constexpr Point<T, 3> operator-(const Vector<T, 3>& v, const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(v.x - p.x, v.y - p.y, v.z - p.z);
    }

    template<typename T>
    constexpr Vector<T, 3> operator+(const Vector<T, 3>& v, const Normal<T, 3>& n) noexcept
    {
      return Vector<T, 3>(v.x + n.x, v.y + n.y, v.z + n.z);
    }
    template<typename T>
    constexpr Vector<T, 3> operator-(const Vector<T, 3>& v, const Normal<T, 3>& n) noexcept
    {
      return Vector<T, 3>(v.x - n.x, v.y - n.y, v.z - n.z);
    }

    template<typename T>
    inline Vector<T, 3> floor(const Vector<T, 3>& v) noexcept
    {
      return Vector<T, 3>(static_cast<T>(std::floor(v.x)), static_cast<T>(std::floor(v.y)), static_cast<T>(std::floor(v.z)));
    }
    template<typename T>
    inline Vector<T, 3> ceil(const Vector<T, 3>& v) noexcept
    {
      return Vector<T, 3>(static_cast<T>(std::ceil(v.x)), static_cast<T>(std::ceil(v.y)), static_cast<T>(std::ceil(v.z)));
    }
    template<typename T>
    inline Vector<T, 3> round(const Vector<T, 3>& v) noexcept
    {
      return Vector<T, 3>(static_cast<T>(std::round(v.x)), static_cast<T>(std::round(v.y)), static_cast<T>(std::round(v.z)));
    }
    template<typename T>
    inline Vector<T, 3> abs(const Vector<T, 3>& v) noexcept
    {
      return Vector<T, 3>(static_cast<T>(std::abs(v.x)), static_cast<T>(std::abs(v.y)), static_cast<T>(std::abs(v.z)));
    }
    template<typename T>
    inline Vector<T, 3> min(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
    {
      return Vector<T, 3>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
    }
    template<typename T>
    inline Vector<T, 3> max(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
    {
      return Vector<T, 3>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
    }

    template<typename T>
    constexpr T dot(const Vector<T, 3>& v1, const Vector<T, 3>& v2) noexcept
    {
      return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
    template<typename T>
    constexpr T dot(const Vector<T, 3>& v, const Normal<T, 3>& n) noexcept
    {
      return v.x * n.x + v.y * n.y + v.z * n.z;
    }
    template<typename T>
    constexpr Vector<T, 3> cross(const Vector<T, 3>& v1, const Vector<T, 3>& v2) noexcept
    {
      return Vector<T, 3>(v1.y * v2.z - v1.z * v2.y,
                          v1.z * v2.x - v1.x * v2.z,
                          v1.x * v2.y - v1.y * v2.x);
    }
    template<typename T>
    constexpr Vector<T, 3> cross(const Vector<T, 3>& v, const Normal<T, 3>& n) noexcept
    {
      return Vector<T, 3>(v.y * n.z - v.z * n.y,
                          v.z * n.x - v.x * n.z,
                          v.x * n.y - v.y * n.x);
    }

    template<typename T>
    inline Vector<T, 3> lerp(float t, const Vector<T, 3>& v1, const Vector<T, 3>& v2) noexcept
    {
      return (1 - t) * v1 + t * v2;
    }

    template<typename T>
    inline Vector<T, 3> lerp(float t, const Vector<T, 3>& v,  const Normal<T, 3>& n) noexcept
    {
      return (1 - t) * v + t * n;
    }
    template<typename T>
    inline bool hasNaN(const Vector<T, 3>& v) noexcept
    {
      return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z);
    }

    template <typename T> 
    inline void coordinateSystem(const Vector<T, 3> &v1, Vector<T, 3> *v2, Vector<T, 3> *v3) noexcept
    {
        if (std::abs(v1.x) > std::abs(v1.y))
            *v2 = Vector<T, 3>(-v1.z, 0, v1.x) /
                  std::sqrt(v1.x * v1.x + v1.z * v1.z);
        else
            *v2 = Vector<T, 3>(0, v1.z, -v1.y) /
                              std::sqrt(v1.y * v1.y + v1.z * v1.z);
                              
        *v3 = cross(v1, *v2);
    }

    /// Vector 2 operations

    template<typename T>
    template<typename U>
    constexpr Vector<T, 2>::Vector(const Point<U, 2>& p)noexcept : x(static_cast<T>(p.x)), y(static_cast<T>(p.y)) {};

    template<typename T>
    template<typename U>
    constexpr Vector<T, 2>::Vector(const Vector<U, 2>& v)noexcept : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {};

    template<typename T>
    constexpr Vector<T, 2>::Vector(const Vector<T, 4>& v) noexcept : x(v.x), y(v.y) {};
    template<typename T>
    constexpr Vector<T, 2>::Vector(const Vector<T, 3>& v) noexcept : x(v.x), y(v.y) {};
    template<typename T>
    constexpr Vector<T, 2>::Vector(const Point<T, 2> & p) noexcept : x(p.x), y(p.y) {};

    template<typename T>
    constexpr T dot(const Vector<T, 2>& v1, const Vector<T, 2>& v2) noexcept
    {
      return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T>
    constexpr T cross(const Vector<T, 2>& v1, const Vector<T, 2>& v2) noexcept
    {
      return v1.x * v2.y - v1.y * v2.x;
    };

    template<typename T>
    constexpr Point<T, 2> operator+(const Vector<T, 2>& v, const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(v.x + p.x, v.y + p.y);
    }
    template<typename T>
    constexpr Point<T, 2> operator-(const Vector<T, 2>& v, const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(v.x - p.x, v.y - p.y);
    }
    template<typename T>
    inline Vector<T, 2> floor(const Vector<T, 2>& v) noexcept
    {
      return Vector<T, 2>(static_cast<T>(std::floor(v.x)), static_cast<T>(std::floor(v.y)));
    }
    template<typename T>
    inline Vector<T, 2> ceil(const Vector<T, 2>& v) noexcept
    {
      return Vector<T, 2>(static_cast<T>(std::ceil(v.x)), static_cast<T>(std::ceil(v.y)));
    }
    template<typename T>
    inline Vector<T, 2> round(const Vector<T, 2>& v) noexcept
    {
      return Vector<T, 2>(static_cast<T>(std::round(v.x)), static_cast<T>(std::round(v.y)));
    }
    template<typename T>
    inline Vector<T, 2> abs(const Vector<T, 2>& v) noexcept
    {
      return Vector<T, 2>(static_cast<T>(std::abs(v.x)), static_cast<T>(std::abs(v.y)));
    }
    template<typename T>
    inline bool hasNaN(const Vector<T, 2>& v) noexcept
    {
      return std::isnan(v.x) || std::isnan(v.y);
    }
    template<typename T>
    inline Vector<T, 2> min(const Vector<T, 2>& v1, const Vector<T, 2>& v2)
    {
      return Vector<T, 2>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
    }
    template<typename T>
    inline Vector<T, 2> max(const Vector<T, 2>& v1, const Vector<T, 2>& v2)
    {
      return Vector<T, 2>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
    }

  /// Point operations
    template<typename T, std::size_t L>
    inline T sqrDist(const Point<T, L>& p1, const Point<T, L>& p2) noexcept
    {
      return dot(p2 - p1, p2 - p1);
    }

    template<typename T, std::size_t L>
    inline T distance(const Point<T, L>& p1, const Point<T, L>& p2) noexcept
    {
      return std::sqrt(sqrDist(p1, p2));
    }

    template<typename T, std::size_t L>
    inline Point<T, L> operator*(const T t, const Point<T, L>& p) noexcept
    {
        return p * t;
    }

    template<typename T, std::size_t L>
    inline Point<T, L> operator+(const Normal<T, L>& n, const Point<T, L>& p) noexcept
    {
        return p + n;
    }
    template<typename T, std::size_t L>
    inline Point<T, L> operator*(const Vector<T, L>& v, const Point<T, L>& p) noexcept
    {
        return p + v;
    }
    /// Point 4 operations

    template<typename T>
    inline Point<T, 4> min(const Point<T, 4>& p1, const Point<T, 4>& p2)
    {
      return Point<T, 4>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z), std::min(p1.w, p2.w));
    }
    template<typename T>
    inline Point<T, 4> max(const Point<T, 4>& p1, const Point<T, 4>& p2)
    {
      return Point<T, 4>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z), std::max(p1.w, p2.w));
    }

    template<typename T>
    constexpr Point<T, 4>::Point(const Point<T, 3>& p)  noexcept : x(p.x), y(p.y), z(p.z), w(1)   {};
    template<typename T>
    constexpr Point<T, 4>::Point(const Vector<T, 4>& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {};
    template<typename T>
    constexpr Point<T, 4>::Point(const Normal<T, 4>& n) noexcept : x(n.x), y(n.y), z(n.z), w(n.w) {};


    template<typename T>
    constexpr Vector<T, 4> Point<T, 4>::operator-(const Point<T, 4>& p) const noexcept
    {
        return Vector<T, 4>(x - p.x, y - p.y, z - p.z, w - p.w);
    }

    template<typename T>
    constexpr Point<T, 4> Point<T, 4>::operator+(const Vector<T, 4>& v) const noexcept
    {
        return Point<T, 4>(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    template<typename T>
    constexpr Point<T, 4>& Point<T, 4>::operator+=(const Vector<T, 4>& v) noexcept
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        this->w += v.w;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 4> Point<T, 4>::operator-(const Vector<T, 4>& v) const noexcept
    {
        return Point<T, 4>(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    template<typename T>
    constexpr Point<T, 4>& Point<T, 4>::operator-=(const Vector<T, 4>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 4> Point<T, 4>::operator+(const Normal<T, 4>& n) const noexcept
    {
        return Point<T, 4>(x + n.x, y + n.y, z + n.z, w + n.w);
    }

    template<typename T>
    constexpr Point<T, 4>& Point<T, 4>::operator+=(const Normal<T, 4>& n) noexcept
    {
        this->x += n.x;
        this->y += n.y;
        this->z += n.z;
        this->w += n.w;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 4> Point<T, 4>::operator-(const Normal<T, 4>& n) const noexcept
    {
        return Point<T, 4>(x - n.x, y - n.y, z - n.z, w - n.w);
    }

    template<typename T>
    constexpr Point<T, 4>& Point<T, 4>::operator-=(const Normal<T, 4>& n) noexcept
    {
        x -= n.x;
        y -= n.y;
        z -= n.z;
        w -= n.w;

        return *this;
    }

    template<typename T>
    inline Point<T, 4> floor(const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(static_cast<T>(std::floor(p.x)), static_cast<T>(std::floor(p.y)), static_cast<T>(std::floor(p.z)), static_cast<T>(std::floor(p.w)));
    }
    template<typename T>
    inline Point<T, 4> ceil(const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(static_cast<T>(std::ceil(p.x)), static_cast<T>(std::ceil(p.y)), static_cast<T>(std::ceil(p.z)), static_cast<T>(std::ceil(p.w)));
    }
    template<typename T>
    inline Point<T, 4> round(const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(static_cast<T>(std::round(p.x)), static_cast<T>(std::round(p.y)), static_cast<T>(std::round(p.z)), static_cast<T>(std::round(p.w)));
    }
    template<typename T>
    inline Point<T, 4> abs(const Point<T, 4>& p) noexcept
    {
      return Point<T, 4>(static_cast<T>(std::abs(p.x)), static_cast<T>(std::abs(p.y)), static_cast<T>(std::abs(p.z)), static_cast<T>(std::abs(p.w)));
    }
    template<typename T>
    inline bool hasNaN(const Point<T, 4>& p) noexcept
    {
      return std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z) || std::isnan(p.w);
    }

    /// Point 3 operations 
    template<typename T>
    inline Point<T, 3> min(const Point<T, 3>& p1, const Point<T, 3>& p2)
    {
      return Point<T, 3>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
    }
    template<typename T>
    inline Point<T, 3> max(const Point<T, 3>& p1, const Point<T, 3>& p2)
    {
      return Point<T, 3>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
    }

    template<typename T>
    constexpr Point<T, 3>::Point(const Vector<T, 3>& v) noexcept : x(v.x), y(v.y), z(v.z) {};
    template<typename T>
    constexpr Point<T, 3>::Point(const Normal<T, 3>& n) noexcept : x(n.x), y(n.y), z(n.z) {};
    template<typename T>
    constexpr Point<T, 3>::Point(const Point<T, 4> & p) noexcept : x(p.x/p.w), y(p.y/p.w), z(p.z/p.w) {};
    template<typename T>
    constexpr Point<T, 3>::Point(const Point<T, 2> & p) noexcept : x(p.x), y(p.y), z(0)   {};

    template<typename T>
    template<typename U>
    constexpr Point<T, 3>::Point(const Point<U, 3>& p)noexcept : x(static_cast<T>(p.x)), y(static_cast<T>(p.y)), z(static_cast<T>(p.z)) {};
    template<typename T>
    template<typename U>
    constexpr Point<T, 3>::Point(const Vector<U, 3>& v)noexcept : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {};
    template<typename T>
    template<typename U>
    constexpr Point<T, 3>::Point(const Normal<U, 3>& n)noexcept : x(static_cast<T>(n.x)), y(static_cast<T>(n.y)), z(static_cast<T>(n.z)) {};
  
    template<typename T>
    constexpr Point<T, 3> Point<T, 3>::operator+(const Vector<T, 3>& v) const noexcept
    {
        return Point<T, 3>(x + v.x, y + v.y, z + v.z);
    }
    template<typename T>
    constexpr Point<T, 3>& Point<T, 3>::operator+=(const Vector<T, 3>& v) noexcept
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;

        return *this;
    }
    

    template<typename T>
    constexpr Point<T, 3> Point<T, 3>::operator-(const Vector<T, 3>& v) const noexcept
    {
        return Point<T, 3>(x - v.x, y - v.y, z - v.z);
    }

    template<typename T>
    constexpr Point<T, 3>& Point<T, 3>::operator-=(const Vector<T, 3>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 3> Point<T, 3>::operator+(const Normal<T, 3>& n) const noexcept
    {
        return Point<T, 3>(x + n.x, y + n.y, z + n.z);
    }

    template<typename T>
    constexpr Point<T, 3>& Point<T, 3>::operator+=(const Normal<T, 3>& n) noexcept
    {
        this->x += n.x;
        this->y += n.y;
        this->z += n.z;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 3> Point<T, 3>::operator-(const Normal<T, 3>& n) const noexcept
    {
        return Point<T, 3>(x - n.x, y - n.y, z - n.z);
    }

    template<typename T>
    constexpr Point<T, 3>& Point<T, 3>::operator-=(const Normal<T, 3>& n) noexcept
    {
        x -= n.x;
        y -= n.y;
        z -= n.z;

        return *this;
    }

    template<typename T>
    constexpr Vector<T, 3> Point<T, 3>::operator-(const Point<T, 3>& p) const noexcept
    {
        return Vector<T, 3>(x - p.x, y - p.y, z - p.z);
    }

    template<typename T>
    inline Point<T, 3> lerp(float t, const Point<T, 3>& p1, const Point<T, 3>& p2) noexcept
    {
      return (1 - t) * p1 + t * p2;
    }

    template<typename T>
    inline Point<T, 3> floor(const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(static_cast<T>(std::floor(p.x)), static_cast<T>(std::floor(p.y)), static_cast<T>(std::floor(p.z)));
    }
    template<typename T>
    inline Point<T, 3> ceil(const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(static_cast<T>(std::ceil(p.x)), static_cast<T>(std::ceil(p.y)), static_cast<T>(std::ceil(p.z)));
    }
    template<typename T>
    inline Point<T, 3> round(const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(static_cast<T>(std::round(p.x)), static_cast<T>(std::round(p.y)), static_cast<T>(std::round(p.z)));
    }
    template<typename T>
    inline Point<T, 3> abs(const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(static_cast<T>(std::abs(p.x)), static_cast<T>(std::abs(p.y)), static_cast<T>(std::abs(p.z)));
    }
    template<typename T>
    inline bool hasNaN(const Point<T, 3>& p) noexcept
    {
      return std::isnan(p.x) || std::isnan(p.y) || std::isnan(p.z);
    }

    /// Point 2 operations
    template<typename T>
    inline Point<T, 2> min(const Point<T, 2>& p1, const Point<T, 2>& p2)
    {
      return Point<T, 2>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
    }
    template<typename T>
    inline Point<T, 2> max(const Point<T, 2>& p1, const Point<T, 2>& p2)
    {
      return Point<T, 2>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
    }

    template<typename T>
    template<typename U>
    constexpr Point<T, 2>::Point(const Point<U, 2>& p)noexcept : x(static_cast<T>(p.x)), y(static_cast<T>(p.y)) {};
    template<typename T>
    template<typename U>
    constexpr Point<T, 2>::Point(const Vector<U, 2>& v)noexcept : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {};

    template<typename T>
    constexpr Point<T, 2>::Point(const Vector<T, 2>& v)noexcept : x(v.x), y(v.y) {};
    template<typename T>
    constexpr Point<T, 2>::Point(const Point<T, 4>& p) noexcept : x(p.x), y(p.y) {};
    template<typename T>
    constexpr Point<T, 2>::Point(const Point<T, 3>& p) noexcept : x(p.x), y(p.y) {};

    template<typename T>
    constexpr Point<T, 2> Point<T, 2>::operator+(const Vector<T, 2>& v) const noexcept
    {
        return Point<T, 2>(x + v.x, y + v.y);
    }

    template<typename T>
    constexpr Point<T, 2>& Point<T, 2>:: operator+=(const Vector<T, 2>& v) noexcept
    {
        this->x += v.x;
        this->y += v.y;

        return *this;
    }

    template<typename T>
    constexpr Point<T, 2> Point<T, 2>::operator-(const Vector<T, 2>& v) const noexcept
    {
        return Point<T, 2>(x - v.x, y - v.y);
    }

    template<typename T>
    constexpr Point<T, 2>& Point<T, 2>::operator-=(const Vector<T, 2>& v) noexcept
    {
        x -= v.x;
        y -= v.y;

        return *this;
    }

    template<typename T>
    constexpr Vector<T, 2> Point<T, 2>::operator-(const Point<T, 2>& p) const noexcept
    {
        return Vector<T, 2>(x - p.x, y - p.y);
    }

    template<typename T>
    inline Point<T, 2> lerp(float t, const Point<T, 2>& p1, const Point<T, 2>& p2) noexcept
    {
      return (1 - t) * p1 + t * p2;
    }

    template<typename T>
    inline Point<T, 2> floor(const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(static_cast<T>(std::floor(p.x)), static_cast<T>(std::floor(p.y)));
    }
    template<typename T>
    inline Point<T, 2> ceil(const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(static_cast<T>(std::ceil(p.x)), static_cast<T>(std::ceil(p.y)));
    }
    template<typename T>
    inline Point<T, 2> round(const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(static_cast<T>(std::round(p.x)), static_cast<T>(std::round(p.y)));
    }
    template<typename T>
    inline Point<T, 2> abs(const Point<T, 2>& p) noexcept
    {
      return Point<T, 2>(static_cast<T>(std::abs(p.x)), static_cast<T>(std::abs(p.y)));
    }
    template<typename T>
    inline bool hasNaN(const Point<T, 2>& p) noexcept
    {
      return std::isnan(p.x) || std::isnan(p.y);
    }

  /// Normal operations
  
    template<typename T, std::size_t L>
    inline Normal<T, L> operator*(T t, const Normal<T, L>& n) noexcept
    {
        return n * t;
    }
    template<typename T, std::size_t L>
    inline Normal<T, L> normalize(const Normal<T, L>& n) noexcept
    {
      T len = length(n);
      return len > 0 ? n / len : n; 
    }

    template<typename T, std::size_t L>
    inline T length(const Normal<T, L>& n) noexcept
    {
      return  std::sqrt(sqrLength(n));
    }
    template<typename T, std::size_t L>
    inline T sqrLength(const Normal<T, L>& n) noexcept
    {
      return dot(n, n);
    }
    

    /// Normal 4 operations

    template<typename T>
    inline Normal<T, 4> min(const Normal<T, 4>& n1, const Normal<T, 4>& n2)
    {
      return Normal<T, 4>(std::min(n1.x, n2.x), std::min(n1.y, n2.y), std::min(n1.z, n2.z), std::min(n1.w, n2.w));
    }
    template<typename T>
    inline Normal<T, 4> max(const Normal<T, 4>& n1, const Normal<T, 4>& n2)
    {
      return Normal<T, 4>(std::max(n1.x, n2.x), std::max(n1.y, n2.y), std::max(n1.z, n2.z), std::max(n1.w, n2.w));
    }
    template<typename T>
    constexpr Normal<T, 4>::Normal(const Point<T, 4>& p) noexcept : x(p.x), y(p.y), z(p.z), w(p.w) {};
    template<typename T>
    constexpr Normal<T, 4>::Normal(const Vector<T, 4>& v) noexcept : x(v.x), y(v.y), z(v.z), w(v.w) {};
    template<typename T>
    constexpr Normal<T, 4>::Normal(const Normal<T, 3>& n) noexcept : x(n.x),y(n.y), z(n.z), w(static_cast<T>(0)) {};

    template<typename T>
    inline T dot(const Normal<T, 4>& n1, const Normal<T, 4>& n2) noexcept
    {
      return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z + n1.w * n2.w;
    }


    template<typename T>
    inline Normal<T, 4> floor(const Normal<T, 4>& n) noexcept
    {
      return Normal<T, 4>(static_cast<T>(std::floor(n.x)), static_cast<T>(std::floor(n.y)), static_cast<T>(std::floor(n.z)), static_cast<T>(std::floor(n.w)));
    }
    template<typename T>
    inline Normal<T, 4> ceil(const Normal<T, 4>& n) noexcept
    {
      return Normal<T, 4>(static_cast<T>(std::ceil(n.x)), static_cast<T>(std::ceil(n.y)), static_cast<T>(std::ceil(n.z)), static_cast<T>(std::ceil(n.w)));
    }
    template<typename T>
    inline Normal<T, 4> round(const Normal<T, 4>& n) noexcept
    {
      return Normal<T, 4>(static_cast<T>(std::round(n.x)), static_cast<T>(std::round(n.y)), static_cast<T>(std::round(n.z)), static_cast<T>(std::round(n.w)));
    }
    template<typename T>
    inline Normal<T, 4> abs(const Normal<T, 4>& n) noexcept
    {
      return Normal<T, 4>(static_cast<T>(std::abs(n.x)), static_cast<T>(std::abs(n.y)), static_cast<T>(std::abs(n.z)), static_cast<T>(std::abs(n.w)));
    }
    template<typename T>
    inline bool hasNaN(const Normal<T, 4>& n) noexcept
    {
      return std::isnan(n.x) || std::isnan(n.y) || std::isnan(n.z) || std::isnan(n.w);
    }

    /// Normal 3 operations
    template<typename T>
    inline Normal<T, 3> min(const Normal<T, 3>& n1, const Normal<T, 3>& n2)
    {
      return Normal<T, 3>(std::min(n1.x, n2.x), std::min(n1.y, n2.y), std::min(n1.z, n2.z));
    }
    template<typename T>
    inline Normal<T, 3> max(const Normal<T, 3>& n1, const Normal<T, 3>& n2)
    {
      return Normal<T, 3>(std::max(n1.x, n2.x), std::max(n1.y, n2.y), std::max(n1.z, n2.z));
    }

    template<typename T>
    inline T dot(const Normal<T, 3>& n1, const Normal<T, 3>& n2) noexcept
    {
      return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
    }

    template<typename T>
    constexpr T dot(const Normal<T, 3>& n, const Vector<T, 3>& v) noexcept
    {
      return n.x * v.x + n.y * v.y + n.z * v.z;
    }

    template<typename T>
    constexpr Normal<T, 3> cross(const Normal<T, 3>& n1, const Normal<T, 3>& n2) noexcept
    {
      return Normal<T, 3>(n1.y * n2.z - n1.z * n2.y,
                          n1.z * n2.x - n1.x * n2.z,
                          n1.x * n2.y - n1.y * n2.x);
    }
    template<typename T>
    constexpr Normal<T, 3> cross(const Normal<T, 3>& n, const Vector<T, 3>& v) noexcept
    {
      return Normal<T, 3>(n.y * v.z - n.z * v.y,
                          n.z * v.x - n.x * v.z,
                          n.x * v.y - n.y * v.x);
    }

    template<typename T>
    inline Normal<T, 3> floor(const Normal<T, 3>& n) noexcept
    {
      return Normal<T, 3>(static_cast<T>(std::floor(n.x)), static_cast<T>(std::floor(n.y)), static_cast<T>(std::floor(n.z)));
    }
    template<typename T>
    inline Normal<T, 3> ceil(const Normal<T, 3>& n) noexcept
    {
      return Normal<T, 3>(static_cast<T>(std::ceil(n.x)), static_cast<T>(std::ceil(n.y)), static_cast<T>(std::ceil(n.z)));
    }
    template<typename T>
    inline Normal<T, 3> round(const Normal<T, 3>& n) noexcept
    {
      return Normal<T, 3>(static_cast<T>(std::round(n.x)), static_cast<T>(std::round(n.y)), static_cast<T>(std::round(n.z)));
    }
    template<typename T>
    inline Normal<T, 3> abs(const Normal<T, 3>& n) noexcept
    {
      return Normal<T, 3>(static_cast<T>(std::abs(n.x)), static_cast<T>(std::abs(n.y)), static_cast<T>(std::abs(n.z)));
    }

    template<typename T>
    inline Normal<T, 3> lerp(float t, const Normal<T, 3>& n1, const Normal<T, 3>& n2) noexcept
    {
      return (1 - t) * n1 + t * n2;
    }
    template<typename T>
    inline Normal<T, 3> lerp(float t, const Normal<T, 3>& n, const Vector<T, 3>& v) noexcept
    {
      return (1 - t) * n + t * v;
    }
    template<typename T>
    inline bool hasNaN(const Normal<T, 3>& n) noexcept
    {
      return std::isnan(n.x) || std::isnan(n.y) || std::isnan(n.z);
    }
    template<typename T>
    constexpr Normal<T, 3> Normal<T, 3>::operator+(const Vector<T, 3>& v) const noexcept
    {
      return Normal<T, 3>(x + v.x, y + v.y, z + v.z);
    }
    template<typename T>
    constexpr Normal<T, 3> Normal<T, 3>::operator-(const Vector<T, 3>& v) const noexcept
    {
      return Normal<T, 3>(x - v.x, y - v.y, z - v.z);
    }
    template<typename T>
    constexpr Point<T, 3> operator+(const Normal<T, 3>& n, const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(n.x + p.x, n.y + p.y, n.z + p.z);
    }
    template<typename T>
    constexpr Point<T, 3> operator-(const Normal<T, 3>& n, const Point<T, 3>& p) noexcept
    {
      return Point<T, 3>(n.x - p.x, n.y - p.y, n.z - p.z);
    }
    template<typename T>
    constexpr Normal<T, 3>& Normal<T, 3>::operator+=(const Vector<T, 3>& v) noexcept
    {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }
    template<typename T>
    constexpr Normal<T, 3>& Normal<T, 3>::operator-=(const Vector<T, 3>& v) noexcept
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }
    
    template<typename T>
    inline Normal<T, 3> faceFoward(const Normal<T, 3>& n1, const Normal<T, 3>& n2) noexcept
    {
      return dot(n1, n2) < 0.f ? -n1 : n1;
    }

    template<typename T>
    inline Normal<T, 3> faceFoward(const Normal<T, 3>& n, const Vector<T, 3>& v) noexcept
    {
      return dot(n, v) < 0.f ? -n : n;
    }
    

    
    template<typename T>
    constexpr Normal<T, 3>::Normal(const Point<T, 3>& p) noexcept : x(p.x), y(p.y), z(p.z) {};
    template<typename T>
    constexpr Normal<T, 3>::Normal(const Vector<T, 3>& v) noexcept : x(v.x), y(v.y), z(v.z) {assert(!hasNaN(v));};
    template<typename T>
    constexpr Normal<T, 3>::Normal(const Normal<T, 4>& n) noexcept : x(n.x), y(n.y), z(n.z) {};


  /// Color operations

    constexpr Color::Color(const Vector<float, 3>& v) noexcept : r(v.r), g(v.g), b(v.b) {};
    constexpr Color::Color(const Normal<float, 3>& n) noexcept : r(n.r), g(n.g), b(n.b) {};
    constexpr Color::Color(const Point<float, 3>&  p) noexcept : r(p.r), g(p.g), b(p.b) {};

    inline Color lerp(float t, Color a, Color b)
    {
      return (1 - t) * a + t * b;
    }

   constexpr float sRGBToLinear(float val) noexcept
    {
        if (val <= 0.04045f) 
            return val / 12.92f;
        
        return std::pow((val + 0.055f) / 1.055f, 2.4f);
    }

    constexpr Color clamp(const Color& c, float minVal = 0.0f, float maxVal = std::numeric_limits<float>::infinity()) noexcept
    {
        return Color(std::clamp(c.r, minVal, maxVal),
                     std::clamp(c.g, minVal, maxVal),
                     std::clamp(c.b, minVal, maxVal));
    }
    constexpr Normal<float, 3> clamp(const Normal<float, 3>& n, float minVal = 0.0f, float maxVal = std::numeric_limits<float>::infinity()) noexcept
    {
        return Normal<float, 3>(std::clamp(n.r, minVal, maxVal),
                                 std::clamp(n.g, minVal, maxVal),
                                 std::clamp(n.b, minVal, maxVal));
    }

    constexpr Color fromRGB8(int r, int g, int b) noexcept
    {
        float fr = std::clamp(r, 0, 255) / 255.0f;
        float fg = std::clamp(g, 0, 255) / 255.0f;
        float fb = std::clamp(b, 0, 255) / 255.0f;
        
        return Color(sRGBToLinear(fr), sRGBToLinear(fg), sRGBToLinear(fb));
    }
    constexpr bool hasNaNs(const Color& c) noexcept
    {
        return std::isnan(c.r) || std::isnan(c.g) || std::isnan(c.b);
    }

    constexpr Color fromHex(uint32_t hexValue) noexcept
    {
        int r = (hexValue >> 16) & 0xFF;
        int g = (hexValue >> 8)  & 0xFF;
        int b =  hexValue        & 0xFF;
        
        return fromRGB8(r, g, b);
    }
    constexpr Color toSRGB(const Color& c) noexcept
        {
            auto gammaCorrect = [](float value) 
            {
                if (value <= 0.0031308f) 
                    return 12.92f * value;
                
                return 1.055f * std::pow(value, 1.0f / 2.4f) - 0.055f;
            };

            return Color(gammaCorrect(c.r), gammaCorrect(c.g), gammaCorrect(c.b));
        }

  /// non geometric operations
    inline float lerp(float t, float v1, float v2)
    {
      return (1 - t) * v1 + t * v2;
    }
    inline float lerp(float t, int v1, int v2)
    {
      return (1 - t) * v1 + t * v2;
    }
    inline float clamp(float t, float a, float b)
    {
      return std::max(a, std::min(t, b));
    }
    

};

#endif //< SSMATH3_HPP