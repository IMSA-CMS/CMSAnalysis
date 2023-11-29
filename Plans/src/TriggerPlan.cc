#include "CMSAnalysis/Plans/interface/TriggerPlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Filters/interface/Trigger.hh"
#include "CMSAnalysis/Modules/interface/TriggerSimModule.hh"
#include "CMSAnalysis/Modules/interface/SimTrigger.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Filters/interface/RecoTrigger.hh"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Filters/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"

using std::make_shared;

void TriggerPlan::initialize()
{
  auto& modules = getModules();

  // Create objects
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  //auto trigSimMod = make_shared<ProductionModule>("HLT");
  // add lines from my trigger data [insert!]
  auto simTrigSingleElectron = make_shared<SimTrigger>("singleElectron", TriggerSimModule::EnumTriggers::singleElectronTriggers, trigSimMod);
  auto simTrigSingleMuon = make_shared<SimTrigger>("singleMuon", TriggerSimModule::EnumTriggers::singleMuonTriggers, trigSimMod);
  // added new parts like MuonElectron
  // auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
  // auto higgsFilterMod = make_shared<FilterModule>(higgsFilter);
  auto simTrigMuonElectron = make_shared<SimTrigger>("muonElectron", TriggerSimModule::EnumTriggers::muonElectronTriggers, trigSimMod);
  auto simTrigDoubleMuon = make_shared<SimTrigger>("doubleMuon", TriggerSimModule::EnumTriggers::doubleMuonTriggers, trigSimMod);
  auto simTrigDoubleElectron = make_shared<SimTrigger>("doubleElectron", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);
  // auto simTrigPhoton = make_shared<SimTrigger>("photon", TriggerSimModule::EnumTriggers::photonTriggers, trigSimMod);
  // auto testTrig = make_shared<SingleElectronTrigger>();
  auto triggerMod = make_shared<TriggerModule>();


  auto EventDumpMod = make_shared<EventDumpModule>(true,true);


  // Configure objects
  //trigSimMod->enableAllTriggers();
  trigSimMod->disableAllTriggers(); // For testing purposes. You should probably remove this, if I haven't already
  // simTrigSingleElectron->enableAllTriggers();
  // simTrigSingleMuon->enableAllTriggers();
  // simTrigPhoton->enableAllTriggers();
  // USE these lines as template add my own lines
  //triggerMod->addTrigger(simTrigSingleElectron);
  triggerMod->addTrigger(simTrigSingleMuon);
  triggerMod->addTrigger(simTrigDoubleMuon);
  // triggerMod->addTrigger(simTrigDoubleElectron);
  // triggerMod->addTrigger(simTrigMuonElectron);
  // triggerMod->addTrigger(simTrigDoubleMuon);
  // triggerMod->addTrigger(simTrigPhoton);

  // triggerMod->addTrigger(testTrig);

  // // Add objects to analyzer
  // analyzer.addProductionModule(trigSimMod);
   modules.addAnalysisModule(triggerMod);
  // // analyzer.addProductionModule(recoTriggerMod);

  // // analyzer.addAnalysisModule(EventDumpMod);
  // // analyzer.addFilterModule(higgsFilterMod);

  
  // Add objects to modules
  modules.addProductionModule(trigSimMod);
  // modules.addProductionModule(triggerMod);
  // modules.addProductionModule(recoTriggerMod);

  // modules.addAnalysisModule(EventDumpMod);

}