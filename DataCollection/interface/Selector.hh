#ifndef SELECTOR_HH
#define SELECTOR_HH

class Particle;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual bool checkParticle(const Particle& particle) const = 0;  
};

#endif