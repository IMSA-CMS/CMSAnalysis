#include "CMSAnalysis/Plans/interface/InvariantMassPlan.hh"

#include <memory>

#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/Modules/interface/RecoGenSimComparisonModule.hh"
#include "CMSAnalysis/Histograms/interface/PhotonElectronInvariantMassHist.hh"

using std::make_shared;

void InvariantMassPlan::initialize() 
{
  
auto& modules = getModules();
  // Create necessary histogram(s), as well as histMod
  auto histMod = make_shared<HistogramOutputModule>();
  
  // auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "same_Sign_Invariant_Mass", 300, 0, 1000);
  // auto invMassHist = make_shared<InvariantMassHist>(EventInput::RecoLevel::Reco, "invariant_Mass", 300, 0, 1000);
  auto photonElectronHist = make_shared<PhotonElectronInvariantMassHist>(EventInput::RecoLevel::Reco, "photon_Electron_Invariant_Mass", 300, 0, 1000);



  auto ssInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Drell-Yan Same Sign Invariant Mass", 300, 1, 1000);
  auto highSsInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Drell-Yan Same Sign Invariant Mass > 500 GeV", 200, 500, 1000);
  // Create necessary module(s) for the filter(s)

  // Create necessary filter(s)

  // auto nLeptonsFilter = make_shared<NLeptonsFilter>();
  // auto lepFilter = make_shared<LeptonFilter>(ParticleType::electron(), 2, "Elec2");

  // Add the filter module(s) to the histogram(s) created above

  // elecInvMassHist->addFilter(lepFilter);
  // elecInvMassHist->addFilter(nLeptonsFilter);
  // photonElectronMassHist->addFilter(lepFilter);
  // photonElectronMassHist->addFilter(nLeptonsFilter);

  // Add the histogram(s) created above to histMod
  // histMod->addHistogram(elecInvMassHist);
  // histMod->addHistogram(photonElectronMassHist);
  // histMod->addHistogram(ssInvMassHist);
  // histMod->addHistogram(highSsInvMassHist);

  // Add production modules
  //auto eventDump = make_shared<EventDumpModule>(1, 1);
  auto compMod = make_shared<RecoGenSimComparisonModule>("mother", true);
  // Hopefully doesn't break // <- this is profound
  modules.addAnalysisModule(histMod);
  //modules.addAnalysisModule(eventDump);
  modules.addAnalysisModule(compMod);
}