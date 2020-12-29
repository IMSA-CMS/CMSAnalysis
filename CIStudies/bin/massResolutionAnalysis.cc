#include <iostream>
#include <memory>

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/BarrelStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/MassBinFilter.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"

using std::make_shared;

Analyzer massResolutionAnalysis()
{
  Analyzer analyzer;

  auto genSimMod = make_shared<GenSimIdentificationModule>();
  auto recoMod = make_shared<RecoIdentificationModule>(5);
  auto matchMod = make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>(); 
  auto barrelStateFilter = make_shared<BarrelStateFilter>(matchMod);
  auto massBinFilter = make_shared<MassBinFilter>(matchMod, 300, 3100, 28);
  auto massResMod = make_shared<MassResolutionModule>(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  auto histMod = make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);
  auto massResHist = make_shared<MassResolutionHist>(genSimMod, recoMod, "Mass Resolution Pasted", 100, -1, 1);

  // Add the filter modules to the histogram(s) created above
  massResHist->addFilter(massBinFilter);
  massResHist->addFilter(barrelStateFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(massResHist);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);
  
  analyzer.addAnalysisModule(massResMod);
  analyzer.addAnalysisModule(histMod);
  
  return analyzer;
}
