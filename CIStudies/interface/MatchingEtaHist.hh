#ifndef MATCHINGETAHIST_HH
#define MATCHINGETAHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetMatchingModule;

class MatchingEtaHist : public HistogramPrototype
{
  public:
  MatchingEtaHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchingModule;

};

#endif