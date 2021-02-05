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


Analyzer hPlusPlusMassAnalysis()
{
  Analyzer analyzer;

  auto genSimMod = make_shared<GenSimIdentificationModule>(9900041);
  auto recoMod = make_shared<RecoIdentificationModule>(50);
  auto matchMod = make_shared<MatchingModule>(genSimMod, recoMod);
  auto weightMod = make_shared<WeightingModule>();
  auto lrWeightMod = make_shared<LRWeightModule>();

  auto nLeptonsFilter = make_shared<NLeptonsFilter>(matchMod); //Needs to be updated with shared pointers
  
  auto histMod = make_shared<HistogramOutputModule>(genSimMod, recoMod, weightMod, lrWeightMod);
  auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);
  auto nElectronsHist = make_shared<NLeptonsHist>(matchMod, "Matched Electrons", 10, 0, 10, 11);
  auto nMuonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Muons", 10, 0, 10, 13);

  auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod, genSimMod);
  auto massRecoEfficiency200 = make_shared<MassRecoEfficiency>(recoMod, 200, 10);
  auto massRecoEfficiency500 = make_shared<MassRecoEfficiency>(recoMod, 500, 25);
  auto massRecoEfficiency800 = make_shared<MassRecoEfficiency>(recoMod, 800, 40);
  auto massRecoEfficiency1000 = make_shared<MassRecoEfficiency>(recoMod, 1000, 50);
  auto massRecoEfficiency1300 = make_shared<MassRecoEfficiency>(recoMod, 1300, 65);

  // Add the histogram(s) created above to histMod
  histMod->addHistogram(nLeptonsHist);
  histMod->addHistogram(nElectronsHist);
  histMod->addHistogram(nMuonsHist);

  analyzer.addProductionModule(genSimMod);
  analyzer.addProductionModule(recoMod);
  analyzer.addProductionModule(matchMod);

  analyzer.addAnalysisModule(leptonEfficiency);
  analyzer.addAnalysisModule(massRecoEfficiency200);
  analyzer.addAnalysisModule(massRecoEfficiency500);
  analyzer.addAnalysisModule(massRecoEfficiency800);
  analyzer.addAnalysisModule(massRecoEfficiency1000);
  analyzer.addAnalysisModule(massRecoEfficiency1300);
  
  return analyzer;
}