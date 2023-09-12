#include "CMSAnalysis/Histograms/interface/LeptonJetDeltaRHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include <vector>

LeptonJetDeltaRHist::LeptonJetDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

std::vector<double> LeptonJetDeltaRHist::value() const
{
  std::vector<double> leptonJetDeltaRs;
  auto leptonJets = leptonJetRecoModule->getLeptonJets();
  for (auto leptonJet : leptonJets)
  {
    leptonJetDeltaRs.push_back(leptonJet.getDeltaR());
  }

  return leptonJetDeltaRs;
}
