#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"

#include "CIAnalysis/CIStudies/interface/Trigger.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TriggerModule::TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  recoMod(iRecoMod)
{
}

bool TriggerModule::process(const edm::EventBase& event)
{
  return true;
}
