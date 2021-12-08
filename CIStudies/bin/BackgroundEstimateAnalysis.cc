#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/DoubleElectronTrigger.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/PhotonsHist.hh"
#include "CIAnalysis/CIStudies/interface/RecoveredInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/SingleElectronTrigger.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerEfficiencyModule.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TwoInvariantMassesHist.hh"
#include "CIAnalysis/CIStudies/interface/UnusualFinalStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/SnowmassLeptonSelector.hh"

using std::make_shared;

Analyzer backgroundEstimateAnalysis() {
  Analyzer analyzer;

  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>();

  auto unusualFinalStateFilter = make_shared<UnusualFinalStateFilter>();
  
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = make_shared<LeptonEfficiency>(weightMod, matchMod);

  // auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(true, "GenSim Same Sign Invariant Mass", 100, 0, 1000);
  // Go up to 2000 - Andy, 09/02 - and make more bins. Modifications also made for picking files
  auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "Reco Same Sign Invariant Mass", 1000, 0, 2000);

  auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 1000, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(recoSameSignInvMassHist);
  histMod->addHistogram(positiveNegativeInvMassHist);

  // Initialize triggers
  auto singleMuonTrigger = make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = make_shared<DoubleMuonTrigger>(37, 27);
  auto singleElectronTrigger = make_shared<SingleElectronTrigger>(28);
  auto doubleElectronTrigger = make_shared<DoubleElectronTrigger>(25, 25);

  // Add triggers to the TriggerModule
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(singleElectronTrigger);
  triggerMod->addTrigger(doubleElectronTrigger);

  // analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(triggerMod);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);

  // Filters
  // 09/12: Add nLeptons filer 
  analyzer.addFilterModule(nLeptonsFilter);
  //analyzer.addFilterModule(unusualFinalStateFilter);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
  //analyzer.addAnalysisModule(eventDump);
  //analyzer.addAnalysisModule(leptonEfficiency);

  auto selector = std::make_shared<SnowmassLeptonSelector>(50);
  analyzer.getInputModule().setLeptonSelector(selector);

  return analyzer;
}
