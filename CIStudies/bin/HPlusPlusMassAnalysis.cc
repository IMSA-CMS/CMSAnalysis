#include <iostream>
#include <memory>
#include <functional>
#include "TROOT.h"
#include "TSystem.h"


#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/DoubleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/PhotonsHist.hh"
#include "CIAnalysis/CIStudies/interface/RecoveredInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/SingleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"
#include "CIAnalysis/CIStudies/interface/TriggerEfficiencyModule.hh"
#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"
#include "CIAnalysis/CIStudies/interface/TripleMuonTrigger.hh"
#include "CIAnalysis/CIStudies/interface/TwoInvariantMassesHist.hh"
#include "CIAnalysis/CIStudies/interface/UnusualFinalStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/PhotonsHist.hh"
#include "CIAnalysis/CIStudies/interface/METModule.hh"
#include "CIAnalysis/CIStudies/interface/METHist.hh"
#include "CIAnalysis/CIStudies/interface/OppositeSignInvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/PtHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/METTrigger.hh"
#include "CIAnalysis/CIStudies/interface/SignFlipModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype1DGeneral.hh"


using std::make_shared;

Analyzer hPlusPlusMassAnalysis() {
  Analyzer analyzer;

  auto eventDump = make_shared<GenSimEventDumpModule>(7);

  //auto genSimMod = make_shared<GenSimIdentificationModule>(9900041, true);
  //auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>();
  auto triggerMod = make_shared<TriggerModule>();
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();
  auto mETMod = make_shared<METModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>(); //Needs to be updated with shared pointers
  
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod);
  auto signFlip = make_shared<SignFlipModule>(matchMod);

  auto recoThirdMuonPtHist = make_shared<ThirdMuonPtHist>(false, std::string("Reconstructed Third Muon Transverse Momentum"), 50, 0, 3000);
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
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  analyzer.addProductionModule(mETMod);

  // Filters
  // 09/12: Add nLeptons filer 
  analyzer.addFilterModule(nLeptonsFilter);
  //analyzer.addFilterModule(unusualFinalStateFilter);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
  analyzer.addAnalysisModule(signFlip);
  //analyzer.addAnalysisModule(eventDump);
  analyzer.addAnalysisModule(leptonEfficiency);

  

  return analyzer;
}
