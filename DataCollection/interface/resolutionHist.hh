#ifndef RESOLUTIONHIST_HH
#define RESOLUTIONHIST_HH

#include "HistogramPrototype1D.hh"
#include "LeptonJetMatchingModule.hh"
#include "LeptonJet.hh"


class resolutionHist : public HistogramPrototype1D
{
  public:
  resolutionHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum, std::string ianalysis);
  std::vector<double> value() const override;
  std::vector<double> RecoGamma(std::vector<LeptonJet>) 
  
  private:
  std::shared_ptr<LeptonJetMatchingModule> LeptonJetMatchingModule;
  std::string analysis;

};


#endif