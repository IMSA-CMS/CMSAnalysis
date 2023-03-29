#include "CMSAnalysis/DataCollection/interface/LeptonJetBackgroundPlan.hh"

#include <iostream>
#include <memory>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/METModule.hh"
#include "CMSAnalysis/Histograms/interface/NLeptonJetHist.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/Histograms/interface/Histograms.hh"
#include "CMSAnalysis/Filters/interface/SnowmassLeptonSelector.hh"
#include "CMSAnalysis/Histograms/interface/METHist.hh"
#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"


using std::make_shared;

void LeptonJetBackgroundPlan::initialize()
{
    Analyzer& analyzer = getAnalyzer();

    // auto genSimMod = make_shared<GenSimIdentificationModule>(1000022, true);
    // auto recoMod = make_shared<RecoIdentificationModule>(5);
    auto metMod = make_shared<METModule>();
    auto eventDump = make_shared<EventDumpModule>(true,true);

    auto leptonJetRecoModule = make_shared<LeptonJetReconstructionModule>(0.05);
    
    auto nLeptonJetsHist = make_shared<NLeptonJetHist>(leptonJetRecoModule, "Number of Lepton Jets", 10, 0, 10);
    auto histMod = make_shared<HistogramOutputModule>();

    auto recoPt = make_shared<PtHist>(InputModule::RecoLevel::Reco, "Leading lepton pT", 500, 0, 1000);
    auto recoInvMass = make_shared<InvariantMassHist>(InputModule::RecoLevel::Reco, "Opposite-sign dilepton mass", 1000, 0, 2000);
    auto metHist = make_shared<METHist>(metMod, "MET", 500, 0, 1000);

    // Add the histogram(s) created above to histMod
    histMod->addHistogram(recoPt);
    histMod->addHistogram(recoInvMass);
    histMod->addHistogram(metHist);
    histMod->addHistogram(nLeptonJetsHist);

    // analyzer.addProductionModule(genSimMod);
    // analyzer.addProductionModule(recoMod);
    analyzer.addProductionModule(leptonJetRecoModule);
    analyzer.addProductionModule(metMod);

    analyzer.addAnalysisModule(histMod); // Don't remove unless you don't want histograms
    analyzer.addAnalysisModule(eventDump);
    /*
    auto leptonSelector = std::make_shared<SnowmassLeptonSelector>(5);
    analyzer.getInputModule()->setLeptonSelector(leptonSelector);
    */
}