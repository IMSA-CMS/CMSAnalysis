#ifndef PARTICLEINPUTMODULE_HH
#define PARTICLEINPUTMODULE_HH

#include "EventInput.hh"
#include "Particle.hh"

class ParticleInputModule : public EventInput 
{
    public:
        std::ParticleCollection getLeptons() {return leptons;}
    protected:
        void addLepton(Particle particle) {lepton.push_back(particle);}
    private:
        std::ParticleCollection leptons;
    
};


#endif