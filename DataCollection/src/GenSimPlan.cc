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
#include "CMSAnalysis/DataCollection/interface/GenSimEventDumpModule.hh"

#include "CMSAnalysis/DataCollection/interface/EventModule.hh"
#include "CMSAnalysis/DataCollection/interface/DarkPhotonGenSimSelector.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusDecayFilter.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusEfficiency.hh"

using std::make_shared;

void GenSimPlan::initialize()
{
    
    Analyzer& analyzer = getAnalyzer();

    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 2);

    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto metMod = make_shared<METModule>();
    auto gammahist = make_shared<GammaHistogram>(InputModule::RecoLevel::GenSim, "Gamma", 100, 0, 1000);
    auto eventDump = make_shared<GenSimEventDumpModule>();
    
    auto histMod = make_shared<HistogramOutputModule>();

    histMod->addHistogram(deltaR);
    histMod->addHistogram(gammahist);

    //eventMod->addSelector(dpSelector);
    eventMod->addSelector(hppSelector);
    auto eventHistMod = eventMod->getHistogramModule();
    //auto hppFilter = make_shared<HPlusPlusDecayFilter>(InputModule::RecoLevel::GenSim);

    //analyzer.addFilterModule(hppFilter);
    analyzer.addProductionModule(metMod);
    analyzer.addAnalysisModule(eventMod);
    analyzer.addAnalysisModule(eventHistMod);

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
	analyzer.addAnalysisModule(hPlusPlusEfficiency);

    analyzer.addAnalysisModule(histMod);
    //analyzer.addAnalysisModule(eventDump);
}
