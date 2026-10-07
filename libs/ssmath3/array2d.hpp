#ifndef SSMATH_ARRAY_2D_HPP
#define SSMATH_ARRAY_2D_HPP

#include <cassert>
#include <cstdint>
#include <vector>

namespace ssmath 
{
    template<typename T>
    class Array2D 
    {
        private:
            std::vector<T> data;
        public:
            std::size_t width;
            std::size_t height;
            constexpr Array2D(std::size_t width, std::size_t height) noexcept
            : width(width), height(height)
            {
                data.resize(width * height);
            }

            constexpr Array2D(std::size_t width, std::size_t height, const T* idata) noexcept
            : width(width), height(height)
            {
                data.assign(idata, idata + width * height);
            }

            const T& operator()(const std::size_t i, const std::size_t j) const
            {
                assert(i < width && j < height);
                return data[j * width + i];
            }

            T& operator()(const std::size_t i, const std::size_t j)
            {
                assert(i < width && j < height);
                return data[j * width + i];
            }
    };

};


#endif //< SSMATH_ARRAY_2D_HPP