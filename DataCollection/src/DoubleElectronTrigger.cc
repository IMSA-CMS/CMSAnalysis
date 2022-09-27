#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"

DoubleElectronTrigger::DoubleElectronTrigger(double iPTCutoff, double iSecondPTCutoff) :
  RecoTrigger("Double Electron Trigger"),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleElectronTrigger::checkTrigger(const InputModule* input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, ParticleType::electron());

  // If there aren't enough electrons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  double highestPT = particles.getLeadingTransverseMomentum();
  double secondHighestPT = particles.getNthHighestPt(2);

  // The particle passes the trigger
  return ((highestPT >= pTCutoff) && (secondHighestPT >= secondPTCutoff));  
}
