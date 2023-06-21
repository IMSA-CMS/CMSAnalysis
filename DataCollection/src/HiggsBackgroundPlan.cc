#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"

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
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/Modules/interface/LocalEventEventInput.hh"
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
    
    Analyzer& analyzer = getAnalyzer();
    
    auto eventMod = make_shared<EventModule>();
    //auto pasSelector = make_shared<PASSelector>();
    auto higgsSelector = make_shared<HiggsSelector>();
    auto higgsCut = make_shared<HiggsCut>();
    auto eventDump = make_shared<GenSimEventDumpModule>();
    //auto fourLeptonCut = make_shared<FourLeptonCut>();
    //auto zVetoCut = make_shared<ZVetoCut>();
    //auto quarkoniaCut = make_shared<QuarkoniaCut>();

    //eventMod->addSelector(pasSelector);
    eventMod->addSelector(higgsSelector);
    eventMod->addCut(higgsCut);
    //eventMod->addCut(fourLeptonCut);
    //eventMod->addCut(zVetoCut);
    //eventMod->addCut(quarkoniaCut);

    auto matchMod = make_shared<MatchingModule>();
    auto triggerMod = make_shared<TriggerModule>();
    auto metMod = make_shared<METModule>();
    auto bJetFilter = make_shared<BJetFilter>();
    auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);

    auto recoDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::Reco);
    auto recoDecayFilterMod = make_shared<FilterModule>(recoDecayFilter);
    recoDecayFilterMod->setInput(eventMod->getEventEventInput());
    auto genSimDecayFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    //analyzer.addFilterModule(make_shared<FilterModule>(recoDecayFilter));
    //analyzer.addFilterModule(make_shared<FilterModule>(genSimDecayFilter));
    auto filterStringModule = make_shared<FilterStringModule>();
    analyzer.addAnalysisModule(filterStringModule);
    


    auto nLeptonsFilter = make_shared<NLeptonsFilter>();
    
    auto histMod = make_shared<HistogramOutputModule>();
    //auto leptonEfficiency = make_shared<LeptonEfficiency>(matchMod);

    auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Electron GenSim Same Sign Invariant Mass", 100, 0, 1000, false, false);
    auto elecGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Electron GenSim Same Sign Invariant Mass", 100, 0, 1000, false, false);
    auto elecRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Electron Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto muonGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Muon GenSim Same Sign Invariant Mass", 100, 0, 1000, false, false);
    auto muonRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Muon Reco Same Sign Invariant Mass", 1000, 0, 500, false, false);
    //auto positiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);

    auto eventHistMod = eventMod->getHistogramModule();
    
    //eventHistMod->addHistogram(recoSameSignInvMassHist);

    auto elecRecoPt = make_shared<PtHist>(EventInput::RecoLevel::Reco, "Electron Reco Leading lepton pT", 500, 0, 1000);
    auto elecGenSimPt = make_shared<PtHist>(EventInput::RecoLevel::GenSim, "Electron GenSim Leading lepton pT", 500, 0, 1000);
    auto muonRecoPt = make_shared<PtHist>(EventInput::RecoLevel::Reco, "Muon Reco Leading lepton pT", 500, 0, 1000);
    auto muonGenSimPt = make_shared<PtHist>(EventInput::RecoLevel::GenSim, "Muon GenSim Leading lepton pT", 500, 0, 1000);
    //auto recoInvMass = make_shared<InvariantMassHist>(EventInput::RecoLevel::Reco, "Opposite-sign dilepton mass", 1000, 0, 2000);
    auto elecMetHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);
    // Add the histogram(s) created above to histMod
    // histMod->addHistogram(elecRecoPt);
    // histMod->addHistogram(elecGenSimPt);
    // histMod->addHistogram(muonRecoPt);
    // histMod->addHistogram(muonGenSimPt);
    // histMod->addHistogram(elecGenSimSameSignInvMassHist);
    // histMod->addHistogram(elecRecoSameSignInvMassHist);
    // histMod->addHistogram(muonGenSimSameSignInvMassHist);
    // histMod->addHistogram(muonRecoSameSignInvMassHist);
    // //histMod->addHistogram(recoInvMass);
    // histMod->addHistogram(elecMetHist);
    // histMod->addHistogram(muonMetHist);

    //auto elecRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Electron Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto elecPositiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Electron Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);
    //auto muonRecoSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::Reco, "Muon Reco Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto muonPositiveNegativeInvMassHist = make_shared<TwoInvariantMassesHist>("Muon Reco Invariant Mass Background", 100, 100, 0, 0, 2000, 2000);

    //auto elecGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Electron GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    //auto muonGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Muon GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);

    auto elecFilter = make_shared<LeptonFilter>(ParticleType::electron(), 4, "Electron");
    auto muonFilter = make_shared<LeptonFilter>(ParticleType::muon(), 4, "Muon");
    auto snowmassCut = make_shared<SnowmassCutFilter>();

    //elecRecoSameSignInvMassHist->addFilter(elecFilter);
    //elecPositiveNegativeInvMassHist->addFilter(elecFilter);
    //muonRecoSameSignInvMassHist->addFilter(muonFilter);
    //muonPositiveNegativeInvMassHist->addFilter(muonFilter);
    //elecRecoPt->addFilter(elecFilter);
    //elecGenSimPt->addFilter(elecFilter);
    //muonRecoPt->addFilter(muonFilter);
    //muonGenSimPt->addFilter(muonFilter);
    //elecGenSimSameSignInvMassHist->addFilter(elecFilter);
    //muonGenSimSameSignInvMassHist->addFilter(muonFilter);
    //elecMetHist->addFilter(elecFilter);


    eventHistMod->addHistogram(sameSignInvMassHist);
    // eventHistMod->addHistogram(elecRecoPt);
    // eventHistMod->addHistogram(elecGenSimPt);
    // eventHistMod->addHistogram(muonRecoPt);
    // eventHistMod->addHistogram(muonGenSimPt);
    // eventHistMod->addHistogram(elecGenSimSameSignInvMassHist);
    // eventHistMod->addHistogram(elecRecoSameSignInvMassHist);
    // eventHistMod->addHistogram(muonGenSimSameSignInvMassHist);
    // eventHistMod->addHistogram(muonRecoSameSignInvMassHist);
    // //histMod->addHistogram(recoInvMass);
    // eventHistMod->addHistogram(elecMetHist);
    // //histMod->addHistogram(elecRecoSameSignInvMassHist);
    // eventHistMod->addHistogram(elecPositiveNegativeInvMassHist);
    // //histMod->addHistogram(muonRecoSameSignInvMassHist);
    // eventHistMod->addHistogram(muonPositiveNegativeInvMassHist);

    analyzer.addProductionModule(metMod);


    // analyzer.addFilterModule(make_shared<FilterModule>(bJetFilter));
    // analyzer.addFilterModule(snowmassCut);
    // analyzer.addFilterModule(nLeptonsFilter);
    analyzer.addFilterModule(recoDecayFilterMod);

    //analyzer.addProductionModule(matchMod);
    analyzer.addAnalysisModule(eventMod);
    analyzer.addAnalysisModule(eventHistMod);    
    analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
    //analyzer.addAnalysisModule(eventDump);

}