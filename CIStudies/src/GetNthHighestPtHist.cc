#include "CIAnalysis/CIStudies/interface/GetNthHighestPtHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

GetNthHighestPtHist::GetNthHighestPtHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nthMuon(inthMuon)
{}


std::vector<double> GetNthHighestPtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    auto genSimPt = genParticles.getNthHighestPt(nthMuon);
    //std::cerr << "GenSim pT: " << genSimPt << std::endl;
    return {genSimPt};
  }


  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto recoPt = recoParticles.getNthHighestPt(nthMuon);
    //std::cerr << "Reco pT: " << recoPt << std::endl;
    return {recoPt};
  }
}
