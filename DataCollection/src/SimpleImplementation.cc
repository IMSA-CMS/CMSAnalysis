
#include "CMSAnalysis/DataCollection/interface/SimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

SimpleImplementation::SimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, double iIsolation, Lepton::SelectionFit fit):
lorentzVec(vec), 
particleCharge(ch),
particleType(type),
particleIsolation(iIsolation),
selectionFit(fit)
{

  //std::cout << "Got to SI\n";
}

bool SimpleImplementation::operator== (const ParticleImplementation& other) const 
{
    try
    {
      auto otherImp = dynamic_cast<const SimpleImplementation&>(other);
      return otherImp.lorentzVec == lorentzVec && otherImp.particleCharge == particleCharge;
    }
    catch(std::bad_cast&)
    {
      return false;
    }
}

reco::Candidate::LorentzVector SimpleImplementation::getFourVector() const
{
  // HELP WHY ARE THE MASSES NEGATIVE [ATANG, 11/07]
  // std::cout << lorentzVec.mass() << std::endl;
  return lorentzVec;
}

int SimpleImplementation::charge() const 
{
  return particleCharge;
}

