#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionPlan.hh"

#include <iostream>
#include <memory>
#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Filters/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Modules/interface/GenSimParticleModule.hh"
#include "CMSAnalysis/Histograms/interface/GetNthHighestPtHist.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonEfficiency.hh"
#include "CMSAnalysis/Histograms/interface/LeptonJetDeltaRHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetEfficiency.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Histograms/interface/LeptonJetPtHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/MassRecoEfficiency.hh"
#include "CMSAnalysis/Histograms/interface/MatchingDeltaRHist.hh"
#include "CMSAnalysis/Histograms/interface/MatchingEtaHist.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Histograms/interface/MatchingPhiHist.hh"
#include "CMSAnalysis/Histograms/interface/MatchedLeptonJetHist.hh"
#include "CMSAnalysis/Histograms/interface/MatchingPtHist.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonJetHist.hh"
#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonsHist.hh"
#include "CMSAnalysis/Filters/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Filters/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/Histograms/interface/RelIsoHist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMLCalculator.hh"
#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"

#include "CMSAnalysis/Histograms/interface/LeptonJetRecoHist.hh"

using std::make_shared;

void LeptonJetReconstructionPlan::initialize()
{
  Analyzer &analyzer = getAnalyzer();

  auto eventMod = std::make_shared<EventModule>();
  auto eventHistMod = eventMod->getHistogramModule();
  eventMod->addSelector(std::make_shared<LeptonJetSelector>());

  auto matchMod = std::make_shared<MatchingModule>();
  auto lepRecoMod = std::make_shared<LeptonJetReconstructionModule>(.01);
  auto genPartMod = std::make_shared<GenSimParticleModule>(1000022);
  auto eventDumpMod = std::make_shared<EventDumpModule>(true, true);
  auto lepMatchMod = std::make_shared<LeptonJetMatchingModule>(lepRecoMod, 0.5);

  auto histOutputMod = std::make_shared<HistogramOutputModule>();
  auto mlMod = std::make_shared<LeptonJetMLCalculator>();

  // Histograms
  auto deltaRHist = std::make_shared<LeptonJetDeltaRHist>(lepRecoMod, "Delta R Values (Reconstructed Jets)", 100, 0, 0.1); // VIKRAM CHANGED
  auto pTHist = std::make_shared<LeptonJetPtHist>(lepRecoMod, "pT Values (Reconstructed Jets)", 100, 0, 200);

  auto matchedLeptonJetHist = std::make_shared<MatchedLeptonJetHist>("Matched Lepton Jet Hist HadET", 100, 0, 10, lepMatchMod, lepRecoMod, true);
  auto unmatchedLeptonJetHist = std::make_shared<MatchedLeptonJetHist>("Unmatched Lepton Jet Hist HadET", 100, 0, 10, lepMatchMod, lepRecoMod, false);

  auto lepJetRecoElec = std::make_shared<LeptonJetRecoHist>(lepMatchMod, true, lepRecoMod, "Number of Reconstructed Electrons",3,0,2);
  auto lepJetRecoMuon = std::make_shared<LeptonJetRecoHist>(lepMatchMod, false, lepRecoMod, "Number of Reconstructed Muons",3,0,2);

  // auto matchDeltaRHist = std::make_shared<MatchingDeltaRHist>(lepMatchMod, "Differences in Delta R for Matched Lepton Jets", 100, 0, 0.5);
  // auto matchPtHist = std::make_shared<MatchingPtHist>(lepMatchMod, "Differences in pT for Matched Lepton Jets", 100, -300, 300);
  // auto matchPhiHist = std::make_shared<MatchingPhiHist>(lepMatchMod, "Differences in Phi for Matched Lepton Jets", 100, 0, 3.15);
  // auto matchEtaHist = std::make_shared<MatchingEtaHist>(lepMatchMod, "Differences in Eta for Matched Lepton Jets", 100, -1, 1);

  //  auto relIsoHist = std::make_shared<IsolationHist>(EventInput::RecoLevel::Reco, "Jet pT Rel", 10000, 0, 100);
  auto leptonJetMLHist = std::make_shared<LeptonJetMLHist>(EventInput::RecoLevel::Reco, "NN Classifier Output Distribution", 100, 0, 1, mlMod, lepRecoMod);

  // auto deltaXYHist = std::make_shared<DxyHist>(EventInput::RecoLevel::Reco, "Vertex Delta XY from Primary Vertex", 50, 0, 5);
  // auto deltaZHist = std::make_shared<DzHist>(EventInput::RecoLevel::Reco, "Vertex Delta Z from Primary Vertex", 50, 0, 5);
  // uncomented

  eventHistMod->addHistogram(deltaRHist);
  histOutputMod->addHistogram(deltaRHist);
  histOutputMod->addHistogram(pTHist);
  histOutputMod->addHistogram(matchedLeptonJetHist);
  histOutputMod->addHistogram(unmatchedLeptonJetHist);
  // eventHistMod->addHistogram(deltaXYHist);
  // eventHistMod->addHistogram(deltaZHist);
  //histOutputMod->addHistogram(relIsoHist);
 // histOutputMod->addHistogram(leptonJetMLHist);

  //   histOutputMod->addHistogram(matchDeltaRHist);
  //   histOutputMod->addHistogram(matchPtHist);
  //   histOutputMod->addHistogram(matchPhiHist);
  //   histOutputMod->addHistogram(matchEtaHist);
  // auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>(3);
  // auto genSimEventDumpMod = std::make_shared<GenSimEventDumpModule>();
  // auto recoEventDumpMod = std::make_shared<RecoEventDumpModule>();
  // auto triggerMod = std::make_shared<TriggerModule>();

  auto nLeptonsFilter = std::make_shared<NLeptonsFilter>(); // Needs to be updated with shared pointers
 
  auto nLeptonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);
  auto recoThirdMuonPtHist = std::make_shared<GetNthHighestPtHist>(EventInput::RecoLevel::Reco, "Reconstructed Third Muon Transverse Momentum", 50, 0, 100, 3);
  auto recoSecondMuonPtHist = std::make_shared<GetNthHighestPtHist>(EventInput::RecoLevel::Reco, "Reconstructed Second Muon Transverse Momentum", 50, 0, 100, 2);
  auto recoFirstMuonPtHist = std::make_shared<GetNthHighestPtHist>(EventInput::RecoLevel::Reco, "Reconstructed First Muon Transverse Momentum", 50, 0, 100, 1);

  // auto nLeptonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nLeptonJetHist = std::make_shared<NLeptonJetHist>(lepRecoMod, "Number of Lepton Jets", 10, 0, 10);
  // auto nElectronsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  //  auto nMuonsHist = std::make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);
  //  auto recoThirdMuonPtHist = std::make_shared<GetNthHighestPtHist>(genSimMod, recoMod, false, "Reconstructed Third Muon Transverse Momentum", 50, 0, 100, 3);
  //  auto recoThirdMuonPtHist = std::make_shared<ThirdMuonPtHist>(genSimMod, recoMod, false, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);

  // Efficiency Modules
  auto leptonEfficiency = std::make_shared<LeptonEfficiency>(matchMod);
  auto leptonJetEfficiency = std::make_shared<LeptonJetEfficiency>(lepRecoMod, lepMatchMod);

  // Add the histogram(s) created above to histMod
  // eventHistMod->addHistogram(nLeptonsHist);
  //eventHistMod->addHistogram(nElectronsHist);
  // eventHistMod->addHistogram(nMuonsHist);
  // eventHistMod->addHistogram(recoThirdMuonPtHist);
  // eventHistMod->addHistogram(recoSecondMuonPtHist);
  // eventHistMod->addHistogram(recoFirstMuonPtHist);
  // eventHistMod->addHistogram(nLeptonJetHist);

  // Initialize triggers

  /*
  auto singleMuonTrigger = std::make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = std::make_shared<DoubleMuonTrigger>(37, 27);
  auto tripleMuonTrigger =
      std::make_shared<TripleMuonTrigger>(10, 5, 5);
      */

  // Add triggers to the TriggerModule -- put these back later

  /*
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);
  */

  // analyzer.addProductionModule(genSimEventDumpMod);
  // analyzer.addProductionModule(recoEventDumpMod);
  analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(lepRecoMod);
  // analyzer.addProductionModule(genPartMod);
  analyzer.addProductionModule(lepMatchMod);

  analyzer.addAnalysisModule(histOutputMod);
  analyzer.addAnalysisModule(eventHistMod);

  // analyzer.addProductionModule(triggerMod);
  //analyzer.addAnalysisModule(leptonEfficiency);
  analyzer.addAnalysisModule(leptonJetEfficiency);
  //analyzer.addAnalysisModule(massRecoEfficiency200);
  //analyzer.addAnalysisModule(massRecoEfficiency500);
  //analyzer.addAnalysisModule(massRecoEfficiency800);
  //analyzer.addAnalysisModule(massRecoEfficiency1000);
  //analyzer.addAnalysisModule(massRecoEfficiency1300);
  analyzer.addAnalysisModule(eventDumpMod);
  //analyzer.addAnalysisModule(recoEventDumpMod);
  /* auto selector = make_shared<SnowmassLeptonSelector>(5);

  analyzer.getEventInput()->setLeptonSelector(selector);
  */
}