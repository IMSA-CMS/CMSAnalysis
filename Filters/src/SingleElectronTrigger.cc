#include "CMSAnalysis/Filters/interface/SingleElectronTrigger.hh"

SingleElectronTrigger::SingleElectronTrigger(double iPTCutoff) : RecoTrigger("Single Electron Trigger"),
                                                                 pTCutoff(iPTCutoff){};

bool SingleElectronTrigger::checkTrigger(const EventInput *input)
{
  auto particles = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::electron());

  // If there aren't enough electrons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  double pT = particles.getLeadingTransverseMomentum();

  // The event passes the trigger if the transverse momentum is greater than the cutoff
  return (pT >= pTCutoff);
};
