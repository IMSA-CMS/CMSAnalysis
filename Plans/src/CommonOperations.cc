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
//#include "CMSAnalysis/Modules/interface/MultiYearScaleFactorReader.hh"
#include "CMSAnalysis/Modules/interface/LeptonScaleFactor.hh"




using std::make_shared;

void CommonOperations::addHiggsScaleFactors(std::shared_ptr<EventModule> eventMod)
{
   // auto pileUpScaleFactor = make_shared<MuonScaleFactor>("","");
   // auto recoScaleFactor = make_shared<MultiYearScaleFactorReader>();
    
   /*
    // Muon Reco
    auto muonScaleFactorReader2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_schemaV2.json");
    auto muonScaleFactor2016 = make_shared<LeptonScaleFactor>("MuonRecoScaleFactor2016", muonScaleFactorReader2016, ParticleType::muon());
    //recoScaleFactorReader->addYearData("2016", muonScaleFactor2016);
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

    auto recoScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonRecoScaleFactor");
    recoScaleFactor->addScaleFactor("2016", muonScaleFactor2016);
    auto iDISOScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonIDISOScaleFactor", iDISOScaleFactorReader, ParticleType::muon());
    auto triggerScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonTriggerScaleFactor", triggerScaleFactorReader, ParticleType::muon());
    auto electronScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("ElectronScaleFactor", electronScaleFactorReader, ParticleType::electron());
    */
   // Muon Reco
//auto recoScaleFactorReader = make_shared<MultiYearScaleFactorReader>();
    auto muonScaleFactor = make_shared<MultiYearScaleFactor>("MuonRecoScaleFactor");
    auto muonScaleFactorReader2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_schemaV2.json");
    auto muonScaleFactor2016 = make_shared<LeptonScaleFactor>("MuonRecoScaleFactor2016", muonScaleFactorReader2016, ParticleType::muon());
    muonScaleFactor->addScaleFactor("2016", muonScaleFactor2016);
    auto muonScaleFactorReader2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2017_schemaV2.json");
    auto muonScaleFactor2017 = make_shared<LeptonScaleFactor>("MuonRecoScaleFactor2017", muonScaleFactorReader2017, ParticleType::muon());
    muonScaleFactor->addScaleFactor("2017", muonScaleFactor2017);
    auto muonScaleFactorReader2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2018_schemaV2.json");
    auto muonScaleFactor2018 = make_shared<LeptonScaleFactor>("MuonRecoScaleFactor2018", muonScaleFactorReader2018, ParticleType::muon());
    muonScaleFactor->addScaleFactor("2018", muonScaleFactor2018);
    auto muonScaleFactorReader2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_preVFP_schemaV2.json");
    auto muonScaleFactor2016APV = make_shared<LeptonScaleFactor>("MuonRecoScaleFactor2016APV", muonScaleFactorReader2016APV, ParticleType::muon());
    muonScaleFactor->addScaleFactor("2016APV", muonScaleFactor2016APV);

    // Trigger 
    auto triggerScaleFactorReader = make_shared<MultiYearScaleFactor>("MuonTriggerScaleFactor");
    auto muonTriggerScaleFactorReader2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_preVFP_schemaV2.json");
    auto muonTriggerScaleFactor2016APV = make_shared<LeptonScaleFactor>("MuonTriggerScaleFactor2016APV", muonTriggerScaleFactorReader2016APV, ParticleType::muon());
    triggerScaleFactorReader->addScaleFactor("2016APV", muonTriggerScaleFactor2016APV);
    auto muonTriggerScaleFactorReader2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_schemaV2.json");
    auto muonTriggerScaleFactor2016 = make_shared<LeptonScaleFactor>("MuonTriggerScaleFactor2016", muonTriggerScaleFactorReader2016, ParticleType::muon());
    triggerScaleFactorReader->addScaleFactor("2016", muonTriggerScaleFactor2016);
    auto muonTriggerScaleFactorReader2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2017_schemaV2.json");
    auto muonTriggerScaleFactor2017 = make_shared<LeptonScaleFactor>("MuonTriggerScaleFactor2017", muonTriggerScaleFactorReader2017, ParticleType::muon());
    triggerScaleFactorReader->addScaleFactor("2017", muonTriggerScaleFactor2017);
    auto muonTriggerScaleFactorReader2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2018_schemaV2.json");
    auto muonTriggerScaleFactor2018 = make_shared<LeptonScaleFactor>("MuonTriggerScaleFactor2018", muonTriggerScaleFactorReader2018, ParticleType::muon());
    triggerScaleFactorReader->addScaleFactor("2018", muonTriggerScaleFactor2018);

    // IDISO
    auto iDISOScaleFactorReader = make_shared<MultiYearScaleFactor>("MuonIDISOScaleFactor");
    auto muonIDISOScaleFactorReader2016APV = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_preVFP_schemaV2.json");
    auto muonIDISOScaleFactor2016APV = make_shared<LeptonScaleFactor>("MuonIDISOScaleFactor2016APV", muonIDISOScaleFactorReader2016APV, ParticleType::muon());
    iDISOScaleFactorReader->addScaleFactor("2016APV", muonIDISOScaleFactor2016APV);
    auto muonIDISOScaleFactorReader2016 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_schemaV2.json");
    auto muonIDISOScaleFactor2016 = make_shared<LeptonScaleFactor>("MuonIDISOScaleFactor2016", muonIDISOScaleFactorReader2016, ParticleType::muon());
    iDISOScaleFactorReader->addScaleFactor("2016", muonIDISOScaleFactor2016);
    auto muonIDISOScaleFactorReader2017 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2017_schemaV2.json");
    auto muonIDISOScaleFactor2017 = make_shared<LeptonScaleFactor>("MuonIDISOScaleFactor2017", muonIDISOScaleFactorReader2017, ParticleType::muon());
    iDISOScaleFactorReader->addScaleFactor("2017", muonIDISOScaleFactor2017);
    auto muonIDISOScaleFactorReader2018 = make_shared<MuonJSONReader>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2018_schemaV2.json");
    auto muonIDISOScaleFactor2018 = make_shared<LeptonScaleFactor>("MuonIDISOScaleFactor2018", muonIDISOScaleFactorReader2018, ParticleType::muon());
    iDISOScaleFactorReader->addScaleFactor("2018", muonIDISOScaleFactor2018);

    // Electron
    auto electronScaleFactorReader = make_shared<MultiYearScaleFactor>("ElectronScaleFactor");
    auto electronScaleFactorReader2016APV = make_shared<ElectronJSONReader>("ScaleFactors/2016APVelectron.json");
    auto electronScaleFactor2016APV = make_shared<LeptonScaleFactor>("ElectronScaleFactor2016APV", electronScaleFactorReader2016APV, ParticleType::electron());
    electronScaleFactorReader->addScaleFactor("2016APV", electronScaleFactor2016APV);
    auto electronScaleFactorReader2016 = make_shared<ElectronJSONReader>("ScaleFactors/2016electron.json");
    auto electronScaleFactor2016 = make_shared<LeptonScaleFactor>("ElectronScaleFactor2016", electronScaleFactorReader2016, ParticleType::electron());
    electronScaleFactorReader->addScaleFactor("2016", electronScaleFactor2016);
    auto electronScaleFactorReader2017 = make_shared<ElectronJSONReader>("ScaleFactors/2017electron.json");
    auto electronScaleFactor2017 = make_shared<LeptonScaleFactor>("ElectronScaleFactor2017", electronScaleFactorReader2017, ParticleType::electron());
    electronScaleFactorReader->addScaleFactor("2017", electronScaleFactor2017);
    auto electronScaleFactorReader2018 = make_shared<ElectronJSONReader>("ScaleFactors/2018electron.json");
    auto electronScaleFactor2018 = make_shared<LeptonScaleFactor>("ElectronScaleFactor2018", electronScaleFactorReader2018, ParticleType::electron());
    electronScaleFactorReader->addScaleFactor("2018", electronScaleFactor2018);

    // auto recoScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonRecoScaleFactor", muonScaleFactor, ParticleType::muon());
    // auto iDISOScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonIDISOScaleFactor", iDISOScaleFactorReader, ParticleType::muon());
    // auto triggerScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("MuonTriggerScaleFactor", triggerScaleFactorReader, ParticleType::muon());
    // auto electronScaleFactor = make_shared<MultiYearScaleFactor<LeptonScaleFactor>>("ElectronScaleFactor", electronScaleFactorReader, ParticleType::electron());

    // auto recoScaleFactor = make_shared<MultiYearScaleFactor>("MuonRecoScaleFactor");
    // auto iDISOScaleFactor = make_shared<MultiYearScaleFactor>("MuonIDISOScaleFactor");
    // auto triggerScaleFactor = make_shared<MultiYearScaleFactor>("MuonTriggerScaleFactor");
    // auto electronScaleFactor = make_shared<MultiYearScaleFactor>("ElectronScaleFactor");

    eventMod->addScaleFactor(muonScaleFactor);
    //std::cout << "Added scale factor" << std::endl;
    eventMod->addScaleFactor(iDISOScaleFactorReader);
    //std::cout << "Added IDISO scale factor" << std::endl;
    eventMod->addScaleFactor(triggerScaleFactorReader);
    //std::cout << "Added trigger scale factor" << std::endl;
    eventMod->addScaleFactor(electronScaleFactorReader);
    //std::cout << "Added electron scale factor" << std::endl;
    //eventMod->addScaleFactor(pileUpScaleFactor);
}