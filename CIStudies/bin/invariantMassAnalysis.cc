#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/InvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"

//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/BarrelStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/MassBinFilter.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
//#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/TriggerFilter.hh"
#include "CIAnalysis/CIStudies/interface/SimTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TriggerSimModule.hh"



using std::make_shared;

Analyzer invariantMassAnalysis()
{
  Analyzer analyzer;

  // Create base module(s)
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);
  auto invMassHist = make_shared<InvariantMassHist>(false, "invariant_Mass", 100, 0, 1500);
  auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "same_Sign_Invariant_Mass", 300, 0, 1500);


  // Create necessary module(s) for the filter(s)
  //auto trigSimMod = make_shared<TriggerSimModule>("HLT");
  //auto simTrigger = make_shared<SimTrigger>("filter_trigger", TriggerSimModule::EnumTriggers::doubleElectronTriggers, trigSimMod);

  // Create necessary filter(s)
  
  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  //auto triggerFilter = make_shared<TriggerFilter>(simTrigger);

  // Add the filter module(s) to the histogram(s) created above
  invMassHist->addFilter(nLeptonsFilter);
  sameSignInvMassHist->addFilter(nLeptonsFilter);
  //invMassHist->addFilter(triggerFilter);
  //sameSignInvMassHist->addFilter(triggerFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(invMassHist);
  histMod->addHistogram(sameSignInvMassHist);
 

  // Add production modules
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  //analyzer.addProductionModule(trigSimMod);

  //analyzer.addFilterModule(triggerFilter);

  // Hopefully doesn't break
  analyzer.addAnalysisModule(histMod);
  return analyzer;
}
