#include "CMSAnalysis/Histograms/interface/NLeptonJetHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include <vector>

NLeptonJetHist::NLeptonJetHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

std::vector<double> NLeptonJetHist::value() const
{
  const std::vector<LeptonJet>& leptonJets = leptonJetRecoModule->getLeptonJets();
  return {static_cast<double>(leptonJets.size())};
}
