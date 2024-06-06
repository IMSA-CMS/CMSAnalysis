#include "CMSAnalysis/Plans/interface/ChargeFlipPlan.hh"
#include <memory>
#include "CMSAnalysis/Modules/interface/EventModule.hh" 
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh" 
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh" 
#include "CMSAnalysis/Modules/interface/MatchingModule.hh" 


using std::make_shared;


void ChargeFlipPlan::initialize()
{
    auto& modules = getModules();


    auto eventMod = make_shared<EventModule>();
    eventMod->addSelector(make_shared<HiggsSelector>());


    auto matchMod = make_shared<MatchingModule>();


    auto signFlip = make_shared<SignFlipModule>(matchMod);
    modules.addProductionModule(eventMod);
    modules.addProductionModule(matchMod);
    modules.addAnalysisModule(signFlip);
}


 