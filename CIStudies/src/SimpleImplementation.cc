
#include "CIAnalysis/CIStudies/interface/SimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

SimpleImplementation::SimpleImplementation(reco::Candidate::LorentzVector vec, int ch, Particle::Type type):
lorentzVec(vec), 
particleCharge(ch),
particleType(type)
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
  return lorentzVec;
}

int SimpleImplementation::charge() const 
{
  return particleCharge;
}

