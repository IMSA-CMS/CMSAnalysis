#ifndef TRIGGERFILTER_HH
#define TRIGGERFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "Trigger.hh"

class TriggerFilter : public Filter
{
  public:
    TriggerFilter(const std::shared_ptr<Trigger> iTrigger);
  protected:
    virtual std::string getFilterString(const InputModule* inputMod) const override;
  private:
    const std::shared_ptr<Trigger> trigger;
};

#endif