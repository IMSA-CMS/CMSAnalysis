#include "CIAnalysis/CIStudies/interface/RecoTrigger.hh"

RecoTrigger::RecoTrigger(std::string iName, std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  Trigger(iName),
  recoMod(iRecoMod)
{}
