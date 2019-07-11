#ifndef MATCHINGMODULE_HH
#define MATCHINGMODULE_HH

#include "ProductionModule.hh"

class MatchingModule : public ProductionModule
{
public:
  MatchingModule(const GenSimIdentificationModule& gensimModule);
  virtual void process(const edm::EventBase& event) override;

private:
  const GenSimIdentificationModule& genSim;
};

#endif
