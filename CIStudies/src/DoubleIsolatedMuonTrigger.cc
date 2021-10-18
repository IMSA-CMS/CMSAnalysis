#include "CIAnalysis/CIStudies/interface/DoubleIsolatedMuonTrigger.hh"

//#include "CIAnalysis/CIStudies/src/RecoIdentificationModule.cc"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"


DoubleIsolatedMuonTrigger::DoubleIsolatedMuonTrigger(double iPTCutoff, double iSecondPTCutoff) :
  Trigger("Double Isolated Muon Trigger"),
  pTCutoff(iPTCutoff),
  secondPTCutoff(iSecondPTCutoff)
{
}

bool DoubleIsolatedMuonTrigger::checkTrigger(std::shared_ptr<InputModule> input)
{
  auto particles = input->getParticles(InputModule::RecoLevel::Reco, Particle::Type::Muon);

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  bool passedFirstCut = false;
  bool passedSecondCut = false;

  for (const auto &particle : particles.getParticles())
  {
    if (particle.pt() >= pTCutoff && particle.isIsolated())
    {
      passedFirstCut = true;
    }
    else if (particle.pt() >= secondPTCutoff && particle.isIsolated())
    {
      passedSecondCut = true;
    }
  }

  //double highestPT = particles.getLeadingTransverseMomentum();
  //double secondHighestPT = particles.getNthHighestPt(2);

  return (passedFirstCut && passedSecondCut);  // The particle passes the trigger
}
