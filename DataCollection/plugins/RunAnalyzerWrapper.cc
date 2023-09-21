// -*- C++ -*-
//
// Package:    CMSAnalysis/RunAnalyzerWrapper
// Class:      RunAnalyzerWrapper
//
/**\class RunAnalyzerWrapper RunAnalyzerWrapper.cc CMSAnalysis/RunAnalyzerWrapper/plugins/RunAnalyzerWrapper.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Kevin Huang
//         Created:  Thu, 29 Sep 2022 18:50:08 GMT
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "../../DataCollection/interface/Analyzer.hh"
#include "../../DataCollection/interface/CmsswEventInterface.hh"
#include "../../DataCollection/interface/AnalyzerOptions.hh"
#include "../../Plans/interface/DataCollectionPlan.hh"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "DataFormats/PatCandidates/interface/PATObject.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;

class RunAnalyzerWrapper : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit RunAnalyzerWrapper(const edm::ParameterSet&);
  ~RunAnalyzerWrapper() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:

  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // ----------member data ---------------------------
  
  std::string analyzerType_;  //used to select what tracks to read from configuration file
  std::string rootOutFile_;
  Analyzer* analyzer = nullptr; 
  //ModuleCollection modules;
  edm::EDGetTokenT<pat::ElectronCollection> ElectronToken_; 
  edm::EDGetTokenT<pat::MuonCollection> MuonToken_;
  edm::EDGetTokenT<pat::PhotonCollection> PhotonToken_;
  edm::EDGetTokenT<pat::JetCollection> JetToken_;
  edm::EDGetTokenT<pat::METCollection> METToken_;
  edm::EDGetTokenT<pat::TriggerEvent> TriggerToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> GenSimToken_;


  CmsswEventInterface eventInterface;

  

// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//   edm::ESGetToken<SetupData, SetupRecord> setupToken_;
// #endif
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
RunAnalyzerWrapper::RunAnalyzerWrapper(const edm::ParameterSet& iConfig)
    : analyzerType_(iConfig.getUntrackedParameter<std::string>("analyzerType")),
     rootOutFile_(iConfig.getUntrackedParameter<std::string>("rootOutFile")),
     ElectronToken_(consumes<pat::ElectronCollection>(edm::InputTag("slimmedElectrons", ""))),
     MuonToken_(consumes<pat::MuonCollection>(edm::InputTag("slimmedMuons", ""))),
     PhotonToken_(consumes<pat::PhotonCollection>(edm::InputTag("slimmedPhotons", ""))),
     JetToken_(consumes<pat::JetCollection>(edm::InputTag("slimmedJets", ""))),
     METToken_(consumes<pat::METCollection>(edm::InputTag("slimmedMETs", ""))),
	 TriggerToken_(consumes<pat::TriggerEvent>(edm::InputTag("triggerEvent"))),
	 GenSimToken_(consumes<reco::GenParticleCollection>(edm::InputTag("GenericParticle"))),
     eventInterface(ElectronToken_,MuonToken_,PhotonToken_,JetToken_,METToken_,TriggerToken_,GenSimToken_)
{ 
  std::string particleDatabase("../../DataCollection/bin/textfiles/ParticleData.txt");
  if (ParticleType::loadParticleDatabase(particleDatabase))
  {
    std::cout << particleDatabase << " has been loaded properly!\n";
  }
  else
  {
    std::cout << particleDatabase << " has not been loaded properly!\n";
  }

  AnalyzerOptions options;
  std::string analysis = options.checkSelectedAnalysis(analyzerType_);
  DataCollectionPlan *plan = options.getAnalysisPlans().at(analysis);
  plan->initialize();
  
  auto modulizer = plan->getAnalyzer();
  analyzer->addModules(modulizer);
}

RunAnalyzerWrapper::~RunAnalyzerWrapper() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void RunAnalyzerWrapper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  eventInterface.setEvent(&iEvent);
  analyzer->processOneEvent(&eventInterface); 
}

// ------------ method called once each job just before starting event loop  ------------
void RunAnalyzerWrapper::beginJob() {
  analyzer->initialize();

}

// ------------ method called once each job just after ending the event loop  ------------
void RunAnalyzerWrapper::endJob() {
  // please remove this method if not needed
  analyzer->writeOutputFile(rootOutFile_);
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void RunAnalyzerWrapper::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RunAnalyzerWrapper);
