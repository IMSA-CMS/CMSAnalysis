#include "CIAnalysis/CIStudies/interface/NLeptonJetHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

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
