#include "CMSAnalysis/Plans/interface/MuonPlan.hh"

#include <memory>


#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/MuonSelector.hh"
#include "CMSAnalysis/Filters/interface/RunFilter.hh"


using std::make_shared;

void MuonPlan::initialize()
{
    
    auto& modules = getModules();
    
    auto eventMod = make_shared<EventModule>();
    auto muonSelector = make_shared<MuonSelector>();
    //auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<EventDumpModule>(true, true, 5);

    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});

    eventMod->addSelector(muonSelector);
    eventMod->addCut(triggerCut);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventInput());

    auto runFilter = make_shared<RunFilter>();
    runFilter->addRunNumber(302337);
    runFilter->addRunNumber(302392);
    runFilter->addRunNumber(302573);
    runFilter->addRunNumber(302634);
    runFilter->addRunNumber(302635);
    runFilter->addRunNumber(302131);
    runFilter->addRunNumber(302163);
    runFilter->addRunNumber(302225);
    runFilter->addRunNumber(302494);
    runFilter->addRunNumber(302131);
    runFilter->addRunNumber(302596);
    runFilter->addRunNumber(302597);
    auto runFilterMod = make_shared<FilterModule>(runFilter);
    
    auto eventHistMod = eventMod->getHistogramModule();

    //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);
    modules.addFilterModule(recoDecayFilterMod);
    //modules.addFilterModule(runFilterMod);
    modules.addAnalysisModule(eventDump);
    modules.addAnalysisModule(eventHistMod);  
    
      

}
