#ifndef COLLINSSOPERFILTER_HH
#define COLLINSSOPERFILTER_HH

#include "FilterModule.hh"


// MassFilter applies a high mass cut based on the invariant mass of the gen sim particles
class CollinsSoperFilter : public FilterModule
{
public:
  CollinsSoperFilter(double hiCut);

protected:
  virtual std::string makeFilterString() override;

private:
  double highCutoff;
};

#endif
