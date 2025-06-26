#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/Modules/interface/FilterStringModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
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
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"

#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/RunFilter.hh"
#include "CMSAnalysis/Filters/interface/ZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/FourLeptonCut.hh"
#include "CMSAnalysis/Plans/interface/CommonOperations.hh"
#include "CMSAnalysis/Modules/interface/ScaleFactorAnalysis.hh"

using std::make_shared;

void HiggsBackgroundPlan::initialize()
{
    
    auto& modules = getModules();
    
    auto eventMod = make_shared<EventModule>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto higgsSelector = make_shared<HiggsSelector>();
    auto higgsCut = make_shared<HiggsCut>();
    //auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<EventDumpModule>(true,true,5);
    auto zVetoCut = make_shared<ZVetoCut>();
    //auto quarkoniaCut = make_shared<QuarkoniaCut>();
    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});
    eventMod->addSelector(hppSelector);
    eventMod->addSelector(higgsSelector);
    eventMod->addCut(triggerCut);
    eventMod->addCut(higgsCut);
    eventMod->addCut(zVetoCut);
    //eventMod->addCut(quarkoniaCut);
    CommonOperations::addHiggsScaleFactors(eventMod);

    auto matchMod = make_shared<MatchingModule>();
    auto triggerMod = make_shared<TriggerModule>();
    auto metMod = make_shared<METModule>();
    auto bJetFilter = make_shared<BJetFilter>();
    auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventInput());
    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    auto filterStringModule = make_shared<FilterStringModule>();
    modules.addAnalysisModule(filterStringModule);
    
 
 
    auto nLeptonsFilter = make_shared<NLeptonsFilter>();
    
    auto histMod = make_shared<HistogramOutputModule>();

    auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Reco Same Sign Invariant Mass", 1000, 0, 2000);

    auto oppositeSignInvMassHist = make_shared<OppositeSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "GenSim Opposite Sign Invariant Mass", 1000, 0, 2000);
    auto recoOppositeSignInvMassHist = make_shared<OppositeSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Reco Opposite Sign Invariant Mass", 1000, 0, 2000);

    auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
    auto highestLeptonPt = make_shared<PtHist>(EventInput::RecoLevel::Reco, "Highest Lepton Pt", 100, 0, 1000);
    
    auto eventHistMod = eventMod->getHistogramModule();

    auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 2000);


    eventHistMod->addHistogram(metHist);
    eventHistMod->addHistogram(recoSameSignInvMassHist);
    eventHistMod->addHistogram(sameSignInvMassHist);
    eventHistMod->addHistogram(positiveNegativeInvMassHist);
    eventHistMod->addHistogram(oppositeSignInvMassHist);
    eventHistMod->addHistogram(recoOppositeSignInvMassHist);
    eventHistMod->addHistogram(highestLeptonPt);

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
    runFilter->addRunNumber(302596);
    runFilter->addRunNumber(302597);
    auto runFilterMod = make_shared<FilterModule>(runFilter);


    // modules.addProductionModule(metMod);
    // //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);
     modules.addFilterModule(recoDecayFilterMod);
    modules.addAnalysisModule(eventHistMod);    
    modules.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
    //modules.addFilterModule(runFilterMod); 
    modules.addAnalysisModule(eventDump);
    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
    hPlusPlusEfficiency->setInput(eventMod->getEventInput());
    modules.addAnalysisModule(hPlusPlusEfficiency);
}
