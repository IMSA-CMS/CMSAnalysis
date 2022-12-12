#include "CMSAnalysis/DataCollection/interface/InvariantMassPlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventModule.hh"
#include "CMSAnalysis/DataCollection/interface/PileupFilter.hh"

using std::make_shared;

void InvariantMassPlan::initialize() 
{
  Analyzer& analyzer = getAnalyzer();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  auto invMassHist = make_shared<InvariantMassHist>(InputModule::RecoLevel::Reco, "invariant_Mass", 300, 0, 1000);
  auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::Reco, "same_Sign_Invariant_Mass", 300, 0, 1000);

  // auto pileUpHist = make_shared<PileUpHist>("Pile Up Hist", 300, 0, 100);
  // Create necessary module(s) for the filter(s)
  // auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  // auto simTrigger = make_shared<SimTrigger>("filter_trigger", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);

  // Create necessary filter(s)
  // auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  // auto triggerFilter = make_shared<TriggerFilter>(simTrigger);

  // auto ssInvMassFilter = make_shared<SameSignInvariantMassFilter>(500);
  // auto pileUpFilter = make_shared<PileupFilter>(40,40);

  // Add the filter module(s) to the histogram(s) created above
  // invMassHist->addFilter(nLeptonsFilter);
  // sameSignInvMassHist->addFilter(nLeptonsFilter);
  // invMassHist->addFilter(triggerFilter);
  // sameSignInvMassHist->addFilter(triggerFilter);
  // invMassHist->addFilter(triggerFilter);

  // invMassHist->addFilter(ssInvMassFilter);
  // sameSignInvMassHist->addFilter(ssInvMassFilter);
  // sameSignInvMassHist->addFilter(pileUpFilter);
  // pileUpHist->addFilter(ssInvMassFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(invMassHist);
  histMod->addHistogram(sameSignInvMassHist);
  // histMod->addHistogram(pileUpHist);

  // Add production modules
  // analyzer.addProductionModule(trigSimMod);

  // analyzer.addFilterModule(triggerFilter);
  auto eventDump = make_shared<EventDumpModule>(true,true);
  // auto compMod = make_shared<RecoGenSimComparisonModule>();
  // Hopefully doesn't break // <- this is profound
  analyzer.addAnalysisModule(histMod);
  //analyzer.addAnalysisModule(recoDump);
  // analyzer.addAnalysisModule(genSimDump);
  // analyzer.addAnalysisModule(compMod);
}