#include "CMSAnalysis/DataCollection/interface/GenSimPlan.hh"

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
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"


using std::make_shared;

void GenSimPlan::initialize()
{
    
    Analyzer& analyzer = getAnalyzer();

    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 2);

    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto metMod = make_shared<METModule>();
    auto gammahist = make_shared<GammaHistogram>(EventInput::RecoLevel::GenSim, "Gamma", 100, 0, 1000);
    auto eventDump = make_shared<GenSimEventDumpModule>();
    
    auto histMod = make_shared<HistogramOutputModule>();

    histMod->addHistogram(deltaR);
    histMod->addHistogram(gammahist);

    //eventMod->addSelector(dpSelector);
    eventMod->addSelector(hppSelector);
    auto eventHistMod = eventMod->getHistogramModule();
    //auto hppFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);

    //analyzer.addFilterModule(hppFilter);
    analyzer.addProductionModule(metMod);
    //Changed because EventModule inherits from ProductionModule now
    analyzer.addProductionModule(eventMod);
    //analyzer.addAnalysisModule(eventMod);
    analyzer.addAnalysisModule(eventHistMod);

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();
	analyzer.addAnalysisModule(hPlusPlusEfficiency);

    analyzer.addAnalysisModule(histMod);
    //analyzer.addAnalysisModule(eventDump);
}
