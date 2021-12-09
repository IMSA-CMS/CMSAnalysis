#include "CIAnalysis/CIStudies/interface/RecoveredInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

RecoveredInvariantMassHist::RecoveredInvariantMassHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons, int iMotherPDGID) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nLeptons(iNLeptons),
  motherPDGID(iMotherPDGID)
{
}

std::vector<double> RecoveredInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.calculateRecoveredInvariantMass(nLeptons, motherPDGID);
    return 
    {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.calculateRecoveredInvariantMass(nLeptons, motherPDGID);
    return {recoInv};
  }
}
