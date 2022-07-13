#ifndef LEPTON_HH
#define LEPTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"


class LeptonJet;


class Lepton : public Particle
{
    public:
        using Particle::Particle;
        Lepton(const Particle &particle);
        double getIsolation() const;
        bool isIsolated() const {throw std::runtime_error("error");};
};
#endif