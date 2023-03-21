#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"

GenSimSimpleImplementation::GenSimSimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, int ipdgId, const Particle* imotherParticle, std::vector<const GenSimParticle*> idaughters, const int status):
lorentzVec(vec), 
particleCharge(ch),
particleType(type),
currentPdgId(ipdgId),
motherParticle(imotherParticle),
daughters(idaughters),
particleStatus(status)
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

int GenSimSimpleImplementation::pdgId() const
{
  return particleCharge * currentPdgId;
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

bool GenSimSimpleImplementation::doesHaveMother() const
{
  if( motherParticle == nullptr)
  {
    return false;
  }
  return true;
}
int GenSimSimpleImplementation::charge() const 
{
  return particleCharge;
}

