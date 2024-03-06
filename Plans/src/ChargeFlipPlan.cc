#include "CMSAnalysis/Plans/interface/ChargeFlipPlan.hh"
#include <memory>
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"

using std::make_shared;

void ChargeFlipPlan::initialize()
{
	auto& modules = getModules();
	auto matchMod = make_shared<MatchingModule>();
	auto signFlip = make_shared<SignFlipModule>(matchMod);
	modules.addProductionModule(matchMod);
	modules.addAnalysisModule(signFlip);
}