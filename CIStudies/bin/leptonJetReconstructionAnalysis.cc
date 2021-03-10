#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/DeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimParticleModule.hh"
#include "CIAnalysis/CIStudies/interface/GetNthHighestPtHist.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MatchingDeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"

Analyzer leptonJetReconstructionAnalysis() {
  Analyzer analyzer;

  auto genSimMod = std::make_shared<GenSimIdentificationModule>(4900022);
  auto recoMod = std::make_shared<RecoIdentificationModule>(5);
  auto matchMod = std::make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = std::make_shared<WeightingModule>();
  auto lrWeightMod = std::make_shared<LRWeightModule>();
  auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>(3);
  auto lepRecoMod = std::make_shared<LeptonJetReconstructionModule>(recoMod);
  auto genPartMod = std::make_shared<GenSimParticleModule>(1000022);
  auto lepMatchMod =
      std::make_shared<LeptonJetMatchingModule>(genPartMod, lepRecoMod);
  auto histOutputMod = std::make_shared<HistogramOutputModule>(
      genSimMod, recoMod, weightMod, lrWeightMod);

  // Histograms
  auto matchDeltaRHist = std::make_shared<MatchingDeltaRHist>(
      lepMatchMod, "Differences in Delta R for Matched Lepton Jets", 100, 0,
      0.5);

  histOutputMod->addHistogram(matchDeltaRHist);
  // auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>(3);
  auto triggerMod = std::make_shared<TriggerModule>(recoMod);

  auto nLeptonsFilter = std::make_shared<NLeptonsFilter>(
      matchMod); // Needs to be updated with shared pointers

  auto nLeptonsHist =
      std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = std::make_shared<NLeptonsHist>(
      matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist =
      std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);
  auto recoThirdMuonPtHist = std::make_shared<GetNthHighestPtHist>(
      genSimMod, recoMod, false, "Reconstructed Third Muon Transverse Momentum",
      50, 0, 100, 3);
  // auto recoThirdMuonPtHist = std::make_shared<ThirdMuonPtHist>(genSimMod,
  // recoMod, false, std::string("Reconstructed Third Muon Transverse Momentum"),
  // 50, 0, 3000);

  auto leptonEfficiency =
      std::make_shared<LeptonEfficiency>(matchMod, genSimMod);

  // Add the histogram(s) created above to histMod
  histOutputMod->addHistogram(nLeptonsHist);
  histOutputMod->addHistogram(nElectronsHist);
  histOutputMod->addHistogram(nMuonsHist);
  histOutputMod->addHistogram(recoThirdMuonPtHist);

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
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(lepRecoMod);
  analyzer.addProductionModule(genPartMod);
  analyzer.addProductionModule(lepMatchMod);

  analyzer.addAnalysisModule(histOutputMod);
  // analyzer.addProductionModule(triggerMod);

  // analyzer.addAnalysisModule(leptonEfficiency);
  // analyzer.addAnalysisModule(massRecoEfficiency200);
  // analyzer.addAnalysisModule(massRecoEfficiency500);
  // analyzer.addAnalysisModule(massRecoEfficiency800);
  // analyzer.addAnalysisModule(massRecoEfficiency1000);
  // analyzer.addAnalysisModule(massRecoEfficiency1300);
  // analyzer.addAnalysisModule(genSimEventDumpMod);

  return analyzer;
}
