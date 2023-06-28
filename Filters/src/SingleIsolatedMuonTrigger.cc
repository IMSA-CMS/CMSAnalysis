#include "CMSAnalysis/Filters/interface/SingleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
SingleIsolatedMuonTrigger::SingleIsolatedMuonTrigger(double iPTCutoff) :
  RecoTrigger("Single Isolated Muon Trigger"),
  pTCutoff(iPTCutoff)
{
}

bool SingleIsolatedMuonTrigger::checkTrigger(const InputModule* input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, ParticleType::muon());

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 1)
  {
    return false;
  }

  //loop through particles - see if isolated and see if > 24; passes if >=1 is okay

  for (const auto &particle : particles.getParticles())
  {
    const auto lepton = Lepton(particle);
    if (particle.getPt() >= pTCutoff && lepton.isIsolated())
    {
      return true;
    }
  }
  return false;
}
