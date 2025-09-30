#include "CMSAnalysis/Plans/interface/CommonOperations.hh"
//#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"
//#include "CMSAnalysis/Modules/interface/ElectronScaleFactor.hh"
//#include "CMSAnalysis/Modules/interface/MuonScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/DummyScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Modules/interface/RootReader.hh"
#include "CMSAnalysis/Modules/interface/MuonJSONReader.hh"
#include "CMSAnalysis/Modules/interface/ElectronJSONReader.hh"
#include "CMSAnalysis/Modules/interface/MultiYearScaleFactorReader.hh"
#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"




using std::make_shared;

void CommonOperations::addHiggsScaleFactors(std::shared_ptr<EventModule> eventMod)
{
   // auto pileUpScaleFactor = make_shared<MuonScaleFactor>("","");
    auto recoScaleFactorReader = make_shared<MultiYearScaleFactorReader>();
    
    // Muon Reco
    auto muonScaleFactor2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_schemaV2.json");
    recoScaleFactorReader->addYearData("2016", muonScaleFactor2016);
    auto muonScaleFactor2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2017_schemaV2.json");
    recoScaleFactorReader->addYearData("2017", muonScaleFactor2017);
    auto muonScaleFactor2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2018_schemaV2.json");
    recoScaleFactorReader->addYearData("2018", muonScaleFactor2018);
    auto muonScaleFactor2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_preVFP_schemaV2.json");
    recoScaleFactorReader->addYearData("2016APV", muonScaleFactor2016APV);
    
    // Trigger 
        auto triggerScaleFactorReader = make_shared<MultiYearScaleFactorReader>();
        auto muonTriggerScaleFactor2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_preVFP_schemaV2.json");
        triggerScaleFactorReader->addYearData("2016APV", muonTriggerScaleFactor2016APV);
        auto muonTriggerScaleFactor2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_schemaV2.json");
        triggerScaleFactorReader->addYearData("2016", muonTriggerScaleFactor2016);
        auto muonTriggerScaleFactor2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2017_schemaV2.json");
        triggerScaleFactorReader->addYearData("2017", muonTriggerScaleFactor2017);
        auto muonTriggerScaleFactor2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2018_schemaV2.json");
        triggerScaleFactorReader->addYearData("2018", muonTriggerScaleFactor2018);
    
    // IDISO
        auto iDISOScaleFactorReader = make_shared<MultiYearScaleFactorReader>();
        auto muonIDISOScaleFactor2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_preVFP_schemaV2.json");
        iDISOScaleFactorReader->addYearData("2016APV", muonIDISOScaleFactor2016APV);
        auto muonIDISOScaleFactor2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_schemaV2.json");
        iDISOScaleFactorReader->addYearData("2016", muonIDISOScaleFactor2016);
        auto muonIDISOScaleFactor2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2017_schemaV2.json");
        iDISOScaleFactorReader->addYearData("2017", muonIDISOScaleFactor2017);
        auto muonIDISOScaleFactor2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2018_schemaV2.json");
        iDISOScaleFactorReader->addYearData("2018", muonIDISOScaleFactor2018);
    
    // Electron
        auto electronScaleFactorReader = make_shared<MultiYearScaleFactorReader>();
        auto electronScaleFactor2016APV = make_shared<ElectronJSONReader>("ScaleFactors/2016APVelectron.json");
        electronScaleFactorReader->addYearData("2016APV", electronScaleFactor2016APV);
        auto electronScaleFactor2016 = make_shared<ElectronJSONReader>("ScaleFactors/2016electron.json");
        electronScaleFactorReader->addYearData("2016", electronScaleFactor2016);
        auto electronScaleFactor2017 = make_shared<ElectronJSONReader>("ScaleFactors/2017electron.json");
        electronScaleFactorReader->addYearData("2017", electronScaleFactor2017);
        auto electronScaleFactor2018 = make_shared<ElectronJSONReader>("ScaleFactors/2018electron.json");
        electronScaleFactorReader->addYearData("2018", electronScaleFactor2018);    

    auto recoScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonRecoScaleFactor", recoScaleFactorReader, ParticleType::muon());
    auto iDISOScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonIDISOScaleFactor", iDISOScaleFactorReader, ParticleType::muon());
    auto triggerScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonTriggerScaleFactor", triggerScaleFactorReader, ParticleType::muon());
    auto electronScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("ElectronScaleFactor", electronScaleFactorReader, ParticleType::electron());

  
	eventMod->addScaleFactor(recoScaleFactor);
    //std::cout << "Added scale factor" << std::endl;
    eventMod->addScaleFactor(iDISOScaleFactor);
    //std::cout << "Added IDISO scale factor" << std::endl;
    eventMod->addScaleFactor(triggerScaleFactor);
    //std::cout << "Added trigger scale factor" << std::endl;
    eventMod->addScaleFactor(electronScaleFactor);
    //std::cout << "Added electron scale factor" << std::endl;
    //eventMod->addScaleFactor(pileUpScaleFactor);
}