#ifndef JET_HH
#define JET_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"



class Jet : public Particle
{
    public:
        using Particle::Particle;
        Jet(const Particle &particle);
};
#endif