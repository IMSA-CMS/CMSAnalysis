#include "CMSAnalysis/DataCollection/interface/GenSimPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonFilter.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/METHist.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsFilter.hh"
#include "CMSAnalysis/DataCollection/interface/NLeptonsHist.hh"
#include "CMSAnalysis/DataCollection/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/DataCollection/interface/SingleElectronTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SingleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/SnowmassCutFilter.hh"
#include "CMSAnalysis/DataCollection/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/DataCollection/interface/Histograms.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerModule.hh"
#include "CMSAnalysis/DataCollection/interface/TripleMuonTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/DataCollection/interface/BJetFilter.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimDeltaRHist.hh"

#include "CMSAnalysis/DataCollection/interface/EventModule.hh"
#include "CMSAnalysis/DataCollection/interface/DarkPhotonGenSimSelector.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusGenSimSelector.hh"

using std::make_shared;

GenSimPlan::GenSimPlan()
{

    Analyzer& analyzer = getAnalyzer();

    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 1);

    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto matchMod = make_shared<MatchingModule>();
    auto triggerMod = make_shared<TriggerModule>();
    auto metMod = make_shared<METModule>();
    auto bJetFilter = make_shared<BJetFilter>();
    
    auto histMod = make_shared<HistogramOutputModule>();
    auto nLeptonsHist = make_shared<NLeptonsHist>(matchMod, "Matched Leptons", 10, 0, 10);

    auto genSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::GenSim, "GenSim Same Sign Invariant Mass", 100, 0, 1000, false, false);

    auto genSimPt = make_shared<PtHist>(InputModule::RecoLevel::GenSim, "Leading lepton pT", 500, 0, 1000);
    auto genSimInvMass = make_shared<InvariantMassHist>(InputModule::RecoLevel::GenSim, "Opposite-sign dilepton mass", 1000, 0, 2000);
    //auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);

    // Add the histogram(s) created above to histMod
    histMod->addHistogram(genSimSameSignInvMassHist);
    histMod->addHistogram(genSimPt);
    histMod->addHistogram(genSimInvMass);
    histMod->addHistogram(deltaR);
    //histMod->addHistogram(metHist);

    eventMod->addSelector(dpSelector);
    //eventMod->addSelector(hppSelector);
    auto eventHistMod = eventMod->getHistogramModule();

    auto elecGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::GenSim, "Electron GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto muonGenSimSameSignInvMassHist = make_shared<SameSignInvariantMassHist>(InputModule::RecoLevel::GenSim, "Muon GenSim Same Sign Invariant Mass", 1000, 0, 2000, false, false);

    histMod->addHistogram(elecGenSimSameSignInvMassHist);
    histMod->addHistogram(muonGenSimSameSignInvMassHist);

    analyzer.addProductionModule(metMod);
    analyzer.addAnalysisModule(eventMod);
    analyzer.addAnalysisModule(eventHistMod);

    analyzer.addAnalysisModule(histMod);

}