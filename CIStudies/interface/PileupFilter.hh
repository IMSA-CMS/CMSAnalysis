#ifndef PILEUPFILTER_HH
#define PILEUPFILTER_HH

#include "FilterModule.hh"

class PileupFilter : public FilterModule
{
public:
  PileupFilter(double loCut, double hiCut);

protected:
  virtual std::string makeFilterString(const edm::EventBase& event);

private:
  double lowCutoff;
  double highCutoff;
};

#endif
