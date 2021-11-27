#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
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
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerEfficiencyModule.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TwoInvariantMassesHist.hh"
#include "CIAnalysis/CIStudies/interface/UnusualFinalStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"


using std::make_shared;

Analyzer backgroundEstimateAnalysis() {
  Analyzer analyzer;

  auto eventDump = make_shared<GenSimEventDumpModule>(7);

  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>(50);

  auto unusualFinalStateFilter = make_shared<UnusualFinalStateFilter>();
  
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = make_shared<LeptonEfficiency>(weightMod, matchMod);

  // auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(true, "GenSim Same Sign Invariant Mass", 100, 0, 1000);
  // Go up to 2000 - Andy, 09/02 - and make more bins. Modifications also made for picking files
  auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "Reco Same Sign Invariant Mass", 1000, 0, 2000, 50);

  auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 1000, 1000, 50);

  // Add the histogram(s) created above to histMod
  //histMod->addHistogram(recoThirdMuonPtHist);
  // histMod->addHistogram(genSimSameSignInvMassHist);
  histMod->addHistogram(recoSameSignInvMassHist);
  //histMod->addHistogram(genSimHPlusPlusRecoveredInvMassHist);
  //histMod->addHistogram(recoHPlusPlusRecoveredInvMassHist);
  //histMod->addHistogram(genSimHMinusMinusRecoveredInvMassHist);
  //histMod->addHistogram(recoHMinusMinusRecoveredInvMassHist);
  //histMod->addHistogram(photonHist);
  //histMod->addHistogram(nLeptonsHist);
  //histMod->addHistogram(nElectronsHist);
  //histMod->addHistogram(nMuonsHist);
  //histMod->addHistogram(recoPhiSameSignInvMassHist);
  //histMod->addHistogram(recoMultSameSignInvMassHist);
  //histMod->addHistogram(recoPhiMultSameSignInvMassHist);
  histMod->addHistogram(positiveNegativeInvMassHist);

  // Initialize triggers
  auto singleMuonTrigger = make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = make_shared<DoubleMuonTrigger>(37, 27);
  auto tripleMuonTrigger = make_shared<TripleMuonTrigger>(10, 5, 5);

  // Add triggers to the TriggerModule
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);

  // analyzer.addProductionModule(matchMod);
  // analyzer.addProductionModule(triggerMod);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);

  // Filters
  // 09/12: Add nLeptons filer 
  analyzer.addFilterModule(nLeptonsFilter);
  //analyzer.addFilterModule(unusualFinalStateFilter);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
  //analyzer.addAnalysisModule(eventDump);
  //analyzer.addAnalysisModule(leptonEfficiency);

  //analyzer.addAnalysisModule(triggerEfficiencyMod4010);
  //analyzer.addAnalysisModule(triggerEfficiencyMod4040);
  //analyzer.addAnalysisModule(triggerEfficiencyMod8040);
  //analyzer.addAnalysisModule(triggerEfficiencyMod20080);

  //analyzer.addAnalysisModule(massRecoEfficiency200);
  //analyzer.addAnalysisModule(massRecoEfficiency500);
  //analyzer.addAnalysisModule(massRecoEfficiency800);
  //analyzer.addAnalysisModule(massRecoEfficiency1000);
  //analyzer.addAnalysisModule(massRecoEfficiency1300);
  
  //analyzer.addAnalysisModule(massRecoEfficiency55);
  //analyzer.addAnalysisModule(massRecoEfficiency1010);
  //analyzer.addAnalysisModule(massRecoEfficiency4010);
  //analyzer.addAnalysisModule(massRecoEfficiency4040);
  //analyzer.addAnalysisModule(massRecoEfficiency8040);
  //analyzer.addAnalysisModule(massRecoEfficiency20080);

  return analyzer;
}
