#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TripleMuonTrigger::TripleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff, double iSecondPTCutoff, double iThirdPTCutoff) :
  Trigger("Triple Muon Trigger", iRecoMod),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff),
  thirdPTCutoff(iThirdPTCutoff)
{
}

bool TripleMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::LeptonType::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 3)
  {
    return false;
  }

  double highestPT = particles.getLeadingTransverseMomentum();
  double secondHighestPT = particles.getNthHighestPt(2);
  double thirdHighestPT = particles.getNthHighestPt(3);

  return ((highestPT >= pTCutoff) && (secondHighestPT >= secondPTCutoff) && (thirdHighestPT >= thirdPTCutoff));
}
