
#include "CMSAnalysis/Utility/interface/SimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

SimpleImplementation::SimpleImplementation(reco::Candidate::LorentzVector vec, double dxy, double dz, int ch, const ParticleType& type, Particle::SelectionFit fit):
lorentzVec(vec), 
deltaXY(dxy),
deltaZ(dz),
particleCharge(ch),
particleType(type),
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



