#ifndef MASSFILTER_HH
#define MASSFILTER_HH

#include "FilterModule.hh"


// MassFilter applies a high mass cut based on the invariant mass of the gen sim particles
class MassFilter : public FilterModule
{
public:
  MassFilter(double hiCut);

protected:
  virtual std::string makeFilterString() override;

private:
  double highCutoff;
};

#endif
