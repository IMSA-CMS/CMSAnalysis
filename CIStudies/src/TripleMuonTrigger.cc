#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"

TripleMuonTrigger::TripleMuonTrigger(double iPTCutoff, double iSecondPTCutoff, double iThirdPTCutoff) :
  RecoTrigger("Triple Muon Trigger"),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff),
  thirdPTCutoff(iThirdPTCutoff)
{
}

bool TripleMuonTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon);

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
