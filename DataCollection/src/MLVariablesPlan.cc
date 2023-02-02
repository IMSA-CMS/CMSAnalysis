#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetDataStripModule.hh"
#include "CMSAnalysis/DataCollection/interface/MLVariablesPlan.hh"

using std::make_shared;

void MLVariablesPlan::initialize()
{
    Analyzer &analyzer = getAnalyzer();
    auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.5);
    auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.1);
    auto leptonJetDataStripMod = make_shared<LeptonJetDataStripModule>("data.root", leptonJetRecoMod, leptonJetMatchingMod);
    analyzer.addProductionModule(leptonJetRecoMod);
    analyzer.addProductionModule(leptonJetMatchingMod);
    analyzer.addAnalysisModule(leptonJetDataStripMod);
}