#ifndef MASSFILTER_HH
#define MASSFILTER_HH

#include "FilterModule.hh"

class GenSimIdentificationModule;

// MassFilter applies a high mass cut based on the invariant mass of the gen sim particles
class MassFilter : public FilterModule
{
public:
  MassFilter(const GenSimIdentificationModule& genSimModule, double hiCut);

protected:
  virtual std::string makeFilterString(const edm::EventBase& event) override;

private:
  const GenSimIdentificationModule& genSim;
  double highCutoff;
};

#endif
