#ifndef XYZ_COLOR_HPP
#define XYZ_COLOR_HPP

#include "Spectrum.hpp"
#include "Utils/common.hpp"

namespace ssrt 
{
    class XYZ 
    {
        private:

        public:
        
            float x{0.f};
            float y{0.f};
            float z{0.f};

            XYZ() = default;
            XYZ(float x, float y, float z)
            : x(x), y(y), z(z) {};


            float innerProduct(Spectrum);

    };

    XYZ spectrumToXYZ(Spectrum s);

    inline XYZ RGBToXYZ(const Color& rgb) 
    {
        XYZ xyz;

        xyz.x = 0.412453f*rgb[0] + 0.357580f*rgb[1] + 0.180423f*rgb[2];
        xyz.y = 0.212671f*rgb[0] + 0.715160f*rgb[1] + 0.072169f*rgb[2];
        xyz.z = 0.019334f*rgb[0] + 0.119193f*rgb[1] + 0.950227f*rgb[2];

        return xyz;
    }

    inline Color XYZToRGB(const XYZ& xyz) 
    {
        Color rgb;

        rgb[0] =  3.240479f*xyz.x - 1.537150f*xyz.y - 0.498535f*xyz.z;
        rgb[1] = -0.969256f*xyz.x + 1.875991f*xyz.y + 0.041556f*xyz.z;
        rgb[2] =  0.055648f*xyz.x - 0.204043f*xyz.y + 1.057311f*xyz.z;
    
        return rgb;
    }
    constexpr int CIE_MIN = 360;
    constexpr int CIE_MAX = 830;
    constexpr int CIE_STEP = 5;
    constexpr int CIE_SAMPLES = 95;
    constexpr float CIE_Y_INTEGRAL = 106.8568f;
    
    const float CIE_X_TABLE[CIE_SAMPLES] = 
    {
        0.0001299f,   0.0002321f,  0.0004149f,  0.0007416f,  0.001368f,   0.002236f,   0.004243f,   0.00765f,   0.01431f,0.02319f,
        0.04351f,   0.07763f,   0.13438f,   0.21477f,   0.2839f,    0.3285f,    0.34828f,   0.34806f,   0.3362f,     0.3187f,
        0.2908f,    0.2511f,    0.19536f,   0.1421f,    0.09564f,   0.05795f,   0.03201f,   0.0147f,    0.0049f,     0.0024f,
        0.0093f,    0.0291f,    0.06327f,   0.1096f,    0.1655f,    0.22575f,   0.2904f,    0.3597f,    0.43345f,    0.51205f,
        0.5945f,    0.6784f,    0.7621f,    0.8425f,    0.9163f,    0.9786f,    1.0263f,    1.0567f,    1.0622f,     1.0456f,
        1.0026f,    0.9384f,    0.85445f,   0.7514f,    0.6424f,    0.5419f,    0.4479f,    0.3608f,    0.2835f,     0.2187f,
        0.1649f,    0.1212f,    0.0874f,    0.0636f,    0.04677f,   0.0329f,    0.0227f,    0.01584f,   0.01136f,    0.00811f,
        0.00579f,   0.0041f,    0.00289f,   0.00205f,   0.00144f,   0.0010f,    0.00071f,   0.0005f,    0.00036f,    0.00025f,
        0.00017f,   0.00012f,   0.00008f,   0.00006f,   0.00004f,   0.00003f,   0.00002f,   0.00001f,   0.00001f,    0.00001f,
        0.00001f,   0.0000f,    0.0000f,    0.0000f,    0.0000f
    };

    const float CIE_Y_TABLE[CIE_SAMPLES] = 
    {
        0.0000039f, 0.0000069f, 0.0000120f, 0.0000220f, 0.0000390f, 0.0000640f, 0.0001200f, 0.0002170f, 0.0003960f, 0.0006400f,
        0.0012100f, 0.0021800f, 0.0040000f, 0.0073000f, 0.0116000f, 0.0168400f, 0.0230000f, 0.0298000f, 0.0380000f, 0.0480000f,
        0.0600000f, 0.0739000f, 0.0909800f, 0.1126000f, 0.1390200f, 0.1693000f, 0.2080200f, 0.2582000f, 0.3230000f, 0.4073000f,
        0.5030000f, 0.6082000f, 0.7100000f, 0.7932000f, 0.8620000f, 0.9148500f, 0.9540000f, 0.9803000f, 0.9949500f, 1.0000000f,
        0.9950000f, 0.9786000f, 0.9520000f, 0.9154000f, 0.8700000f, 0.8163000f, 0.7570000f, 0.6949000f, 0.6310000f, 0.5668000f,
        0.5030000f, 0.4412000f, 0.3810000f, 0.3210000f, 0.2650000f, 0.2170000f, 0.1750000f, 0.1382000f, 0.1070000f, 0.0816000f,
        0.0610000f, 0.0445800f, 0.0320000f, 0.0232000f, 0.0170000f, 0.0119200f, 0.0082100f, 0.0057230f, 0.0040000f, 0.0027320f,
        0.0018700f, 0.0012770f, 0.0008700f, 0.0005930f, 0.0004030f, 0.0002740f, 0.0001870f, 0.0001270f, 0.0000860f, 0.0000580f,
        0.0000390f, 0.0000270f, 0.0000180f, 0.0000120f, 0.0000080f, 0.0000050f, 0.0000030f, 0.0000020f, 0.0000010f, 0.0000010f,
        0.0000000f, 0.0000000f, 0.0000000f, 0.0000000f, 0.0000000f
    };

    const float CIE_Z_TABLE[CIE_SAMPLES] = 
    {
        0.0006061f, 0.001086f,  0.001946f,  0.003486f,  0.00645f,   0.01055f,   0.02005f,   0.03621f,   0.06785f,   0.1102f,
        0.2074f,    0.3713f,    0.6456f,    1.0391f,    1.3856f,    1.623f,     1.747f,     1.7826f,    1.7721f,    1.7346f,
        1.6692f,    1.5281f,    1.2876f,    1.011f,     0.7303f,    0.505f,     0.3235f,    0.1873f,    0.0984f,    0.046f,
        0.0223f,    0.0116f,    0.0055f,    0.0026f,    0.0013f,    0.0007f,    0.0003f,    0.0002f,    0.0001f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f,
        0.0000f,    0.0000f,    0.0000f,    0.0000f,    0.0000f
    };

    inline float sampleCIE(float lambda, const float* table) 
    {
        if (lambda < CIE_MIN || lambda > CIE_MAX) 
            return 0.f;
        
        float offset = (lambda - CIE_MIN) / CIE_STEP;
        int index = static_cast<int>(offset);
        float t = offset - index; 
        
        if (index >= (CIE_MAX - CIE_MIN) / CIE_STEP) 
            return table[index];
        
        return ::lerp(t, table[index], table[index + 1]);
    }

    inline float cieX(float lambda) 
    { 
        return sampleCIE(lambda, CIE_X_TABLE); 
    }
    inline float cieY(float lambda) 
    { 
        return sampleCIE(lambda, CIE_Y_TABLE); 
    }
    inline float cieZ(float lambda) 
    { 
        return sampleCIE(lambda, CIE_Z_TABLE); 
    }
}

#endif //< XYZ_COLOR_HPP