#ifndef MUON_HH
#define MUON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"

//Reconstructed Muon
//Derrives from particle and gives and interface to electron objects
class Muon : public Lepton
{
    public:
        using Lepton::Lepton;
        Muon(const Particle& particle);
};
#endif