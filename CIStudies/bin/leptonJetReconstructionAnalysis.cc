#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/DeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimParticleModule.hh"
#include "CIAnalysis/CIStudies/interface/GetNthHighestPtHist.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetPtHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MatchingDeltaRHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchingEtaHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPhiHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchedLeptonJetHist.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPtHist.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonJetHist.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/METTrigger.hh"
#include "CIAnalysis/CIStudies/interface/METModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/SnowmassLeptonSelector.hh"
using std::make_shared;

Analyzer leptonJetReconstructionAnalysis() {
  Analyzer analyzer;
  // auto genSimMod = std::make_shared<GenSimIdentificationModule>(4900022);
  // auto recoMod = std::make_shared<RecoIdentificationModule>(5);
  // auto matchMod = std::make_shared<MatchingModule>(genSimMod, recoMod);
  // auto weightMod = std::make_shared<WeightingModule>();
  // auto lrWeightMod = std::make_shared<LRWeightModule>();

  // auto genSimMod = std::make_shared<GenSimIdentificationModule>(4900022);
  // auto recoMod = std::make_shared<RecoIdentificationModule>(5);
  auto matchMod = std::make_shared<MatchingModule>();
  auto weightMod = std::make_shared<WeightingModule>();
  auto lrWeightMod = std::make_shared<LRWeightModule>();
  auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>();
  auto lepRecoMod = std::make_shared<LeptonJetReconstructionModule>(.05);
  auto genPartMod = std::make_shared<GenSimParticleModule>(1000022);
  auto lepMatchMod =
      std::make_shared<LeptonJetMatchingModule>(lepRecoMod, 0.5);
  auto histOutputMod = std::make_shared<HistogramOutputModule>(
      weightMod, lrWeightMod);
    auto metMod = std::make_shared<METModule>();

  // Histograms
  auto deltaRHist = std::make_shared<DeltaRHist>(lepRecoMod, "Delta R Values (Reconstructed Jets)", 100, 0, 0.1);
  auto pTHist = std::make_shared<LeptonJetPtHist>(lepRecoMod, "pT Values (Reconstructed Jets)", 100, 0, 200);
  auto matchedLeptonJetHist = std::make_shared<MatchedLeptonJetHist>("Matched Lepton Jet Hist HadET", 100, 0, 10, lepMatchMod, lepRecoMod, true);
  auto unmatchedLeptonJetHist = std::make_shared<MatchedLeptonJetHist>("Unmatched Lepton Jet Hist HadET", 100, 0, 10, lepMatchMod, lepRecoMod, false);
  // auto matchDeltaRHist = std::make_shared<MatchingDeltaRHist>(lepMatchMod, "Differences in Delta R for Matched Lepton Jets", 100, 0, 0.5);
  // auto matchPtHist = std::make_shared<MatchingPtHist>(lepMatchMod, "Differences in pT for Matched Lepton Jets", 100, -300, 300);
  // auto matchPhiHist = std::make_shared<MatchingPhiHist>(lepMatchMod, "Differences in Phi for Matched Lepton Jets", 100, 0, 3.15);
  // auto matchEtaHist = std::make_shared<MatchingEtaHist>(lepMatchMod, "Differences in Eta for Matched Lepton Jets", 100, -1, 1);

  histOutputMod->addHistogram(deltaRHist);
  histOutputMod->addHistogram(pTHist);
  histOutputMod->addHistogram(matchedLeptonJetHist);
  histOutputMod->addHistogram(unmatchedLeptonJetHist);
  // histOutputMod->addHistogram(matchDeltaRHist);
  // histOutputMod->addHistogram(matchPtHist);
  // histOutputMod->addHistogram(matchPhiHist);
  // histOutputMod->addHistogram(matchEtaHist);
  // auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>(3);

  auto triggerMod = std::make_shared<TriggerModule>();

  auto nLeptonsFilter = std::make_shared<NLeptonsFilter>(); // Needs to be updated with shared pointers

  auto nLeptonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);
  auto recoThirdMuonPtHist = std::make_shared<GetNthHighestPtHist>(false, "Reconstructed Third Muon Transverse Momentum", 50, 0, 100, 3);
  auto recoSecondMuonPtHist = std::make_shared<GetNthHighestPtHist>(false, "Reconstructed Second Muon Transverse Momentum", 50, 0, 100, 2);
  auto recoFirstMuonPtHist = std::make_shared<GetNthHighestPtHist>(false, "Reconstructed First Muon Transverse Momentum", 50, 0, 100, 1);

  // auto nLeptonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nLeptonJetHist = std::make_shared<NLeptonJetHist>(lepRecoMod, "Number of Lepton Jets", 10, 0, 10);
  // auto nElectronsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  // auto nMuonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);
  // auto recoThirdMuonPtHist = std::make_shared<GetNthHighestPtHist>(genSimMod, recoMod, false, "Reconstructed Third Muon Transverse Momentum", 50, 0, 100, 3);
  // auto recoThirdMuonPtHist = std::make_shared<ThirdMuonPtHist>(genSimMod, recoMod, false, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);

  // Efficiency Modules
  auto leptonEfficiency = std::make_shared<LeptonEfficiency>(weightMod, matchMod);
  auto leptonJetEfficiency = std::make_shared<LeptonJetEfficiency>(weightMod, lepRecoMod, lepMatchMod);

  // Add the histogram(s) created above to histMod
  histOutputMod->addHistogram(nLeptonsHist);
  histOutputMod->addHistogram(nElectronsHist);
  histOutputMod->addHistogram(nMuonsHist);
  histOutputMod->addHistogram(recoThirdMuonPtHist);
  histOutputMod->addHistogram(recoSecondMuonPtHist);
  histOutputMod->addHistogram(recoFirstMuonPtHist);
  histOutputMod->addHistogram(nLeptonJetHist);

  // Initialize triggers
  /*
  auto singleMuonTrigger = std::make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = std::make_shared<DoubleMuonTrigger>(37, 27);
  auto tripleMuonTrigger =
      std::make_shared<TripleMuonTrigger>(10, 5, 5);
      */

  // Add triggers to the TriggerModule -- put these back later
  auto metTrigger = std::make_shared<METTrigger>(metMod, 50);
  triggerMod->addTrigger(metTrigger);
  /*
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);
  */

  // analyzer.addProductionModule(genSimMod);
  // analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  // analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(lepRecoMod);
  // analyzer.addProductionModule(genPartMod);
  // analyzer.addProductionModule(lepMatchMod);

  // analyzer.addAnalysisModule(histOutputMod);
  analyzer.addProductionModule(metMod);
  analyzer.addProductionModule(triggerMod);

  // analyzer.addAnalysisModule(leptonEfficiency);
  // analyzer.addAnalysisModule(leptonJetEfficiency);
  // analyzer.addAnalysisModule(massRecoEfficiency200);
  // analyzer.addAnalysisModule(massRecoEfficiency500);
  // analyzer.addAnalysisModule(massRecoEfficiency800);
  // analyzer.addAnalysisModule(massRecoEfficiency1000);
  // analyzer.addAnalysisModule(massRecoEfficiency1300);
  // analyzer.addAnalysisModule(genSimEventDumpMod);

  analyzer.addAnalysisModule(histOutputMod);
  //analyzer.addProductionModule(triggerMod);

  auto selector = make_shared<SnowmassLeptonSelector>(5);
  analyzer.getInputModule().setLeptonSelector(selector);
  return analyzer;
}
