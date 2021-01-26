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
  bool getPassSingleElectron() const {return passSingleElectron;}

private:
  const std::shared_ptr<RecoIdentificationModule> recoMod;

  // Trigger pass
  bool passSingleElectron;
  bool passSingleMuon;

  std::vector<std::shared_ptr<Trigger>> triggers;
};

#endif
