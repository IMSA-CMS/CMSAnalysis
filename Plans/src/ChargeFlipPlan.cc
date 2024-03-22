#include "CMSAnalysis/Plans/interface/ChargeFlipPlan.hh"
#include <memory>
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"

using std::make_shared;

void ChargeFlipPlan::initialize()
{
	auto& modules = getModules();

	auto eventMod = make_shared<EventModule>();
	eventMod->addSelector(make_shared<HiggsSelector>());
	auto matchMod = make_shared<MatchingModule>();
	matchMod->setInput(eventMod->getEventInput());
	auto signFlip = make_shared<SignFlipModule>(matchMod);
	modules.addProductionModule(matchMod);
	modules.addAnalysisModule(signFlip);

	//auto histMod = make_shared<HistogramOutputModule>();
	//auto ChargeFlipHist = make_shared<ChargeFlipHist>(EventInput::RecoLevel::Reco, "Charge Flip Rates", 300, 0, 1000);
	//histMod->addHistogram(ChargeFlipHist);
	//modules.addAnalysisModule(histMod);
}