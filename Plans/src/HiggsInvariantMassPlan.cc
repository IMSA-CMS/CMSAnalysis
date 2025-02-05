#include "CMSAnalysis/Modules/interface/HiggsInvariantMassModule.hh"
#include "CMSAnalysis/Plans/interface/HiggsInvariantMassPlan.hh"
#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/Modules/interface/FilterStringModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"
#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Filters/interface/RepeatedEventCuts.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Filters/interface/ElectronEnergyScaleUp.hh"
#include "CMSAnalysis/Filters/interface/ElectronEnergyScaleDown.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonsHist.hh"
#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Modules/interface/RecoGenSimComparisonModule.hh"
#include "CMSAnalysis/Filters/interface/ZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/MuonSelector.hh"
#include "CMSAnalysis/Filters/interface/RunFilter.hh"

using std::make_shared;

void HiggsInvariantMassPlan::initialize()
{
    auto fileMod = std::make_shared<HiggsInvariantMassModule>("invariantMassFile.root");

	auto& modules = getModules();
    
    auto eventMod = make_shared<EventModule>();
    auto selector = make_shared<HiggsSelector>();
    //auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<EventDumpModule>(true, true, 20);
    auto compareMod = make_shared<RecoGenSimComparisonModule>("fakePhoton", true);

    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});

    eventMod->addSelector(selector);
    eventMod->addCut(triggerCut);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventInput());

    
    auto eventHistMod = eventMod->getHistogramModule();

    //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);
    modules.addFilterModule(recoDecayFilterMod);
    //modules.addFilterModule(runFilterMod);
    modules.addAnalysisModule(eventDump);
    modules.addAnalysisModule(compareMod);
    modules.addAnalysisModule(eventHistMod);
   	modules.addAnalysisModule(fileMod);
}