#ifndef TRANSFORMED_LIGHT_HPP
#define TRANSFORMED_LIGHT_HPP

#include <Light/Light.hpp>
#include <memory>
namespace Luz 
{
    class TransformedLight : public Light
    {
        private:
            std::shared_ptr<Light> light;

            Transform* L2W;
            Transform* W2L;
            
        public:
    };
};

#endif //< TRANSFORMED_LIGHT_HPP