#ifndef ISMEASUREDCHARGEFILTER_HH
#define MISMEASUREDCHARGEFILTER_HH

#include <memory>
#include "FilterModule.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class MatchingModule;

class MismeasuredChargeFilter : public FilterModule
{
  public:
    MismeasuredChargeFilter();
  protected:
    virtual std::string makeFilterString() override;
  private:
};

#endif
