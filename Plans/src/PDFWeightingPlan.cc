#include "CMSAnalysis/Plans/interface/PDFWeightingPlan.hh"

#include <memory>



#include "CMSAnalysis/Modules/interface/PdfAnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"

using std::make_shared;

void PDFWeightingPlan::initialize()
{
  auto& modules = getModules();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  //auto PTHist = make_shared<PhotonsHist>(EventInput::RecoLevel::Reco, "Photon_PT", 100, 0, 1000);

  // Add the histogram(s) created above to histMod
  //histMod->addHistogram(PTHist);

  // Hopefully doesn't break
  modules.addAnalysisModule(histMod);
}