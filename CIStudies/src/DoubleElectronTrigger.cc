#include "CIAnalysis/CIStudies/interface/DoubleElectronTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

DoubleElectronTrigger::DoubleElectronTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff, double iSecondPTCutoff) :
  Trigger("Double Electron Trigger", iRecoMod),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleElectronTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::Type::Electron);

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
