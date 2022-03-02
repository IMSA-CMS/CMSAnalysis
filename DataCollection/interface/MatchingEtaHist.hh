#ifndef MATCHINGETAHIST_HH
#define MATCHINGETAHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetMatchingModule;

class MatchingEtaHist : public HistogramPrototype1D
{
  public:
  MatchingEtaHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetMatchingModule> leptonJetMatchingModule;

};

#endif
