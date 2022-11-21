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
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"
#include "CMSAnalysis/DataCollection/interface/PhotonsHist.hh"


using std::make_shared;

void InvariantMassPlan::setupAnalyzer()
{
  Analyzer& analyzer = getAnalyzer();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  auto PTHist = make_shared<PhotonsHist>(InputModule::RecoLevel::Reco, "Photon_PT", 100, 0, 100);

  // Create necessary module(s) for the filter(s)
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  auto simTrigger = make_shared<SimTrigger>("filter_trigger", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);

  // Create necessary filter(s)
  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  auto triggerFilter = make_shared<TriggerFilter>(simTrigger);

  // Add the filter module(s) to the histogram(s) created above
  // invMassHist->addFilter(nLeptonsFilter);
  // sameSignInvMassHist->addFilter(nLeptonsFilter);
  // invMassHist->addFilter(triggerFilter);
  // sameSignInvMassHist->addFilter(triggerFilter);

  // Add the histogram(s) created above to histMod
  // histMod->addHistogram(PTHist);

  // Add production modules
  // analyzer.addProductionModule(trigSimMod);

  // analyzer.addFilterModule(triggerFilter);

  // Hopefully doesn't break
  analyzer.addAnalysisModule(histMod);
}
