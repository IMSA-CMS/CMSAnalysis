#ifndef MATCHINGDELTARHIST_HH
#define MATCHINGDELTARHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetMatchingModule;

class MatchingDeltaRHist : public HistogramPrototype
{
  public:
  MatchingDeltaRHist(const LeptonJetMatchingModule& iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const LeptonJetMatchingModule& leptonJetMatchingModule;

};

#endif