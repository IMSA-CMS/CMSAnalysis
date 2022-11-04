#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"

GenSimSimpleImplementation::GenSimSimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, const Particle* motherParticle, std::vector<const GenSimParticle*> daughters):
lorentzVec(vec), 
particleCharge(ch),
particleType(type)
{

}

bool GenSimSimpleImplementation::operator== (const ParticleImplementation& other) const 
{
  try
  {
    auto otherImp = dynamic_cast<const GenSimSimpleImplementation&>(other);
    return otherImp.lorentzVec == lorentzVec && otherImp.particleCharge == particleCharge;
  }
  catch(std::bad_cast&)
  {
    return false;
  }
}

reco::Candidate::LorentzVector GenSimSimpleImplementation::getFourVector() const
{
  return lorentzVec;
}

Particle GenSimSimpleImplementation::daughter(int i) const  
{
  auto particle = *(daughters.at(i));
  Particle newPart(particle);
  return newPart;
}

int GenSimSimpleImplementation::charge() const 
{
  return particleCharge;
}

