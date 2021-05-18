#include "CIAnalysis/CIStudies/interface/SingleIsolatedMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

SingleIsolatedMuonTrigger::SingleIsolatedMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff) :
  Trigger("Single Isolated Muon Trigger", iRecoMod),
  pTCutoff(iPTCutoff)
{
}

bool SingleIsolatedMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::LeptonType::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  //loop through particles - see if isolated and see if > 24; passes if >=1 is okay

  for (const auto &particle : particles.getParticles())
  {
    if (particle.pt() >= pTCutoff && particle.isIsolated())
    {
      return true;
    }
  }
  return false;
}
