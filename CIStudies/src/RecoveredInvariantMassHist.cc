#include "CIAnalysis/CIStudies/interface/RecoveredInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

RecoveredInvariantMassHist::RecoveredInvariantMassHist(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons) :
  GenSimRecoPrototype(genSimModule, recoModule, typeSwitch, iname, iNBins, iminimum, imaximum),
  nLeptons(iNLeptons)
{
}

double RecoveredInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getGenSim()->getGenParticles();
    auto genSimInv = genParticles.calculateRecoveredInvariantMass(nLeptons);
    return genSimInv;
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getReco()->getRecoCandidates();
    auto recoInv = recoParticles.calculateRecoveredInvariantMass(nLeptons);
    return recoInv;
  }
}
