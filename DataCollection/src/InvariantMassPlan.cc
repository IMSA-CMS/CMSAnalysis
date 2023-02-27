#include "CMSAnalysis/DataCollection/interface/InvariantMassPlan.hh"

#include <iostream>
#include <memory>

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/MassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MassBinFilter.hh"
#include "CMSAnalysis/DataCollection/interface/ResolutionModule.hh"
//#include "CMSAnalysis/DataCollection/interface/MassResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/InvariantMassFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SimTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/RecoGenSimComparisonModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventModule.hh"
#include "CMSAnalysis/DataCollection/interface/PileupFilter.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonFilter.hh"
#include "CMSAnalysis/DataCollection/interface/FakePhotonFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MismeasuredChargeFilter.hh"

using std::make_shared;

void InvariantMassPlan::initialize() 
{
  Analyzer& analyzer = getAnalyzer();

  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  
  auto elecInvMassHist = make_shared<InvariantMassHist>(InputModule::RecoLevel::Reco, "Electron Invariant Mass", 300, 1, 1000);
  auto photonElectronMassHist = make_shared<PhotonInvariantMassHist>(InputModule::RecoLevel::Reco, "Photon-electron invariant mass", 300, 1, 1000);

  // Create necessary module(s) for the filter(s)

  // Create necessary filter(s)

  auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  auto lepFilter = make_shared<LeptonFilter>(ParticleType::electron(), 2, "Elec2");

  // Add the filter module(s) to the histogram(s) created above

  elecInvMassHist->addFilter(lepFilter);
  elecInvMassHist->addFilter(nLeptonsFilter);
  photonElectronMassHist->addFilter(lepFilter);
  photonElectronMassHist->addFilter(nLeptonsFilter);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(elecInvMassHist);
  histMod->addHistogram(photonElectronMassHist);

  // Add production modules
  auto compMod = make_shared<RecoGenSimComparisonModule>("mother");
  // Hopefully doesn't break // <- this is profound
  analyzer.addAnalysisModule(histMod);
  analyzer.addAnalysisModule(compMod);
}