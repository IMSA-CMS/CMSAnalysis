#ifndef PHOTON_HH
#define PHOTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/RecoParticle.hh"


class LeptonJet;


class Photon : public RecoParticle
{
    public:
        using RecoParticle::RecoParticle;
        Photon(const Particle &particle);
};
#endif