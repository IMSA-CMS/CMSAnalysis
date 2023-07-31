#ifndef LEPTONFILTER_HH
#define LEPTONFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

// Filters all events with at least nParticles of type type
class LeptonFilter : public Filter
{
    public:
        LeptonFilter(const ParticleType& type, int nParticles, std::string outputString);

    protected:
        virtual std::string getFilterString(const EventInput* inputMod) const override;

    private:
        const ParticleType& type;
        int nParticles;
        std::string outputString;
};

#endif