#ifndef ISMEASUREDCHARGEFILTER_HH
#define MISMEASUREDCHARGEFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class MatchingModule;

class MismeasuredChargeFilter : public Filter
{
  public:
    MismeasuredChargeFilter();
  protected:
    virtual std::string getFilterString(const EventInput* inputMod) const override;
  private:
};

#endif
