#include "CIAnalysis/CIStudies/interface/SingleIsolatedMuonTrigger.hh"

SingleIsolatedMuonTrigger::SingleIsolatedMuonTrigger(double iPTCutoff) :
  RecoTrigger("Single Isolated Muon Trigger"),
  pTCutoff(iPTCutoff)
{
}

bool SingleIsolatedMuonTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  //loop through particles - see if isolated and see if > 24; passes if >=1 is okay

  for (const auto &particle : particles.getParticles())
  {
    if (particle.getPt() >= pTCutoff && particle.isIsolated())
    {
      return true;
    }
  }
  return false;
}
