#include "CIAnalysis/CIStudies/interface/LeptonJetPtHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"

#include <vector>

LeptonJetPtHist::LeptonJetPtHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
  
{
}

double LeptonJetPtHist::value() const
{
  const std::vector<double>& pTValues = leptonJetRecoModule->getPtValues();
  if (pTValues.size() > 0)
  {
    return pTValues[0];
  }
  else
  {
    return -1;
  }  
}