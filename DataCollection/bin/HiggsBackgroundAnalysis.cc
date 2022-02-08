#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/InvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/LRWeightModule.hh"
#include "CMSAnalysis/DataCollection/interface/METHist.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/PtHist.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/DataCollection/interface/ThirdMuonPtHist.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/DataCollection/interface/WeightingModule.hh"

using std::make_shared;

Analyzer higgsBackgroundAnalysis()
{
  Analyzer analyzer;

  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();
  auto metMod = make_shared<METModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
 
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

  // auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(true, "GenSim Same Sign Invariant Mass", 100, 0, 1000);
  // Go up to 2000 - Andy, 09/02 - and make more bins. Modifications also made for picking files
  auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
  auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);

  auto recoPt = make_shared<PtHist>(false, "Leading lepton pT", 500, 0, 1000);
  auto recoInvMass = make_shared<InvariantMassHist>(false, "Opposite-sign dilepton mass", 1000, 0, 2000);
  auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(recoSameSignInvMassHist);
  histMod->addHistogram(positiveNegativeInvMassHist);
  histMod->addHistogram(recoPt);
  histMod->addHistogram(recoInvMass);
  histMod->addHistogram(metHist);

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

  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  analyzer.addProductionModule(metMod);

  analyzer.addFilterModule(nLeptonsFilter);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms

  auto leptonSelector = std::make_shared<SnowmassLeptonSelector>(50, .05);
  analyzer.getInputModule().setLeptonSelector(leptonSelector);

  return analyzer;
}
