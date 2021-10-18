#include "CIAnalysis/CIStudies/interface/DoubleElectronTrigger.hh"

DoubleElectronTrigger::DoubleElectronTrigger(double iPTCutoff, double iSecondPTCutoff) :
  Trigger("Double Electron Trigger"),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleElectronTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Electron);

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
