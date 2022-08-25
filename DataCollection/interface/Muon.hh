#ifndef MUON_HH
#define MUON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"

class Muon : public Lepton
{
    public:
        using Lepton::Lepton;
        Muon(const Particle& particle);
};
#endif