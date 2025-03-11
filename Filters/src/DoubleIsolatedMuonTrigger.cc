#include "CMSAnalysis/Filters/interface/DoubleIsolatedMuonTrigger.hh"

//#include "CMSAnalysis/DataCollection/src/RecoIdentificationModule.cc"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"


DoubleIsolatedMuonTrigger::DoubleIsolatedMuonTrigger(double iPTCutoff, double iSecondPTCutoff) :
  RecoTrigger("Double Isolated Muon Trigger"),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleIsolatedMuonTrigger::checkTrigger(const EventInput* input)
{
  auto particles = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  bool passedFirstCut = false;
  bool passedSecondCut = false;

  for (const Lepton particle : particles.getParticles())
  {
    if (particle.getPt() >= pTCutoff && particle.isIsolated())
    {
      passedFirstCut = true;
    }
    else if (particle.getPt() >= secondPTCutoff && particle.isIsolated())
    {
      passedSecondCut = true;
    }
  }

  //double highestPT = particles.getLeadingTransverseMomentum();
  //double secondHighestPT = particles.getNthHighestPt(2);

  return (passedFirstCut && passedSecondCut);  // The particle passes the trigger
}
