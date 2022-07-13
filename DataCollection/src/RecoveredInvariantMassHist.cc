#include "CMSAnalysis/DataCollection/interface/RecoveredInvariantMassHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

RecoveredInvariantMassHist::RecoveredInvariantMassHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons, int iMotherPDGID) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nLeptons(iNLeptons),
  motherPDGID(iMotherPDGID)
{
}
std::vector<double> RecoveredInvariantMassHist::protectedValue(InputModule::RecoLevel typeGenSim) const
{
    auto particles = getInput()->getLeptons(typeGenSim);
    auto inv = particles.calculateRecoveredInvariantMass(nLeptons, motherPDGID);
    return {inv};
}
