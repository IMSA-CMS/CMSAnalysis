#ifndef MATCHINGDELTARHIST_HH
#define MATCHINGDELTARHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
//#define HistogramPrototype1D_HH

class LeptonJetMatchingModule;

class MatchingDeltaRHist : public HistogramPrototype1D
{
  public:
  MatchingDeltaRHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchingModule;

};

#endif
