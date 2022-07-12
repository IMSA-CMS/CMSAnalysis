#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionHist.hh"
//#include "CMSAnalysis/DataCollection/interface/PtResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/TotalEventsHist.hh"
#include "CMSAnalysis/DataCollection/interface/CIAcceptedEventsHist.hh"

using std::make_shared;

Analyzer massAcceptanceAnalysis()
{
  Analyzer analyzer;

  //auto genSimMod = make_shared<GenSimIdentificationModule>();
  //auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>();
  auto signFlipMod = make_shared<SignFlipModule>(matchMod);

  auto histMod = make_shared<HistogramOutputModule>();
  auto totalEventsHist = make_shared<TotalEventsHist>("Mass vs Total Events", 200, 0, 3000);


  // Add the histogram(s) created above to histMod
  histMod->addHistogram(totalEventsHist);

  analyzer.addProductionModule(matchMod);
  analyzer.addAnalysisModule(signFlipMod);

  analyzer.addAnalysisModule(histMod);
  return analyzer;
}
