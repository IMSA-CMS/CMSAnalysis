#include "CIAnalysis/CIStudies/interface/Trigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

Trigger::Trigger(std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  recoMod(iRecoMod)
{
}

bool Trigger::checkEvent(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  ++total;
  
  if (checkTrigger(recoMod))
  {
    ++pass;
    return true;
  }

  else
  {
    return false;
  }
}
