#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/WeightingModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionHist.hh"
//#include "CMSAnalysis/DataCollection/interface/PtResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/MassAcceptanceFirstHist.hh"
#include "CMSAnalysis/DataCollection/interface/MassAcceptanceSecondHist.hh"
#include "CMSAnalysis/DataCollection/interface/LRWeightModule.hh"

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