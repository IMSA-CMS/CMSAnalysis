#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/TriggerSimModule.hh"
#include "CIAnalysis/CIStudies/interface/SimTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"

using std::make_shared;

Analyzer triggerAnalysis()
{
  Analyzer analyzer;

  // Create objects
  auto triggerSimMod = make_shared<TriggerSimModule>();
  auto simTrigd = make_shared<SimTrigger>("doubleElectron", TriggerSimModule::EnumTriggers::doubleElectronTriggers, triggerSimMod);
  auto simTrigs = make_shared<SimTrigger>("singleMuon", TriggerSimModule::EnumTriggers::singleMuonTriggers, triggerSimMod);
  auto triggerMod = make_shared<TriggerModule>();

  // Configure objects
  triggerMod->addTrigger(simTrigd);
  triggerMod->addTrigger(simTrigs);

  // Add objects to analyzer
  analyzer.addProductionModule(triggerSimMod);
  analyzer.addProductionModule(triggerMod);

  return analyzer;
}