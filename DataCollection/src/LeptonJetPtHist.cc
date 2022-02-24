#include "CMSAnalysis/DataCollection/interface/LeptonJetPtHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

#include <vector>

LeptonJetPtHist::LeptonJetPtHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

std::vector<double> LeptonJetPtHist::value() const
{
  const std::vector<double>& pTValues = leptonJetRecoModule->getPtValues();
  if (pTValues.size() > 0)
  {
    return {pTValues[0]};
  }
  else
  {
    return {-1};
  }  
}
