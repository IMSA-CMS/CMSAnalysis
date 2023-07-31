#ifndef COLLINSSOPERFILTER_HH
#define COLLINSSOPERFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"


// MassFilter applies a high mass cut based on the invariant mass of the gen sim particles
class CollinsSoperFilter : public Filter
{
public:
  CollinsSoperFilter(double hiCut);

protected:
  virtual std::string getFilterString(const EventInput* inputMod) const override;

private:
  double highCutoff;
};

#endif
