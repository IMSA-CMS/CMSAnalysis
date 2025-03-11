#include "CMSAnalysis/Plans/interface/DisplacedVertexPlan.hh"

#include <iostream>
#include <memory>


#include "CMSAnalysis/Histograms/interface/PhotonsHist.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"

using std::make_shared;

void DisplacedVertexPlan::initialize()
{
  auto& modules = getModules();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  auto PTHist = make_shared<PhotonsHist>(EventInput::RecoLevel::Reco, "Photon_PT", 100, 0, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(PTHist);

  // Hopefully doesn't break
  modules.addAnalysisModule(histMod);
}