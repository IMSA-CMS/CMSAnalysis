#include "CIAnalysis/CIStudies/interface/SingleElectronTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

SingleElectronTrigger::SingleElectronTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff) :
  RecoTrigger("Single Electron Trigger", iRecoMod),
  pTCutoff(iPTCutoff)
{
}

bool SingleElectronTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::Type::Electron);

  // If there aren't enough electrons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  double pT = particles.getLeadingTransverseMomentum();

  // The particle passes the trigger if the transverse momentum is greater than the cutoff
  return (pT >= pTCutoff);  
}
