#include "CMSAnalysis/DataCollection/interface/RecoveredInvariantMassHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

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
