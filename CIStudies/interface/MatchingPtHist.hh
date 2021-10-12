#ifndef MATCHINGPTHIST_HH
#define MATCHINGPTHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetMatchingModule;

class MatchingPtHist : public HistogramPrototype1D
{
  public:
  MatchingPtHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchingModule;

};

#endif
