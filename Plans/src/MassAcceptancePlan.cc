#include "CMSAnalysis/Plans/interface/MassAcceptancePlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Filters/interface/MassFilter.hh"
#include "CMSAnalysis/Filters/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Filters/interface/MassBinFilter.hh"
#include "CMSAnalysis/Modules/interface/ResolutionModule.hh"
#include "CMSAnalysis/Histograms/interface/TotalEventsHist.hh"
#include "CMSAnalysis/Histograms/interface/CIAcceptedEventsHist.hh"

using std::make_shared;

void MassAcceptancePlan::initialize()
{
    auto& analyzer = getAnalyzer();

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

}