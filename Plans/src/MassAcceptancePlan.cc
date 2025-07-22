#include "CMSAnalysis/Plans/interface/MassAcceptancePlan.hh"

#include <memory>


#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Histograms/interface/TotalEventsHist.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"

using std::make_shared;

void MassAcceptancePlan::initialize()
{
    auto& modules = getModules();

  //auto genSimMod = make_shared<GenSimIdentificationModule>();
  //auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>();
  auto signFlipMod = make_shared<SignFlipModule>(matchMod);

  auto histMod = make_shared<HistogramOutputModule>();
  auto totalEventsHist = make_shared<TotalEventsHist>("Mass vs Total Events", 200, 0, 3000);


  // Add the histogram(s) created above to histMod
  histMod->addHistogram(totalEventsHist);

  modules.addProductionModule(matchMod);
  modules.addAnalysisModule(signFlipMod);

  modules.addAnalysisModule(histMod);

}