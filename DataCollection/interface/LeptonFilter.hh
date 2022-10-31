#ifndef LEPTONFILTER_HH
#define LEPTONFILTER_HH

#include "FilterModule.hh"

#include "Particle.hh"

// Filters all events with at least nParticles of type type
class LeptonFilter : public FilterModule
{
    public:
        LeptonFilter(const ParticleType& type, int nParticles, std::string outputString);

    protected:
        virtual std::string makeFilterString() override;

    private:
        const ParticleType& type;
        int nParticles;
        std::string outputString;
};

#endif