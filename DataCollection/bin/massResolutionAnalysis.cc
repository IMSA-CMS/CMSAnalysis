#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"

using std::make_shared;

Analyzer massResolutionAnalysis()
{
  Analyzer analyzer;

  auto matchMod = make_shared<MatchingModule>();
  auto barrelStateFilter = make_shared<BarrelStateFilter>(matchMod);
  auto massBinFilterForMass = make_shared<MassBinFilter>(matchMod, 300, 3100, 28);
  auto massBinFilterForPt = make_shared<MassBinFilter>(matchMod, 50, 1900, 37);
  //auto massResMod = make_shared<MassResolutionModule>(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  auto histMod = make_shared<HistogramOutputModule>();
  auto massResHist = make_shared<MassResolutionHist>("Mass Resolution Pasted", 100, -1, 1);

  //std::cerr << "no suspicious activity so far" << std::endl;

  auto ptResHist = make_shared<PtResolutionHist>("Pt Resolution Pasted", 100, -1, 1);

  //std::cerr << "phase one complete, initiate drone strike" << std::endl;

  // Add the filter modules to the histogram(s) created above
  massResHist->addFilter(massBinFilterForMass);
  massResHist->addFilter(barrelStateFilter);
  ptResHist->addFilter(massBinFilterForPt);
  ptResHist->addFilter(barrelStateFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(massResHist);
  histMod->addHistogram(ptResHist);

  analyzer.addProductionModule(matchMod);
  
  //analyzer.addAnalysisModule(massResMod);
  //analyzer.addAnalysisModule(ptResMod);
  analyzer.addAnalysisModule(histMod);
  return analyzer;
}
