#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsDataStripModule.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsDataStripPlan.hh"

using std::make_shared;

HiggsDataStripPlan::HiggsDataStripPlan()
{
	Analyzer &analyzer = getAnalyzer();
	auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.1);
	auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.1);
	auto dataStripMod = make_shared<HiggsDataStripModule>("data.root", leptonJetRecoMod, leptonJetMatchingMod);
	analyzer.addProductionModule(leptonJetRecoMod);
	analyzer.addProductionModule(leptonJetMatchingMod);
	analyzer.addAnalysisModule(dataStripMod);
}