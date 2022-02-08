#include "CMSAnalysis/DataCollection/interface/GetNthHighestPtHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

GetNthHighestPtHist::GetNthHighestPtHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nthMuon(inthMuon)
{}


std::vector<double> GetNthHighestPtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimPt = genParticles.getNthHighestPt(nthMuon);
    //std::cerr << "GenSim pT: " << genSimPt << std::endl;
    return {genSimPt};
  }


  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoPt = recoParticles.getNthHighestPt(nthMuon);
    //std::cerr << "Reco pT: " << recoPt << std::endl;
    return {recoPt};
  }
}
