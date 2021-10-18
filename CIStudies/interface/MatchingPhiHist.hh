#ifndef MATCHINGPHIHIST_HH
#define MATCHINGPHIHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetMatchingModule;

class MatchingPhiHist : public HistogramPrototype
{
  public:
  MatchingPhiHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchingModule;

};

#endif