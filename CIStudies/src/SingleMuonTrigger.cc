#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

SingleMuonTrigger::SingleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff) :
  RecoTrigger("Single Muon Trigger", iRecoMod),
  pTCutoff(iPTCutoff)
{
}

bool SingleMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::Type::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  double pT = particles.getLeadingTransverseMomentum();


 
  return (pT >= pTCutoff);  // The particle passes the trigger if the transverse momentum is greater than the cutoff
}
