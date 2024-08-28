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
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/ZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/FourLeptonCut.hh"
#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"

using std::make_shared;

void HiggsBackgroundPlan::initialize()
{
    
    auto& modules = getModules();
    
    auto eventMod = make_shared<EventModule>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto higgsSelector = make_shared<ElectronEnergyScaleDown>();
    auto higgsCut = make_shared<HiggsCut>();
    //auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<GenSimEventDumpModule>();
    auto zVetoCut = make_shared<ZVetoCut>();
    //auto quarkoniaCut = make_shared<QuarkoniaCut>();
    // auto scaleFactor = make_shared<JSONScaleFactor>("ScaleFactors_Muon_highPt_RECO_2018_schemaV2.txt");
    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});
    auto scaleFactor = make_shared<MultiYearScaleFactor>();
    scaleFactor->addScaleFactor("2018", JSONScaleFactor("ScaleFactors_Muon_highPt_RECO_2018_schemaV2.json"));
    scaleFactor->addScaleFactor("2017", JSONScaleFactor("ScaleFactors_Muon_highPt_RECO_2017_schemaV2.json"));
    scaleFactor->addScaleFactor("2016", JSONScaleFactor("ScaleFactors_Muon_highPt_RECO_2016_schemaV2.json"));
    scaleFactor->addScaleFactor("2016APV", JSONScaleFactor("ScaleFactors_Muon_highPt_RECO_2016_preVFP_schemaV2.json"));


    eventMod->addSelector(hppSelector);
    eventMod->addSelector(higgsSelector);
    eventMod->addCut(triggerCut);
    eventMod->addCut(higgsCut);
    eventMod->addCut(zVetoCut);
    //eventMod->addCut(quarkoniaCut);
    eventMod->addScaleFactor(scaleFactor);


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
    auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
 
    auto eventHistMod = eventMod->getHistogramModule();

    auto MetHist = make_shared<METHist>(metMod, "MET", 500, 0, 2000);


    eventHistMod->addHistogram(MetHist);
    eventHistMod->addHistogram(recoSameSignInvMassHist);
    eventHistMod->addHistogram(sameSignInvMassHist);
    eventHistMod->addHistogram(positiveNegativeInvMassHist);


    modules.addProductionModule(metMod);
    //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);
    modules.addFilterModule(make_shared<FilterModule>(recoDecayFilter));
    modules.addAnalysisModule(eventHistMod);    
    modules.addAnalysisModule(histMod); // Don't remove unless you don't want histograms

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
    hPlusPlusEfficiency->setInput(eventMod->getEventInput());
    modules.addAnalysisModule(hPlusPlusEfficiency);
}
