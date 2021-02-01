#ifndef NLEPTONSHIST_HH
#define NLEPTONSHIST_HH

#include "HistogramPrototype.hh"

class MatchingModule;

class NLeptonsHist : public HistogramPrototype
{
  public:
  NLeptonsHist(const std::shared_ptr<MatchingModule> imatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum, int itargetPdgId = 0);
  double value() const override;

  private:
  const std::shared_ptr<MatchingModule> matchModule;
  int targetPdgId;
};

#endif
