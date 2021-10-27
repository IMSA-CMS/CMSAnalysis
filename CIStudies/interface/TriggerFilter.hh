#ifndef TRIGGERFILTER_HH
#define TRIGGERFILTER_HH

#include <memory>
#include "FilterModule.hh"
#include "Trigger.hh"

class RecoIdentificationModule;

class TriggerFilter : public FilterModule
{
  public:
    TriggerFilter(const std::shared_ptr<Trigger> iTrigger);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const std::shared_ptr<Trigger> trigger;
};

#endif