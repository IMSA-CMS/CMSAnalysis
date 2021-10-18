#ifndef CIACCEPTEDHIST_HH
#define CIACCEPTEDHIST_HH

#include "HistogramPrototype.hh"

class MatchingModule;
//class GenSimIdentificationModule;

class CIAcceptedEventsHist : public HistogramPrototype
{
  public:
  CIAcceptedEventsHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  const std::shared_ptr<MatchingModule> matchMod;
  // const std::shared_ptr<GenSimIdentificationModule> genSimIdentification;
};


#endif
