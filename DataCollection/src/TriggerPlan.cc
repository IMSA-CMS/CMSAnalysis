#include "CMSAnalysis/DataCollection/interface/TriggerPlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/Trigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/EventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"

using std::make_shared;

void TriggerPlan::initialize()
{
  Analyzer& analyzer = getAnalyzer();

  // Create objects
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  auto simTrigSingleElectron = make_shared<SimTrigger>("singleElectron", TriggerSimModule::EnumTriggers::singleElectronTriggers, trigSimMod);
  auto simTrigSingleMuon = make_shared<SimTrigger>("singleMuon", TriggerSimModule::EnumTriggers::singleMuonTriggers, trigSimMod);
  // auto simTrigPhoton = make_shared<SimTrigger>("photon", TriggerSimModule::EnumTriggers::photonTriggers, trigSimMod);
  // auto testTrig = make_shared<SingleElectronTrigger>();
  auto triggerMod = make_shared<TriggerModule>();


  auto EventDumpMod = make_shared<EventDumpModule>(true,true);


  // Configure objects
  trigSimMod->enableAllTriggers(); // For testing purposes. You should probably remove this, if I haven't already
  // simTrigSingleElectron->enableAllTriggers();
  // simTrigSingleMuon->enableAllTriggers();
  // simTrigPhoton->enableAllTriggers();
  triggerMod->addTrigger(simTrigSingleElectron);
  triggerMod->addTrigger(simTrigSingleMuon);
  // triggerMod->addTrigger(simTrigPhoton);

  // triggerMod->addTrigger(testTrig);

  // Add objects to analyzer
  analyzer.addProductionModule(trigSimMod);
  analyzer.addProductionModule(triggerMod);
  // analyzer.addProductionModule(recoTriggerMod);

  // analyzer.addAnalysisModule(EventDumpMod);

}