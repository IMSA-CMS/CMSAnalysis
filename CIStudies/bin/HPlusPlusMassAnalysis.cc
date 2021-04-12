#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerEfficiencyModule.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"

Analyzer hPlusPlusMassAnalysis() {
  Analyzer analyzer;

  auto eventDump = std::make_shared<GenSimEventDumpModule>(3);

  auto genSimMod = std::make_shared<GenSimIdentificationModule>(9900041);
  auto recoMod = std::make_shared<RecoIdentificationModule>(50);
  auto matchMod = std::make_shared<MatchingModule>(genSimMod, recoMod);
  auto triggerMod = std::make_shared<TriggerModule>(recoMod);
  auto weightMod = std::make_shared<WeightingModule>();
  auto lrWeightMod = std::make_shared<LRWeightModule>();

  auto nLeptonsFilter = std::make_shared<NLeptonsFilter>(matchMod); //Needs to be updated with shared pointers
  
  auto histMod = std::make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);
  auto nLeptonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = std::make_shared<LeptonEfficiency>(matchMod, genSimMod);
  auto massRecoEfficiency200 = std::make_shared<MassRecoEfficiency>(recoMod, 200, 10);
  auto massRecoEfficiency500 = std::make_shared<MassRecoEfficiency>(recoMod, 500, 25);
  auto massRecoEfficiency800 = std::make_shared<MassRecoEfficiency>(recoMod, 800, 40);
  auto massRecoEfficiency1000 = std::make_shared<MassRecoEfficiency>(recoMod, 1000, 50);
  auto massRecoEfficiency1300 = std::make_shared<MassRecoEfficiency>(recoMod, 1300, 65);

  auto triggerEfficiencyMod = std::make_shared<TriggerEfficiencyModule>(matchMod, genSimMod, 200, 10);

  auto recoThirdMuonPtHist = std::make_shared<ThirdMuonPtHist>(genSimMod, recoMod, false, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);
  auto sameSignInvMassHist = std::make_shared<SameSignInvariantMassHist>(genSimMod, recoMod, true, "GenSim Same Sign Invariant Mass", 100, 0, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(recoThirdMuonPtHist);
  histMod->addHistogram(nLeptonsHist);
  histMod->addHistogram(nElectronsHist);
  histMod->addHistogram(nMuonsHist);

  // Initialize triggers
  auto singleMuonTrigger = std::make_shared<SingleMuonTrigger>(recoMod, 50);
  auto doubleMuonTrigger = std::make_shared<DoubleMuonTrigger>(recoMod, 37, 27);
  auto tripleMuonTrigger =
      std::make_shared<TripleMuonTrigger>(recoMod, 10, 5, 5);

  // Add triggers to the TriggerModule
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(triggerMod);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);

  // Don't remove unless you don't want histograms
  analyzer.addAnalysisModule(histMod);
  // analyzer.addAnalysisModule(triggerEfficiencyMod);
  // analyzer.addAnalysisModule(eventDump);
  // analyzer.addAnalysisModule(leptonEfficiency);
  // analyzer.addAnalysisModule(massRecoEfficiency200);
  // analyzer.addAnalysisModule(massRecoEfficiency500);
  // analyzer.addAnalysisModule(massRecoEfficiency800);
  // analyzer.addAnalysisModule(massRecoEfficiency1000);
  // analyzer.addAnalysisModule(massRecoEfficiency1300);

  return analyzer;
}
