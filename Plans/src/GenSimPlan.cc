#include "CMSAnalysis/Plans/interface/GenSimPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Filters/interface/DoubleElectronTrigger.hh"
#include "CMSAnalysis/Filters/interface/DoubleMuonTrigger.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Filters/interface/LeptonFilter.hh"
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
#include "CMSAnalysis/Histograms/interface/GenSimDeltaRHist.hh"
#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"

#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/DarkPhotonGenSimSelector.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Filters/interface/HiggsMassCut.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Modules/interface/HiggsLeptonEfficiency.hh"


using std::make_shared;

void GenSimPlan::initialize()
{    
    auto& modules = getModules();

    //Analyzer& analyzer = getAnalyzer();

    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 2);
    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HiggsSelector>();
    auto hppGenSimSelector = make_shared<HPlusPlusGenSimSelector>();
    auto metMod = make_shared<METModule>();
    //auto gammahist = make_shared<GammaHistogram>(EventInput::RecoLevel::GenSim, "Gamma", 100, 0, 1000);
    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Same Sign Invariant Mass", 1000, 0, 2000, false, false);
    auto eventDump = make_shared<GenSimEventDumpModule>();
    auto matchMod = make_shared<MatchingModule>();
    auto higgsCut = make_shared<HiggsCut>();
    auto higgsMassCut = make_shared<HiggsMassCut>();
    
    auto histMod = make_shared<HistogramOutputModule>();

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
    //auto leptonEfficiency = make_shared<HiggsLeptonEfficiency>(matchMod);

    hPlusPlusEfficiency->setInput(eventMod->getEventInput());
    //leptonEfficiency->setInput(eventMod->getEventInput());

    histMod->addHistogram(deltaR);
    //histMod->addHistogram(gammahist);

    histMod->addHistogram(deltaR);
    histMod->addHistogram(sameSignInvMassHist);

    //eventMod->addSelector(dpSelector);
    //eventMod->addSelector(hppSelector);
    eventMod->addSelector(hppGenSimSelector);
    //eventMod->addCut(higgsCut);
    //eventMod->addCut(higgsMassCut);
    auto eventHistMod = eventMod->getHistogramModule();
    //auto hppFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);

    //modules.addFilterModule(hppFilter);
    modules.addProductionModule(metMod);
    //Changed because EventModule inherits from ProductionModule now
    modules.addProductionModule(eventMod);
    //modules.addAnalysisModule(eventMod);
    modules.addAnalysisModule(eventHistMod);

	modules.addAnalysisModule(hPlusPlusEfficiency);
    //modules.addAnalysisModule(leptonEfficiency);

    modules.addAnalysisModule(histMod);
    //modules.addAnalysisModule(eventDump);
}