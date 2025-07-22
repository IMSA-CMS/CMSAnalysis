#include "CMSAnalysis/Plans/interface/CommonOperations.hh"
#include "CMSAnalysis/Modules/interface/MultiYearScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/ElectronScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/MuonScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/DummyScaleFactor.hh"
#include "CMSAnalysis/Modules/interface/EventModule.hh"
using std::make_shared;

void CommonOperations::addHiggsScaleFactors(std::shared_ptr<EventModule> eventMod)
{
	auto scaleFactor = make_shared<MultiYearScaleFactor>(std::string("RecoHiggsScaleFactor"));
    auto iDISOScaleFactor = make_shared<MultiYearScaleFactor>(std::string("HiggsIDISOScaleFactor"));
    auto triggerScaleFactor = make_shared<MultiYearScaleFactor>(std::string("HiggsTriggerScaleFactor"));
    //std::cout << "Adding scale factors" << std::endl;
    
    scaleFactor->addMuonScaleFactor("2018", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2018_schemaV2.json"));
    //std::cout << "Added 2018 muon scale factor" << std::endl;
    scaleFactor->addMuonScaleFactor("2017", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2017_schemaV2.json"));
    //std::cout << "Added 2017 muon scale factor" << std::endl;
    scaleFactor->addMuonScaleFactor("2016", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_schemaV2.json"));
    //std::cout << "Added 2016 muon scale factor" << std::endl;
    scaleFactor->addMuonScaleFactor("2016APV", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_RECO_2016_preVFP_schemaV2.json"));
    //std::cout << "Added 2016APV muon scale factor" << std::endl;
    triggerScaleFactor->addMuonScaleFactor("2016APV", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_preVFP_schemaV2.json"));
    //std::cout << "Added 2016APV muon trigger scale factor" << std::endl;
    triggerScaleFactor->addMuonScaleFactor("2016", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2016_schemaV2.json"));
    //std::cout << "Added 2016 muon trigger scale factor" << std::endl;
    triggerScaleFactor->addMuonScaleFactor("2018", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2018_schemaV2.json"));
    //std::cout << "Added 2018 muon trigger scale factor" << std::endl;
    triggerScaleFactor->addMuonScaleFactor("2017", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_HLT_2017_schemaV2.json"));
    //std::cout << "Added 2017 muon trigger scale factor" << std::endl;
    iDISOScaleFactor->addMuonScaleFactor("2016APV", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_preVFP_schemaV2.json"));
    //std::cout << "Added 2016APV muon IDISO scale factor" << std::endl;
    iDISOScaleFactor->addMuonScaleFactor("2016", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2016_schemaV2.json"));
    //std::cout << "Added 2016 muon IDISO scale factor" << std::endl;
    iDISOScaleFactor->addMuonScaleFactor("2018", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2018_schemaV2.json"));
    //std::cout << "Added 2018 muon IDISO scale factor" << std::endl;
    iDISOScaleFactor->addMuonScaleFactor("2017", make_shared<MuonScaleFactor>("ScaleFactors/ScaleFactors_Muon_highPt_IDISO_2017_schemaV2.json"));
   // std::cout << "Added 2017 muon IDISO scale factor" << std::endl;
    scaleFactor->addElectronScaleFactor("2018", make_shared<ElectronScaleFactor>("ScaleFactors/2018electron.json"));
    //std::cout << "Added 2018 electron scale factor" << std::endl;
    scaleFactor->addElectronScaleFactor("2017", make_shared<ElectronScaleFactor>("ScaleFactors/2017electron.json"));
    //std::cout << "Added 2017 electron scale factor" << std::endl;
    scaleFactor->addElectronScaleFactor("2016", make_shared<ElectronScaleFactor>("ScaleFactors/2016electron.json"));
    //std::cout << "Added 2016 electron scale factor" << std::endl;
    scaleFactor->addElectronScaleFactor("2016APV", make_shared<ElectronScaleFactor>("ScaleFactors/2016APVelectron.json"));
    //std::cout << "Added 2016APV electron scale factor" << std::endl;
    triggerScaleFactor->addElectronScaleFactor("2016APV", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2016APV electron trigger scale factor" << std::endl;
    triggerScaleFactor->addElectronScaleFactor("2016", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2016 electron trigger scale factor" << std::endl;
    triggerScaleFactor->addElectronScaleFactor("2017", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2017 electron trigger scale factor" << std::endl;
    triggerScaleFactor->addElectronScaleFactor("2018", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2018 electron trigger scale factor" << std::endl;
    iDISOScaleFactor->addElectronScaleFactor("2016APV", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2016APV electron IDISO scale factor" << std::endl;
    iDISOScaleFactor->addElectronScaleFactor("2016", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2016 electron IDISO scale factor" << std::endl;
    iDISOScaleFactor->addElectronScaleFactor("2017", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2017 electron IDISO scale factor" << std::endl;
    iDISOScaleFactor->addElectronScaleFactor("2018", make_shared<DummyScaleFactor>());
    //std::cout << "Added 2018 electron IDISO scale factor" << std::endl;

	eventMod->addScaleFactor(scaleFactor);
    //std::cout << "Added scale factor" << std::endl;
    eventMod->addScaleFactor(iDISOScaleFactor);
    //std::cout << "Added IDISO scale factor" << std::endl;
    eventMod->addScaleFactor(triggerScaleFactor);
    //std::cout << "Added trigger scale factor" << std::endl;
}