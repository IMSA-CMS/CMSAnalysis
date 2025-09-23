#ifndef TAU_HH
#define TAU_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"

//Reconstructed Muon
//Derrives from particle and gives and interface to electron objects
class Tau : public Lepton
{
    public:
        using Lepton::Lepton;
        Tau(const Particle& particle);
};
#endif