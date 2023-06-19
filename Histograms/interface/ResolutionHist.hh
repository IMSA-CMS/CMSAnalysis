#ifndef RESOLUTIONHIST_HH
#define RESOLUTIONHIST_HH

#include "HistogramPrototype1D.hh"
#include "LeptonJetMatchingModule.hh"
#include "LeptonJet.hh"

class LeptonJetMatchingModule;

class ResolutionHist : public HistogramPrototype1D
{
  public:
  ResolutionHist(std::shared_ptr<LeptonJetMatchingModule> ileptonJetMatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override; 
  
  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchModule;
};


#endif