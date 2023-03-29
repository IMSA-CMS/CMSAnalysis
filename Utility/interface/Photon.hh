#ifndef PHOTON_HH
#define PHOTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"


class LeptonJet;


class Photon : public Particle
{
    public:
        using Particle::Particle;
        Photon(const Particle &particle);
};
#endif