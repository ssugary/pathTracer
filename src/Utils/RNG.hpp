#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

#include "common.hpp"
#include <algorithm>
#include <cstdint>
#include <random>
;

class RNG 
{
    private:
        std::mt19937_64 gen;
        std::uniform_real_distribution<float> dist_float{0.0, ONE_MINUS_EPSILON};

    public:
        RNG(uint64_t sequenceIndex = 0)
        {
            setSequence(sequenceIndex);
        }

        uint64_t uniformUInt32()
        {
            return gen();
        }

        uint64_t uniformUInt32(uint32_t b) 
        {
            if (b == 0) 
                return 0;
            std::uniform_int_distribution<uint32_t> dist_int(0, b - 1);
            return dist_int(gen);
        }

        float uniformFloat()
        {
            return dist_float(gen);
        }

        void setSequence(uint32_t sequenceIndex)
        {
            gen.seed(sequenceIndex);
        }
};

#endif //< RANDOM_NUMBER_GENERATOR_HPP