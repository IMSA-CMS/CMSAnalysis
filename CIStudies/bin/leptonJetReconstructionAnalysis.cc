#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/MassRecoEfficiency.hh"
#include "CIAnalysis/CIStudies/interface/GenSimEventDumpModule.hh"


Analyzer leptonJetReconstructionAnalysis()
{
  Analyzer analyzer;

  auto genSimMod = make_shared<GenSimIdentificationModule>(0);
  auto recoMod = make_shared<RecoIdentificationModule>(5);
  auto matchMod = make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();
  auto genSimEventDumpMod = make_shared<GenSimEventDumpModule>();

  // Add the histogram(s) created above to histMod
  //histMod->addHistogram(nLeptonsHist);
  //histMod->addHistogram(nElectronsHist);
  //histMod->addHistogram(nMuonsHist);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);

  //analyzer.addAnalysisModule(leptonEfficiency);
  //analyzer.addAnalysisModule(massRecoEfficiency200);
  //analyzer.addAnalysisModule(massRecoEfficiency500);
  //analyzer.addAnalysisModule(massRecoEfficiency800);
  //analyzer.addAnalysisModule(massRecoEfficiency1000);
  //analyzer.addAnalysisModule(massRecoEfficiency1300);
  analyzer.addAnalysisModule(genSimEventDumpMod);
  
  return analyzer;
}
