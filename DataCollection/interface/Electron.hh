#ifndef ELECTRON_HH
#define ELECTRON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"

//Reconstructed Electron
//Derrives from particle and gives and interface to electron objects
class Electron : public Lepton
{
    public:
        using Lepton::Lepton;
        Electron(const Particle& particle);
};
#endif