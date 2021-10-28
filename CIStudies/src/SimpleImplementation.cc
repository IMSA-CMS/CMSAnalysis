
#include "CIAnalysis/CIStudies/interface/SimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

SimpleImplementation::SimpleImplementation(const reco::Candidate::LorentzVector* lorentzVec)
{
  //std::cout << "Got to SI\n";
  }

reco::Candidate::LorentzVector SimpleImplementation::getFourVector() const
{
  checkIsNull();
  return particle->p4();
}

int SimpleImplementation::charge() const 
{
  checkIsNull();
  return particle->charge();
}

void SimpleImplementation::checkIsNull() const
{
  if(!particle)
  {
    throw std::runtime_error("attempted to use null pointer in Particle (SI)");	
  }
}
