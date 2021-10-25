#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"

SingleMuonTrigger::SingleMuonTrigger(double iPTCutoff) :
  RecoTrigger("Single Muon Trigger"),
  pTCutoff(iPTCutoff)
{
}

bool SingleMuonTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  double pT = particles.getLeadingTransverseMomentum();


 
  return (pT >= pTCutoff);  // The particle passes the trigger if the transverse momentum is greater than the cutoff
}
