#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/DataStripModule.hh"
#include "CMSAnalysis/DataCollection/interface/MLVariablesPlan.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"

using std::make_shared;

MLVariablesPlan::MLVariablesPlan()
{

}

void MLVariablesPlan::initialize()
{
    Analyzer &analyzer = getAnalyzer();
    auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.1);
    auto genSimEventDump = make_shared<GenSimEventDumpModule>();
    auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.1);
    auto dataStripMod = make_shared<DataStripModule>("data.root", leptonJetRecoMod, leptonJetMatchingMod);
    analyzer.addProductionModule(leptonJetRecoMod);
    analyzer.addProductionModule(leptonJetMatchingMod);
    analyzer.addAnalysisModule(dataStripMod);
}