#include "CMSAnalysis/DataCollection/interface/HPlusPlusMassPlan.hh"

#include <iostream>
#include <memory>
#include <functional>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonEfficiency.hh"
#include "CMSAnalysis/DataCollection/interface/MassRecoEfficiency.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsHist.hh"
#include "CMSAnalysis/DataCollection/interface/PhotonsHist.hh"
#include "CMSAnalysis/DataCollection/interface/RecoveredInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerEfficiencyModule.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/DataCollection/interface/UnusualFinalStateFilter.hh"
#include "CMSAnalysis/DataCollection/interface/PhotonsHist.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/METHist.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonEfficiency.hh"
#include "CMSAnalysis/DataCollection/interface/METTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SignFlipModule.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype1DGeneral.hh"


using std::make_shared;

HPlusPlusMassPlan::HPlusPlusMassPlan() {
  Analyzer& analyzer = getAnalyzer();

  auto eventDump = make_shared<GenSimEventDumpModule>(7);

  //auto genSimMod = make_shared<GenSimIdentificationModule>(9900041, true);
  //auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto mETMod = make_shared<METModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>(); //Needs to be updated with shared pointers
  
  auto histMod = make_shared<HistogramOutputModule>();
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod);
  auto signFlip = make_shared<SignFlipModule>(matchMod);

  auto recoThirdMuonPtHist = make_shared<ThirdMuonPtHist>(InputModule::RecoLevel::Reco, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);
  //auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(true, "GenSim Same Sign Invariant Mass", 100, 0, 1000);
  // Go up to 2000 - Andy, 09/02 - and make more bins. Modifications also made for picking files
  //auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(false, "Reco Same Sign Invariant Mass", 1000, 0, 2000);

  // auto recoThirdMuonPtHist = make_shared<ThirdMuonPtHist>(genSimMod, recoMod, false, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);
  // 
  auto genSimSameSignInvMassHist = make_shared<HistogramPrototype1DGeneral>("GenSim Same Sign Invariant Mass", 100, 0, 1000,
[](const InputModule* input){return std::vector<double>{input -> getParticles(InputModule::RecoLevel::GenSim).calculateSameSignInvariantMass()};});

  auto recoSameSignInvMassHist = make_shared<HistogramPrototype1DGeneral>("Reco Same Sign Invariant Mass", 100, 0, 1000,
  [](const InputModule* input){return std::vector<double>{input -> getParticles(InputModule::RecoLevel::Reco).calculateSameSignInvariantMass()};});
  // auto genSimHPlusPlusRecoveredInvMassHist = make_shared<RecoveredInvariantMassHist>(genSimMod, recoMod, true, "GenSim H++ Recovered Invariant Mass with 3 Leptons", 100, 0, 1000, 3, 9900041);
  // auto recoHPlusPlusRecoveredInvMassHist = make_shared<RecoveredInvariantMassHist>(genSimMod, recoMod, false, "Reco H++ Recovered Invariant Mass with 3 Leptons", 100, 0, 1000, 3, 9900041);
  // auto genSimHMinusMinusRecoveredInvMassHist = make_shared<RecoveredInvariantMassHist>(genSimMod, recoMod, true, "GenSim H-- Recovered Invariant Mass with 3 Leptons", 100, 0, 1000, 3, -9900041);
  // auto recoHMinusMinusRecoveredInvMassHist = make_shared<RecoveredInvariantMassHist>(genSimMod, recoMod, false, "Reco H-- Recovered Invariant Mass with 3 Leptons", 100, 0, 1000, 3, 9900041);



  // Add the histogram(s) created above to histMod
  //histMod->addHistogram(recoThirdMuonPtHist);
  // histMod->addHistogram(sameSignInvMassHistGen);
  //histMod->addHistogram(sameSignInvMassHistReco);
  // histMod->addHistogram(genSimHPlusPlusRecoveredInvMassHist);
  // histMod->addHistogram(recoHPlusPlusRecoveredInvMassHist);
  // histMod->addHistogram(genSimHMinusMinusRecoveredInvMassHist);
  // histMod->addHistogram(recoHMinusMinusRecoveredInvMassHist);
  
  //histMod->addHistogram(recoPhotonHist);
  // histMod->addHistogram(nLeptonsHist);
  //histMod->addHistogram(nElectronsHist);
  //histMod->addHistogram(nMuonsHist);
  //histMod->addHistogram(recoPhiSameSignInvMassHist);
  //histMod->addHistogram(recoMultSameSignInvMassHist);
  //histMod->addHistogram(recoPhiMultSameSignInvMassHist);
  //histMod->addHistogram(positiveNegativeInvMassHist);
  histMod->addHistogram(genSimSameSignInvMassHist);
  histMod->addHistogram(recoSameSignInvMassHist);

  // Initialize triggers
  auto singleMuonTrigger = make_shared<SingleMuonTrigger>(50);
  auto doubleMuonTrigger = make_shared<DoubleMuonTrigger>(37, 27);
  auto tripleMuonTrigger = make_shared<TripleMuonTrigger>(10, 5, 5);
  auto mETTrigger = make_shared<METTrigger>(mETMod, 120);

  // Add triggers to the TriggerModule
  triggerMod->addTrigger(singleMuonTrigger);
  triggerMod->addTrigger(doubleMuonTrigger);
  triggerMod->addTrigger(tripleMuonTrigger);
  triggerMod->addTrigger(mETTrigger);

  //analyzer.addProductionModule(genSimMod);
  //analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);
  // analyzer.addProductionModule(triggerMod); 
  analyzer.addProductionModule(mETMod);

  // Filters
  // 09/12: Add nLeptons filer 
  analyzer.addFilterModule(nLeptonsFilter);
  //analyzer.addFilterModule(unusualFinalStateFilter);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
  analyzer.addAnalysisModule(signFlip);
  //analyzer.addAnalysisModule(eventDump);
  analyzer.addAnalysisModule(leptonEfficiency);
}
