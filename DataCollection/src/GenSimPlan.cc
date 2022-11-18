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

void GenSimPlan::setupAnalyzer()
{

    Analyzer& analyzer = getAnalyzer();

    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 1);

    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HPlusPlusGenSimSelector>();
    auto metMod = make_shared<METModule>();

    
    auto histMod = make_shared<HistogramOutputModule>();
    auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);

    histMod->addHistogram(metHist);
    histMod->addHistogram(deltaR);

    eventMod->addSelector(dpSelector);
    //eventMod->addSelector(hppSelector);
    auto eventHistMod = eventMod->getHistogramModule();

    analyzer.addProductionModule(metMod);
    analyzer.addAnalysisModule(eventMod);
    analyzer.addAnalysisModule(eventHistMod);

    analyzer.addAnalysisModule(histMod);

}
