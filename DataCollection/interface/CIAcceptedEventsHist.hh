#ifndef CIACCEPTEDHIST_HH
#define CIACCEPTEDHIST_HH

#include "HistogramPrototype1D.hh"

class MatchingModule;

class CIAcceptedEventsHist : public HistogramPrototype1D
{
  public:
  CIAcceptedEventsHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  const std::shared_ptr<MatchingModule> matchMod;
  
};


#endif
