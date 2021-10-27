#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

//#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
//#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/BarrelStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/MassBinFilter.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
//#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
//#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"
//#include "CIAnalysis/CIStudies/interface/PtResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/TotalEventsHist.hh"
#include "CIAnalysis/CIStudies/interface/CIAcceptedEventsHist.hh"
//#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"

using std::make_shared;

Analyzer massAcceptanceAnalysis()
{
  Analyzer analyzer;

  //auto genSimMod = make_shared<GenSimIdentificationModule>();
  //auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>(); 

  auto histMod = make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);
  auto totalEventsHist = make_shared<TotalEventsHist>(genSimMod, "Mass vs Total Events", 200, 0, 3000);


  auto acceptedEventsHist = make_shared<CIAcceptedEventsHist>(matchMod, genSimMod, "Mass vs Accepted Events", 200, 0, 3000);
  auto acceptanceHistTemplate = make_shared<CIAcceptedEventsHist>(matchMod, genSimMod, "Mass vs Acceptance", 200, 0, 3000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(totalEventsHist);
  histMod->addHistogram(acceptedEventsHist);
  histMod->addHistogram(acceptanceHistTemplate);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);

  analyzer.addAnalysisModule(histMod);
  return analyzer;
}
