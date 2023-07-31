#include "CMSAnalysis/Filters/interface/DoubleMuonTrigger.hh"
DoubleMuonTrigger::DoubleMuonTrigger(double iPTCutoff, double iSecondPTCutoff) : RecoTrigger("Double Muon Trigger"),
                                                                                 pTCutoff(iPTCutoff),
                                                                                 secondPTCutoff(iSecondPTCutoff){};

bool DoubleMuonTrigger::checkTrigger(const EventInput *input)
{
  auto particles = input->getParticles(EventInput::RecoLevel::Reco, ParticleType::muon());

  // If there aren't enough muons, then automatically fail the trigger
  if (particles.getNumParticles() < 2)
  {
    return false;
  }

  double highestPT = particles.getLeadingTransverseMomentum();
  double secondHighestPT = particles.getNthHighestPt(2);

  return ((highestPT >= pTCutoff) && (secondHighestPT >= secondPTCutoff)); // The particle passes the trigger
};
