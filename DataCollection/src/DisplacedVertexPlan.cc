#include "CMSAnalysis/DataCollection/interface/DisplacedVertexPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"
#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/METTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleIsolatedMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DisplacedVertexModule.hh"
#include "CMSAnalysis/DataCollection/interface/PhotonsHist.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"

using std::make_shared;

void DisplacedVertexPlan::initialize()
{
  Analyzer& analyzer = getAnalyzer();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  auto PTHist = make_shared<PhotonsHist>(InputModule::RecoLevel::Reco, "Photon_PT", 100, 0, 1000);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(PTHist);

  // Hopefully doesn't break
  analyzer.addAnalysisModule(histMod);
}