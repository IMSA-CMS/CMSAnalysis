#include "CMSAnalysis/Plans/interface/GenSimPlan.hh"

#include <memory>

#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"

#include "CMSAnalysis/Histograms/interface/GenSimDeltaRHist.hh"
#include "CMSAnalysis/Modules/interface/GenSimEventDumpModule.hh"

#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Modules/interface/RecoGenSimComparisonModule.hh"
#include "CMSAnalysis/Filters/interface/DarkPhotonGenSimSelector.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusGenSimSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Filters/interface/HiggsCut.hh"
#include "CMSAnalysis/Filters/interface/HiggsMassCut.hh"
#include "CMSAnalysis/Modules/interface/HPlusPlusEfficiency.hh"
#include "CMSAnalysis/Filters/interface/HPlusPlusDecayFilter.hh"


using std::make_shared;

void GenSimPlan::initialize()
{    
    auto& modules = getModules();


    auto deltaR = make_shared<GenSimDeltaRHist>("Delta R", 100, 0, 2);
    auto eventMod = make_shared<EventModule>();
    auto dpSelector = make_shared<DarkPhotonGenSimSelector>();
    auto hppSelector = make_shared<HiggsSelector>();
    auto hppGenSimSelector = make_shared<HPlusPlusGenSimSelector>();
    auto higgsFilter = make_shared<HPlusPlusDecayFilter>(EventInput::RecoLevel::GenSim);
    auto metMod = make_shared<METModule>();
    auto recoGenMatch = make_shared<RecoGenSimComparisonModule>();

    auto sameSignInvMassHist = make_shared<SameSignInvariantMassHist>(EventInput::RecoLevel::GenSim, "Same Sign Invariant Mass", 1000, 0, 2000, false, false);

    auto eventDump = make_shared<GenSimEventDumpModule>();
    auto matchMod = make_shared<MatchingModule>();
    auto higgsCut = make_shared<HiggsCut>();
    auto higgsMassCut = make_shared<HiggsMassCut>();
    
    auto histMod = make_shared<HistogramOutputModule>();

    auto hPlusPlusEfficiency = make_shared<HPlusPlusEfficiency>();


    hPlusPlusEfficiency->setInput(eventMod->getEventInput());

    histMod->addHistogram(deltaR);
    histMod->addHistogram(sameSignInvMassHist);


    eventMod->addSelector(hppSelector);
    eventMod->addSelector(hppGenSimSelector);
    auto eventHistMod = eventMod->getHistogramModule();

    modules.addProductionModule(metMod);
    modules.addProductionModule(eventMod);
    modules.addFilterModule(make_shared<FilterModule>(higgsFilter));
    modules.addAnalysisModule(eventHistMod);

	modules.addAnalysisModule(hPlusPlusEfficiency);
    modules.addAnalysisModule(recoGenMatch);
    modules.addAnalysisModule(histMod);
}
