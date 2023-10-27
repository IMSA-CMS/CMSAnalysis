#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetDataStripModule.hh"
#include "CMSAnalysis/Plans/interface/MLVariablesPlan.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"
#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"


using std::make_shared;

void MLVariablesPlan::initialize()
{
    auto& modules = getModules();
    auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.5);
    auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.5);
    auto leptonJetMLStripMod = make_shared<LeptonJetMLStripModule>(leptonJetRecoMod);
    auto treeMakerMod = make_shared<TreeMakerModule>(leptonJetMLStripMod);
    modules.addProductionModule(leptonJetRecoMod);
    modules.addProductionModule(leptonJetMatchingMod);
    modules.addAnalysisModule(treeMakerMod);
}