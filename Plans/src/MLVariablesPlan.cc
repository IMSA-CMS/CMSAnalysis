#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetDataStripModule.hh"
#include "CMSAnalysis/Plans/interface/MLVariablesPlan.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetTreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/LeptonJetSelector.hh"
#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Filters/interface/HighestMuonPtCut.hh"

using std::make_shared;

void MLVariablesPlan::initialize()
{
    auto& modules = getModules();
    auto leptonJetRecoMod = make_shared<LeptonJetReconstructionModule>(0.5);
    auto leptonJetMatchingMod = make_shared<LeptonJetMatchingModule>(leptonJetRecoMod, 0.5);
    auto eventMod = std::make_shared<EventModule>();

    eventMod->addSelector(std::make_shared<LeptonJetSelector>(0.5));//(leptonJetRecoMod)

    auto triggerCut = make_shared<TriggerCut>(std::vector<std::string>{"HLT_Mu37_TkMu27", "HLT_IsoMu24"});
    auto highestMuonPtCut = make_shared<HighestMuonPtCut>(40);

    // eventMod->addCut(triggerCut);
    // eventMod->addCut(highestMuonPtCut);

    auto leptonJetMLStripMod = make_shared<LeptonJetMLStripModule>();
    leptonJetMLStripMod->setInput(eventMod->getEventInput());
    auto treeMakerMod = make_shared<LeptonJetTreeMakerModule>(leptonJetMLStripMod, "Signal");
    treeMakerMod->setInput(eventMod->getEventInput());

    modules.addProductionModule(eventMod);
    modules.addProductionModule(leptonJetRecoMod);
    modules.addProductionModule(leptonJetMatchingMod);
    modules.addProductionModule(leptonJetMLStripMod);
    modules.addAnalysisModule(treeMakerMod);

    //DY50Run2.txt - background numFiles=1-5
}