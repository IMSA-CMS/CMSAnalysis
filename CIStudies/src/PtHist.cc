#include "CIAnalysis/CIStudies/interface/PtHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

double PtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getGenSim().getGenParticles();
    auto genSimPt = genParticles.getLeadingTransverseMomentum();
    return genSimPt;
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getReco().getRecoCandidates();
    auto recoPt = recoParticles.getLeadingTransverseMomentum();
    return recoPt;
  }
}
