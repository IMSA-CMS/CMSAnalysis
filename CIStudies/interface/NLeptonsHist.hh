#ifndef NLEPTONSHIST_HH
#define NLEPTONSHIST_HH

#include "HistogramPrototype.hh"

class MatchingModule;

class NLeptonsHist : public HistogramPrototype
{
  public:
  NLeptonsHist(const MatchingModule& imatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const MatchingModule& matchModule;
};

#endif
