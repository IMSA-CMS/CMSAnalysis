#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

SingleMuonTrigger::SingleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, double iPTCutoff) :
  Trigger(iRecoMod),
  pTCutoff(iPTCutoff)
{
}

bool SingleMuonTrigger::checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  auto particles = recoMod->getRecoCandidates(Particle::LeptonType::Muon);
  double pT = particles.getLeadingTransverseMomentum();

  if (pT >= pTCutoff)  // The particle passes the trigger
  {
    return true;
  }

  else
  {
    return false;
  }
}
