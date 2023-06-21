#include "CMSAnalysis/Histograms/interface/RecoveredInvariantMassHist.hh"

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

RecoveredInvariantMassHist::RecoveredInvariantMassHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int iNLeptons, int iMotherPDGID) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nLeptons(iNLeptons),
  motherPDGID(iMotherPDGID)
{
}
std::vector<double> RecoveredInvariantMassHist::protectedValue(EventInput::RecoLevel typeGenSim) const
{
    auto particles = getInput()->getLeptons(typeGenSim);
    auto inv = particles.calculateRecoveredInvariantMass(nLeptons, motherPDGID);
    return {inv};
}
