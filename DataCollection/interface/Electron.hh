#ifndef ELECTRON_HH
#define ELECTRON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"

class Electron : public Lepton
{
    public:
        using Lepton::Lepton;
        Electron(const Particle& particle);
};
#endif