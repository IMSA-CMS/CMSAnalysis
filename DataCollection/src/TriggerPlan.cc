#include "CMSAnalysis/DataCollection/interface/TriggerPlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/Trigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoEventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"

using std::make_shared;

TriggerPlan::TriggerPlan()
{
  Analyzer& analyzer = getAnalyzer();

  // Create objects
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  auto simTrigd = make_shared<SimTrigger>("doubleElectron", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);
  auto simTrigs = make_shared<SimTrigger>("singleMuon", TriggerSimModule::EnumTriggers::singleMuonTriggers, trigSimMod);
  auto testTrig = make_shared<SingleElectronTrigger>();
  auto triggerMod = make_shared<TriggerModule>();


  auto genSimEventDumpMod = make_shared<GenSimEventDumpModule>();
  auto recoDumpMod = make_shared<RecoEventDumpModule>();


  // Configure objects
  simTrigd->enableAllTriggers(); // For testing purposes. You should probably remove this, if I haven't already
  simTrigs->enableAllTriggers();
  triggerMod->addTrigger(simTrigd);
  triggerMod->addTrigger(simTrigs);
  triggerMod->addTrigger(testTrig);

  // Add objects to analyzer
  analyzer.addProductionModule(trigSimMod);
  analyzer.addProductionModule(triggerMod);
  // analyzer.addProductionModule(recoTriggerMod);

  analyzer.addAnalysisModule(genSimEventDumpMod);
  analyzer.addAnalysisModule(recoDumpMod);
}