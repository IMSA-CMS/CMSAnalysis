#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Modules/interface/HiggsDataStripModule.hh"
#include "CMSAnalysis/Plans/interface/HiggsDataStripPlan.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/HiggsCombineStripModule.hh"
#include "CMSAnalysis/Modules/interface/MultiChannelTreeMakerModule.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"

using std::make_shared;

void HiggsDataStripPlan::initialize()
{
	auto& modules = getModules();
	//auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.1);
	//auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.1);
	//auto dataStripMod = make_shared<HiggsDataStripModule>("data.root", leptonJetRecoMod, leptonJetMatchingMod);
	//auto matchMod = make_shared<MatchingModule>();
	//auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
	auto higgsCombineStripModule = make_shared<HiggsCombineStripModule>();
	auto multiChannelTreeMakerModule = make_shared<MultiChannelTreeMakerModule>(higgsCombineStripModule, "Signal");
    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);

	//modules.addAnalysisModule(hPlusPlusEfficiency);
	//modules.addProductionModule(leptonJetRecoMod);
	//modules.addProductionModule(leptonJetMatchingMod);
	//modules.addAnalysisModule(dataStripMod);
    modules.addFilterModule(make_shared<FilterModule>(recoDecayFilter)); 

	modules.addProductionModule(higgsCombineStripModule);
	modules.addAnalysisModule(multiChannelTreeMakerModule);
}