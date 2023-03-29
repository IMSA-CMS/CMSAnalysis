#ifndef ELECTRON_HH
#define ELECTRON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"

class Electron : public Lepton
{
    public:
        using Lepton::Lepton;
        Electron(const Particle& particle);
};
#endif