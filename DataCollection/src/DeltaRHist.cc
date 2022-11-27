#include "CMSAnalysis/DataCollection/interface/DeltaRHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"

#include <vector>

DeltaRHist::DeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

std::vector<double> DeltaRHist::value() const
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