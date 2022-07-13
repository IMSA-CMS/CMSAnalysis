#include "CMSAnalysis/DataCollection/interface/GetNthHighestPtHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

GetNthHighestPtHist::GetNthHighestPtHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nthMuon(inthMuon)
{}


std::vector<double> GetNthHighestPtHist::protectedValue(InputModule::RecoLevel typeGenSim) const
{
  auto particles = getInput()->getLeptons(typeGenSim);
  auto pt = particles.getNthHighestPt(nthMuon);
  //std::cerr << "Reco pT: " << recoPt << std::endl;
  return {pt};
}
