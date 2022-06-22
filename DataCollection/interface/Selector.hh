#ifndef SELECTOR_HH
#define SELECTOR_HH

class Particle;
class Lepton;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual bool checkParticle(const Lepton& particle) const = 0;  
};

#endif