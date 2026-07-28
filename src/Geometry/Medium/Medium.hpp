#ifndef MEDIUM_HPP
#define MEDIUM_HPP

namespace Geo 
{
    class Medium 
    {      
        public:
        /*TODO*/
    };
    class MediumInterface  
    {
        public:
        
        const Medium *inside;
        const Medium *outside;

        MediumInterface() 
        : inside(nullptr), outside(nullptr) {};

        MediumInterface(const Medium *medium)
        : inside(medium), outside(medium) {}

        MediumInterface(const Medium *inside, const Medium *outside)
        : inside(inside), outside(outside) {}

        bool IsMediumTransition() const 
        { 
            return inside != outside; 
        }

    };
}

#endif