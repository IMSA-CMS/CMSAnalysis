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
//#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
//#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"
//#include "CIAnalysis/CIStudies/interface/PtResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/MassAcceptanceFirstHist.hh"
#include "CIAnalysis/CIStudies/interface/MassAcceptanceSecondHist.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"

using std::make_shared;

Analyzer mvaAnalysis()
{
  Analyzer analyzer;

  auto genSimMod = make_shared<GenSimIdentificationModule>();
  auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>(); 
  //auto barrelStateFilter = make_shared<BarrelStateFilter>(matchMod);
  //auto massBinFilterForMass = make_shared<MassBinFilter>(matchMod, 300, 3100, 28);
  //auto massBinFilterForPt = make_shared<MassBinFilter>(matchMod, 50, 1900, 37);
  //auto massResMod = make_shared<MassResolutionModule>(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  auto histMod = make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);
  auto mvaHist = make_shared<MassAcceptanceFirstHist>(genSimMod, "MVA Hist 1", 100, 0, 1000);

  std::cerr << "no suspicious activity so far" << std::endl;

  auto mvaHist2 = make_shared<MassAcceptanceSecondHist>(matchMod, "MVA Hist 2", 100, 0, 1000);

  std::cerr << "phase one complete, initiate drone strike" << std::endl;

  // Add the filter modules to the histogram(s) created above
  //MVAFirstHist->addFilter(massBinFilterForMass);
  //MVAFirstHist->addFilter(barrelStateFilter);
  //MVASecondHist->addFilter(massBinFilterForPt);
  //MVASecondHist->addFilter(barrelStateFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(mvaHist);
  histMod->addHistogram(mvaHist2);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);
  
  //analyzer.addAnalysisModule(massResMod);
  //analyzer.addAnalysisModule(ptResMod);
  analyzer.addAnalysisModule(histMod);
  return analyzer;
}