#ifndef UNUSUALFINALSTATEFILTER_HH
#define UNUSUALFINALSTATEFILTER_HH

#include <memory>
#include "FilterModule.hh"

class RecoIdentificationModule;

class UnusualFinalStateFilter : public FilterModule
{
  public:
    UnusualFinalStateFilter();
  protected:
    virtual std::string makeFilterString() override;
  private:
};

#endif
