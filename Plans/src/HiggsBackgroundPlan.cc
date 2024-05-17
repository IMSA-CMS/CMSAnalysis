#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Filters/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/Modules/interface/FilterStringModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"
#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Filters/interface/RepeatedEventCuts.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/LocalEventInput.hh"
#include "CMSAnalysis/Filters/interface/LeptonFilter.hh"
#include "CMSAnalysis/Modules/interface/LeptonEfficiency.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonsHist.hh"
#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/Filters/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/Filters/interface/SnowmassCutFilter.hh"
#include "CMSAnalysis/Filters/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Filters/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Filters/interface/BJetFilter.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/PASSelector.hh"
#include "CMSAnalysis/Filters/interface/QuarkoniaCut.hh"
#include "CMSAnalysis/Filters/interface/ZVetoCut.hh"
#include "CMSAnalysis/Filters/interface/FourLeptonCut.hh"

using std::make_shared;

void HiggsBackgroundPlan::initialize()
{
    
    auto& modules = getModules();
    
    auto eventMod = make_shared<EventModule>();
    //auto pasSelector = make_shared<PASSelector>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto higgsSelector = make_shared<HiggsSelector>();
    auto higgsCut = make_shared<HiggsCut>();
    auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<GenSimEventDumpModule>();
    //auto fourLeptonCut = make_shared<FourLeptonCut>();
    auto zVetoCut = make_shared<ZVetoCut>();
    //auto quarkoniaCut = make_shared<QuarkoniaCut>();

    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});

    //eventMod->addSelector(pasSelector);
    eventMod->addSelector(hppSelector);
    eventMod->addSelector(higgsSelector);
    eventMod->addCut(triggerCut);
    eventMod->addCut(higgsCut);
    //eventMod->addCut(repeatedEventCuts);
    //eventMod->addCut(fourLeptonCut);
    eventMod->addCut(zVetoCut);
    //eventMod->addCut(quarkoniaCut);

    auto matchMod = make_shared<MatchingModule>();
    auto triggerMod = make_shared<TriggerModule>();
    auto metMod = make_shared<METModule>();
    auto bJetFilter = make_shared<BJetFilter>();
    auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventInput());
    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    //modules.addFilterModule(make_shared<FilterModule>(recoDecayFilter));
    //modules.addFilterModule(make_shared<FilterModule>(genSimDecayFilter));
    auto filterStringModule = make_shared<FilterStringModule>();
    modules.addAnalysisModule(filterStringModule);
    
 
 
    auto nLeptonsFilter = make_shared<NLeptonsFilter>();
    
    auto histMod = make_shared<HistogramOutputModule>();
    //auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod);

    auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Reco Same Sign Invariant Mass", 1000, 0, 2000);
    auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
    auto oppositeSignInvMassHist = make_shared<OppositeSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Opposite Sign Invariant Mass", 1000, 0, 2000);
 
    auto eventHistMod = eventMod->getHistogramModule();

    auto RecoPt = make_shared<PtHist>(EventInput::RecoLevel::Reco, "Reco Leading lepton pT", 500, 0, 2000);
    auto GenSimPt = make_shared<PtHist>(EventInput::RecoLevel::GenSim, "GenSim Leading lepton pT", 500, 0, 2000);

    auto MetHist = make_shared<METHist>(metMod, "MET", 500, 0, 2000);
    auto deltaXYHist = std::make_shared<DxyHist>(EventInput::RecoLevel::Reco, "Vertex Delta XY from Primary Vertex", 50, 0, 5);
    auto deltaZHist = std::make_shared<DzHist>(EventInput::RecoLevel::Reco, "Vertex Delta Z from Primary Vertex", 50, 0, 5);


    eventHistMod->addHistogram(MetHist);
    eventHistMod->addHistogram(RecoPt);
    eventHistMod->addHistogram(GenSimPt);
    eventHistMod->addHistogram(recoSameSignInvMassHist);
    eventHistMod->addHistogram(sameSignInvMassHist);
    eventHistMod->addHistogram(positiveNegativeInvMassHist);
    eventHistMod->addHistogram(oppositeSignInvMassHist);
    eventHistMod->addHistogram(deltaXYHist);
    eventHistMod->addHistogram(deltaZHist);


    modules.addProductionModule(metMod);
    //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);


    // modules.addFilterModule(make_shared<FilterModule>(bJetFilter));
    // modules.addFilterModule(snowmassCut);
    // modules.addFilterModule(nLeptonsFilter);
    //modules.addFilterModule(make_shared<FilterModule>(genSimDecayFilter));
    modules.addFilterModule(make_shared<FilterModule>(recoDecayFilter)); 
    //modules.addProductionModule(matchMod);
    modules.addAnalysisModule(eventHistMod);    
    modules.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
    //modules.addAnalysisModule(eventDump);

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
    hPlusPlusEfficiency->setInput(eventMod->getEventInput());
    modules.addAnalysisModule(hPlusPlusEfficiency);
}
