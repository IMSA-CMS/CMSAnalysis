#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"
#include "CMSAnalysis/Filters/interface/BJetCut.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsTriggerCut.hh"
#include "CMSAnalysis/Filters/interface/HiggsZVetoFilter.hh"
#include "CMSAnalysis/Filters/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/Filters/interface/RunFilter.hh"
#include "CMSAnalysis/Histograms/interface/HistogramPrototype2DProjection.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonsHist.hh"
#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/Histograms/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonEfficiency.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/TriggerModule.hh"
#include "CMSAnalysis/Plans/interface/CommonOperations.hh"
#include <memory>

using std::make_shared;

void HiggsBackgroundPlan::initialize()
{
    auto &modules = getModules();

    auto eventMod = make_shared<EventModule>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto higgsSelector = make_shared<HiggsSelector>();
    auto higgsCut = make_shared<HiggsCut>();
    //auto repeatedEventCuts = make_shared<RepeatedEventCuts>();
    auto eventDump = make_shared<GenSimEventDumpModule>(5);
    auto bJetCut = make_shared<BJetCut>();
    //auto quarkoniaCut = make_shared<QuarkoniaCut>();
    //auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Ele27_WPTight_Gsf", "HLT_IsoMu24"});
    auto triggerCut = make_shared<HiggsTriggerCut>();
    eventMod->addSelector(hppSelector);
    eventMod->addSelector(higgsSelector);
    eventMod->addCut(triggerCut);
    eventMod->addCut(higgsCut);
    eventMod->addCut(bJetCut);
    //eventMod->addCut(quarkoniaCut);
    CommonOperations::addHiggsScaleFactors(eventMod);

    auto matchMod = make_shared<MatchingModule>();
    auto triggerMod = make_shared<TriggerModule>();
    auto metMod = make_shared<METModule>();
    // auto bJetFilter = make_shared<BJetFilter>();

    auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventInput());

    auto zVetoFilter = make_shared<HiggsZVetoFilter>();
    auto zVetoFilterMod = make_shared<FilterModule>(zVetoFilter);
    zVetoFilterMod->setInput(eventMod->getEventInput());

    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    // auto filterStringModule = make_shared<FilterStringModule>();
    // modules.addAnalysisModule(filterStringModule);

    auto nLeptonsFilter = make_shared<NLeptonsFilter>();

    auto histMod = make_shared<HistogramOutputModule>();

    auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod);

    auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(
        EventInput::RecoLevel::GenSim, "GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto recoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(
        EventInput::RecoLevel::Reco, "Reco Same Sign Invariant Mass", 1000, 0, 2000);

    auto oppositeSignInvMassHist = make_shared<OppositeSignInvariantMassHist>(
        EventInput::RecoLevel::GenSim, "GenSim Opposite Sign Invariant Mass", 1000, 0, 2000);
    auto recoOppositeSignInvMassHist = make_shared<OppositeSignInvariantMassHist>(
        EventInput::RecoLevel::Reco, "Reco Opposite Sign Invariant Mass", 1000, 0, 2000);

    auto positiveNegativeInvMassHist =
        make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
    auto xProjection = make_shared<HistogramPrototype2DProjection>(
        "Reco Invariant Mass Background X Projection", positiveNegativeInvMassHist, true, 2000);
    auto yProjection = make_shared<HistogramPrototype2DProjection>(
        "Reco Invariant Mass Background Y Projection", positiveNegativeInvMassHist, false, 2000);

    auto highestLeptonPt = make_shared<PtHist>(EventInput::RecoLevel::Reco, "Highest Lepton Pt", 100, 0, 1000);

    auto eventHistMod = eventMod->getHistogramModule();

    auto metHist = make_shared<METHist>("MET", 500, 0, 2000);

    eventHistMod->addHistogram(metHist);
    eventHistMod->addHistogram(recoSameSignInvMassHist);
    eventHistMod->addHistogram(sameSignInvMassHist);
    eventHistMod->addHistogram(positiveNegativeInvMassHist);
    eventHistMod->addHistogram(oppositeSignInvMassHist);
    eventHistMod->addHistogram(recoOppositeSignInvMassHist);
    eventHistMod->addHistogram(highestLeptonPt);
    eventHistMod->addHistogram(xProjection);
    eventHistMod->addHistogram(yProjection);

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
    modules.addProductionModule(matchMod);
    modules.addFilterModule(zVetoFilterMod);
    modules.addFilterModule(recoDecayFilterMod);
    //modules.addFilterModule(make_shared<FilterModule>(bJetFilter));
    modules.addAnalysisModule(leptonEfficiency);
    modules.addAnalysisModule(eventHistMod);
    modules.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
    //modules.addFilterModule(runFilterMod);
    modules.addAnalysisModule(eventDump);
    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
    hPlusPlusEfficiency->setInput(eventMod->getEventInput());
    modules.addAnalysisModule(hPlusPlusEfficiency);
}
