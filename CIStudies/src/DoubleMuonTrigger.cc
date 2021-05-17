#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

DoubleMuonTrigger::DoubleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff, double iSecondPTCutoff) :
  Trigger("Double Muon Trigger", iRecoMod),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::LeptonType::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  double highestPT = particles.getLeadingTransverseMomentum();
  double secondHighestPT = particles.getNthHighestPt(2);

  return ((highestPT >= pTCutoff) && (secondHighestPT >= secondPTCutoff));  // The particle passes the trigger
}
