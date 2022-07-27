#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/METHist.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SnowmassCutFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/DataCollection/interface/BJetFilter.hh"

using std::make_shared;

Analyzer higgsBackgroundAnalysis()
{
  Analyzer analyzer;

  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto metMod = make_shared<METModule>();
  auto bJetFilter = make_shared<BJetFilter>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
 
  auto histMod = make_shared<HistogramOutputModule>();
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

  // Go up to 2000 - Andy, 09/02 - and make more bins. Modifications also made for picking files

  auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::GenSim, "GenSim Same Sign Invariant Mass", 100, 0, 1000, false, false);
  auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::Reco, "Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
  auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);

  auto recoPt = make_shared<PtHist>(InputModule::RecoLevel::Reco, "Leading lepton pT", 500, 0, 1000);
  auto recoInvMass = make_shared<InvariantMassHist>(InputModule::RecoLevel::Reco, "Opposite-sign dilepton mass", 1000, 0, 2000);
  auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(genSimSameSignInvMassHist);
  histMod->addHistogram(positiveNegativeInvMassHist);
  histMod->addHistogram(recoPt);
  histMod->addHistogram(recoInvMass);
  histMod->addHistogram(metHist);

  auto elecRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::Reco, "Electron Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
  auto elecPositiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Electron Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
  auto muonRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::Reco, "Muon Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
  auto muonPositiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Muon Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);

  auto elecFilter = make_shared<LeptonFilter>(Particle::Type::Electron, 4, "Electron");
  auto muonFilter = make_shared<LeptonFilter>(Particle::Type::Muon, 4, "Muon");
  auto snowmassCut = make_shared<SnowmassCutFilter>();

  elecRecoSameSignInvMassHist->addFilter(elecFilter);
  elecPositiveNegativeInvMassHist->addFilter(elecFilter);
  muonRecoSameSignInvMassHist->addFilter(muonFilter);
  muonPositiveNegativeInvMassHist->addFilter(muonFilter);

  histMod->addHistogram(elecRecoSameSignInvMassHist);
  histMod->addHistogram(elecPositiveNegativeInvMassHist);
  histMod->addHistogram(muonRecoSameSignInvMassHist);
  histMod->addHistogram(muonPositiveNegativeInvMassHist);

  analyzer.addProductionModule(metMod);
  analyzer.addFilterModule(bJetFilter);

  analyzer.addFilterModule(snowmassCut);
  analyzer.addFilterModule(nLeptonsFilter);
  
  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms

  auto leptonSelector = std::make_shared<SnowmassLeptonSelector>(10);
  analyzer.getInputModule()->setLeptonSelector(leptonSelector);

  return analyzer;
}
