#include "CMSAnalysis/Plans/interface/PDFWeightingPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/Modules/interface/PdfAnalysisModule.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Filters/interface/METTrigger.hh"
#include "CMSAnalysis/Filters/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/SingleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/DoubleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/Modules/interface/DisplacedVertexModule.hh"
#include "CMSAnalysis/Histograms/interface/PhotonsHist.hh"
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