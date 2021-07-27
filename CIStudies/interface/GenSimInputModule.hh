#ifndef GENSIMINPUTMODULE_HH
#define GENSIMINPUTMODULE_HH

#include "ParticleInputModule.hh"

class GenSimInputModule : public ParticleInputModule
{
    public:
        GenSimInputModule();
        //just returns true
        virtual bool process() override;
        ParticleCollection getGenParticles() {return genParticles;}
        virtual ParticleCollection getLeptons() const override;
    
    private:
        // checks if the gen particles originates from the hard scatter process
        bool isParticle(Particle p) const;

        ParticleCollection genParticles;
        int targetPdgId;
        // const bool ignoreRadiation;
};



#endif