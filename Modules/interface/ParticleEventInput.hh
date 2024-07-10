#ifndef PARTICLEEVENTINPUT_HH
#define PARTICLEEVENTINPUT_HH

#include "EventInput.hh"
#include "Particle.hh"

class ParticleEventInput : public EventInput 
{
    public:
        std::ParticleCollection getLeptons() {return leptons;}
        
    protected:
        void addLepton(Particle particle) {lepton.push_back(particle);}
    private:
        std::ParticleCollection leptons;
    
};


#endif