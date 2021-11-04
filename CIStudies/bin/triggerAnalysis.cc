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
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  auto simTrigd = make_shared<SimTrigger>("doubleElectron", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);
  auto simTrigs = make_shared<SimTrigger>("singleMuon", TriggerSimModule::EnumTriggers::singleMuonTriggers, trigSimMod);
  auto triggerMod = make_shared<TriggerModule>();

  // Configure objects
  simTrigd->enableAllTriggers(); // For testing purposes. You should probably remove this, if I haven't already
  triggerMod->addTrigger(simTrigd);
  triggerMod->addTrigger(simTrigs);

  // Add objects to analyzer
  analyzer.addProductionModule(trigSimMod);
  analyzer.addProductionModule(triggerMod);

  return analyzer;
}