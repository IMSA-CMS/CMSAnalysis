#ifndef PILEUPFILTER_HH
#define PILEUPFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"

class PileupFilter : public Filter
{
public:
  PileupFilter(double loCut, double hiCut);

protected:
  virtual std::string getFilterString(const InputModule* inputMod) const override;

private:
  double lowCutoff;
  double highCutoff;
};

#endif