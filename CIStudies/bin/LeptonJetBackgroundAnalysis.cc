#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonJetHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"


using std::make_shared;

Analyzer leptonJetBackgroundAnalysis() {
  Analyzer analyzer;

  auto genSimMod = make_shared<GenSimIdentificationModule>(1000022, true);
  auto recoMod = make_shared<RecoIdentificationModule>(5);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();
  
  auto leptonJetRecoModule = make_shared<LeptonJetReconstructionModule>(recoMod, 0.05);
  
  auto nLeptonJetsHist = make_shared<NLeptonJetHist>(leptonJetRecoModule, "Number of Lepton Jets", 10, 0, 10);
  auto histMod = make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);


  histMod->addHistogram(nLeptonJetsHist);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(leptonJetRecoModule);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms

  return analyzer;
}
