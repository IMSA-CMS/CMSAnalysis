#include "CIAnalysis/CIStudies/interface/SingleElectronTrigger.hh"

SingleElectronTrigger::SingleElectronTrigger(double iPTCutoff) :
  Trigger("Single Electron Trigger"),
  pTCutoff(iPTCutoff)
{
}

bool SingleElectronTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Electron);

  // If there aren't enough electrons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  double pT = particles.getLeadingTransverseMomentum();

  // The particle passes the trigger if the transverse momentum is greater than the cutoff
  return (pT >= pTCutoff);  
}
