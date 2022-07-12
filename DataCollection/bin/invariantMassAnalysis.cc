#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/InvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"

//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/PtResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"

using std::make_shared;

Analyzer invariantMassAnalysis()
{
  Analyzer analyzer;

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  auto invMassHist = make_shared<InvariantMassHist>(false, "invariant_Mass", 100, 0, 100);
  auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "same_Sign_Invariant_Mass", 300, 0, 300);

  // Create necessary module(s) for the filter(s)
  auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  auto simTrigger = make_shared<SimTrigger>("filter_trigger", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);

  // Create necessary filter(s)
  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  auto triggerFilter = make_shared<TriggerFilter>(simTrigger);

  // Add the filter module(s) to the histogram(s) created above
  invMassHist->addFilter(nLeptonsFilter);
  sameSignInvMassHist->addFilter(nLeptonsFilter);
  invMassHist->addFilter(triggerFilter);
  sameSignInvMassHist->addFilter(triggerFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(invMassHist);
  histMod->addHistogram(sameSignInvMassHist);

  // Add production modules
  analyzer.addProductionModule(trigSimMod);

  analyzer.addFilterModule(triggerFilter);

  // Hopefully doesn't break
  analyzer.addAnalysisModule(histMod);
  return analyzer;
}
