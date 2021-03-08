#ifndef TRIGGERMODULE_HH
#define TRIGGERMODULE_HH

#include <memory>

#include "ProductionModule.hh"

class Trigger;
class RecoIdentificationModule;

class TriggerModule : public ProductionModule
{
public:
  TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod);
  virtual bool process(const edm::EventBase& event) override;
  virtual void finalize() override;
  void addTrigger(std::shared_ptr<Trigger> trigger) {triggers.push_back(trigger);}
  void printOverallEfficiency();

private:
  const std::shared_ptr<RecoIdentificationModule> recoMod;
  std::vector<std::shared_ptr<Trigger>> triggers;
  int passAny;
  int total;
};

#endif
