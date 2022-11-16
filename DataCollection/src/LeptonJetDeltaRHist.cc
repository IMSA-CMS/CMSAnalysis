#include "CMSAnalysis/DataCollection/interface/LeptonJetDeltaRHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

#include <vector>

LeptonJetDeltaRHist::LeptonJetDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

std::vector<double> LeptonJetDeltaRHist::value() const
{
  const std::vector<double>& deltaRValues = leptonJetRecoModule->getDeltaRValues();
  if (deltaRValues.size() > 0)
  {
      return {deltaRValues[0]};
  }
  else
  {
      return {-1};
  }  
}