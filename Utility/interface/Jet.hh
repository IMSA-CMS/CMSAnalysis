#ifndef JET_HH
#define JET_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/RecoParticle.hh"



class Jet : public RecoParticle
{
    public:
        using RecoParticle::RecoParticle;
        Jet(const Particle &particle);

        // bool isBJet();
};
#endif

// add accessor here