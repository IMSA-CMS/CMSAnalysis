#ifndef LEPTON_HH
#define LEPTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class LeptonJet;


class Lepton : public Particle
{
    public:
        using Particle::Particle;

        double getIsolation() const;
        bool isIsolated() const {throw std::runtime_error("error");};

};
#endif