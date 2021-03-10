#include "CIAnalysis/CIStudies/interface/DeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

#include <vector>

DeltaRHist::DeltaRHist(const LeptonJetReconstructionModule& iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

double DeltaRHist::value() const
{
  const std::vector<double>& deltaRValues = leptonJetRecoModule.getDeltaRValues();
  if (deltaRValues.size() > 0)
  {
      return deltaRValues[0];
  }
  else
  {
      return -1;
  }  
}