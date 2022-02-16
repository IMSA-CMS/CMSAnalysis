#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "CIAnalysis/CIStudies/interface/HistogramOutputModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonJetHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/SnowmassLeptonSelector.hh"

using std::make_shared;

Analyzer leptonJetBackgroundAnalysis() 
{
  Analyzer analyzer;

  // auto genSimMod = make_shared<GenSimIdentificationModule>(1000022, true);
  // auto recoMod = make_shared<RecoIdentificationModule>(5);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();
  auto metMod = make_shared<METModule>();

  auto leptonJetRecoModule = make_shared<LeptonJetReconstructionModule>(0.05);
  
  auto nLeptonJetsHist = make_shared<NLeptonJetHist>(leptonJetRecoModule, "Number of Lepton Jets", 10, 0, 10);
  auto histMod = make_shared<HistogramOutputModule>(weightMod, lrWeightMod);

  auto recoPt = make_shared<PtHist>(false, "Leading lepton pT", 500, 0, 1000);
  auto recoInvMass = make_shared<InvariantMassHist>(false, "Opposite-sign dilepton mass", 1000, 0, 2000);
  auto metHist = make_shared<METHist>(metMod, "MET", 350, 10, 360);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(recoPt);
  histMod->addHistogram(recoInvMass);
  histMod->addHistogram(metHist);
  histMod->addHistogram(nLeptonJetsHist);

  // analyzer.addProductionModule(genSimMod);
  // analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(leptonJetRecoModule);
  analyzer.addProductionModule(weightMod);
  analyzer.addProductionModule(lrWeightMod);
  analyzer.addProductionModule(metMod);

  analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms

  auto leptonSelector = std::make_shared<SnowmassLeptonSelector>(5);
  analyzer.getInputModule().setLeptonSelector(leptonSelector);

  return analyzer;
}
