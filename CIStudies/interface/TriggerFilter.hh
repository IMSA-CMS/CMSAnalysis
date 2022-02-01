#ifndef TRIGGERFILTER_HH
#define TRIGGERFILTER_HH

#include <memory>
#include "FilterModule.hh"
#include "Trigger.hh"

class TriggerFilter : public FilterModule
{
  public:
    TriggerFilter(const std::shared_ptr<Trigger> iTrigger);
  protected:
    virtual std::string makeFilterString() override;
  private:
    const std::shared_ptr<Trigger> trigger;
};

#endif