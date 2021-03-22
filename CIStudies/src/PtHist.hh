#include "CIAnalysis/CIStudies/interface/PtHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

PtHist::InvariantMassHist(const GenSimIdentificationModule& iGenSim, const RecoIdentificationModule& iReco, bool iTypeGenSim) :
  GenSimRecoPrototype(iGenSim, iReco, iTypeGenSim)
{
}

PtHistHist::protectedValue(bool typeGenSim)
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = genSim.getGenParticles();
    auto genSimPt = genParticles.getLeadingTransverseMomentum();
    return genSimPt;
  }

  else if (!typeGenSim)    // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = reco.getRecoCandidates();
    auto recoPt = recoParticles.getLeadingTransverseMomentum();
    return recoPt;
  }
}
