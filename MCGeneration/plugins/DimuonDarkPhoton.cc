// -*- C++ -*-
//
// Package:    GenStudy/Dimuon
// Class:      Dimuon
// 
/**\class Dimuon Dimuon.cc GenStudy/Dimuon/plugins/Dimuon.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Shawn Gregory Zaleski
//         Created:  Tue, 30 Jun 2015 14:01:36 GMT
// Edited by Ming Huang to look at lepton jets originating from dark photons
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "/cvmfs/cms.cern.ch/slc7_amd64_gcc820/cms/cmssw/CMSSW_10_6_4/src/FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include <FWCore/ServiceRegistry/interface/Service.h>
#include <CommonTools/UtilAlgos/interface/TFileService.h>
#include <TTree.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TH2F.h>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include <vector>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class DimuonHiggsPlusPlus : public edm::EDAnalyzer {
public:
  explicit DimuonHiggsPlusPlus(const edm::ParameterSet&);
  ~DimuonHiggsPlusPlus();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  const reco::Candidate* getDaughter(const reco::Candidate* part,int pid);
  const reco::Candidate* getLastDaughter(const reco::Candidate* part,int pid);
  const reco::Candidate* getBoson( const reco::GenParticleCollection& genParts);
  const reco::Candidate* getMother(const reco::Candidate* part, int pid);
  //const reco::Candidate* getDYBoson(const reco::Candidate* part int pid)
  bool isBoson(int pid);
  bool isMuon(int pid);
  bool checkBosonStatus(const reco::GenParticleCollection& genParts);


  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  virtual void endRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  int leptonJetReco(std::vector<const reco::Candidate*> eles);
  
  int leptonJetRecoNewTrigger(std::vector<const reco::Candidate*> leptons);
  const reco::Candidate* findBiggestPT(std::vector<const reco::Candidate*> leptons, std::vector<const reco::Candidate*> &smallerPTLeptons);

  const void printGenParticleCollection(const reco::GenParticleCollection& genParts);
  const int getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts);

  struct P4Struct {
    float energy,et,eta,phi,pt,mass,theta;
    void fill(const math::XYZTLorentzVector& p4){
      if(p4.Pt()!=0 && p4.Et()!=0){
	energy = p4.E();
	et = p4.Et();
	eta = p4.Eta();
	phi = p4.Phi();
	pt =p4.Pt();
	mass = p4.mag();
	theta = p4.Theta();
      }else clear();
    }
    void clear(){energy=-999;et=-999;eta=-999;phi=-0;pt=-999;mass=-999;theta=-999;}
    static std::string contents(){return "energy/F:et:eta:phi:pt:mass:theta";}
  };

  TTree* tree_;

  // Neutralino and dark photon histograms
  TH1F *h_neutralinoNum, *h_chiR;
  TH1F *h_darkPhotonNum, *h_darkPhotonPT, *h_darkPhotonEta;

  // Electron analysis histograms
  TH1F *h_lepFromGammavNum, *h_lepFromGammavPT;
  TH1F *h_lepPhi, *h_lepEta, *h_lepR, *h_lepInvariantMass;
  TH1F *h_lepDeltaPhi, *h_lepDeltaEta;
  TH1F *h_lepDeltaXY;

  // High pT electron histograms
  TH1F *h_30GevOrMoreLeptons;
  TH1F *h_lepBiggestPT, *h_lepBiggestPTEta, *h_lepBiggestPTPhi;
  TH1F *h_lepSecondBiggestPT, *h_lepSecondBiggestPTEta, *h_lepSecondBiggestPTPhi;
  TH1F *h_lepThirdBiggestPT, *h_lepThirdBiggestPTEta, *h_lepThirdBiggestPTPhi;
  TH1F *h_lepBiggestPTPasses, *h_lepSecondBiggestPTPasses, *h_lepThirdBiggestPTPasses;
  TH1F *h_triggerLeptonsMinDeltaR;    

  // Electron set analysis histograms
  TH1F *h_lepSetPT, *h_lepSetE, *h_lepSetSigmaPhi, *h_lepSetSigmaEta;

  // Reconstruction histograms
  TH1F *h_recoLeptonJetNum, *h_actualJetNum, *h_jetNumDiff;
  TH1F *h_recoLeptonNumPerEvent, *h_recoLeptonNumInOneJetEvents;
  TH1F *h_notInJetLepton;
  TH1F *h_noJetNeutralinoNum;
  TH1F *h_acceptedEventsNum;
  TH1F *h_recoLeptonJetCounter, *h_actualLeptonJetCounter;
  TH1F *h_triggerMuonDeltaR;

  // Four vector comparison histograms
  TH1F *h_chiPt, *h_lepJetPt, *h_chiLepPtDiff;
  TH1F *h_chiPhi, *h_lepJetPhi, *h_chiLepPhiDiff;
  TH1F *h_chiEta, *h_lepJetEta, *h_chiLepEtaDiff;
  TH1F *h_chiMass, *h_lepJetMass, *h_chiLepMassDiff;

  P4Struct bosonP4_; // as a sanity check we have the right event...
  P4Struct muMinusP4_;
  P4Struct muPlusP4_;
  int muMinusPID_;
  int muPlusPID_;
  int bosonId_;
  double crossSec, cosTheta, tanPhi, csTheta, csPhi;
  double mCosThetaPlus, mCosThetaMinus;

  int countXY;
  int totalXY;

  int debug_;
  edm::InputTag genPartsTag_;
  int decayParticlePID_;
  edm::InputTag genInfoProduct_;
  edm::EDGetTokenT<GenRunInfoProduct> genInfoProductToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> genPartsToken_;

  // ----------member data ---------------------------
  
};


void DimuonHiggsPlusPlus::beginJob()
{
  edm::Service<TFileService> fs;
  // Neutralino histograms
  h_neutralinoNum = fs->make<TH1F>("neutralinoNum", "Number of neutralinos per event", 14, 0, 14);
  h_chiR = fs->make<TH1F>("chiR", "delta r of the two neutralinos in the event", 100, 0, 10);

  // Dark photon histograms
  h_darkPhotonNum = fs->make<TH1F>("darkPhotonNum", "Amount of dark photons per event", 15, -.5, 14.5);
  h_darkPhotonPT = fs->make<TH1F>("darkPhotonPT", "Dark Photon PT", 100, 0, 400);
  h_darkPhotonEta = fs->make<TH1F>("darkPhotonEta", "Dark Photon Eta", 100, -5, 5);

  // Electrons that came from a dark photon histograms
  h_lepFromGammavNum = fs->make<TH1F>("lepFromGammavNum", "Number of leptons from dark photons in each event", 14, 0, 14);
  h_lepFromGammavPT = fs->make<TH1F>("lepFromGammavPT", "PT of leptons from dark photons", 100, 0, 100);

  // Electron histograms
  h_lepPhi = fs->make<TH1F>("lepPhi", "Phi of leptons", 100, -3.2, 3.2);
  h_lepEta = fs->make<TH1F>("lepEta", "Eta of leptons", 100, -10, 10);
  h_lepInvariantMass = fs->make<TH1F>("lepInvariantMass", "Invariant mass of leptons", 100, -0.5, 1.5);
  h_lepDeltaPhi = fs->make<TH1F>("lepDeltaPhi", "difference in phi between two leptons that came from the same dark photon", 100, -10, 10);
  h_lepDeltaEta = fs->make<TH1F>("lepDeltaEta", "delta eta of leptons from the same dark photon", 100, -10, 10);
  h_lepDeltaXY = fs->make<TH1F>("lepDeltaXY", "distance from origin of leptons", 1000, 0, 10000);
  h_lepR = fs->make<TH1F>("lepR", "Distance between a pair of two leptons that originated from the same dark photon in phi-eta space", 300, -0.1, 2.9);

  // High pT electron histograms
  h_30GevOrMoreLeptons = fs->make<TH1F>("30GevOrMoreLeptons", "The number of events that have a lepton 30 gev or greater", 2, 0, 2);
  h_lepBiggestPT = fs->make<TH1F>("lepBiggestPT", "PT of the lepton with the biggest PT in its event", 20, 0, 100);
  h_lepBiggestPTEta = fs->make<TH1F>("lepBiggestPTEta", "Eta of lepton with biggest PT", 100, -10, 10);
  h_lepBiggestPTPhi = fs->make<TH1F>("lepBiggestPTPhi", "Phi of lepton with biggest PT", 100, -3.2, 3.2);

  h_lepSecondBiggestPT = fs->make<TH1F>("lepSecondBiggestPT", "PT of the lepton with the second biggest PT in its event", 20, 0, 100);
  h_lepSecondBiggestPTEta = fs->make<TH1F>("lepSecondBiggestPTEta", "Eta of lepton with second biggest PT", 100, -10, 10);
  h_lepSecondBiggestPTPhi = fs->make<TH1F>("lepSecondBiggestPTPhi", "Phi of lepton with second biggest PT", 100, -3.2, 3.2);
  
  h_lepThirdBiggestPT = fs->make<TH1F>("lepThirdBiggestPT", "PT of the lepton with the third biggest PT in its event", 20, 0, 100);
  h_lepThirdBiggestPTEta = fs->make<TH1F>("lepThirdBiggestPTEta", "Eta of lepton with third biggest PT", 100, -10, 10);
  h_lepThirdBiggestPTPhi = fs->make<TH1F>("lepThirdBiggestPTPhi", "Phi of lepton with third biggest PT", 100, -3.2, 3.2);

  
  h_lepBiggestPTPasses = fs->make<TH1F>("h_lepBiggestPTPasses", "Fraction of biggest pT leptons of which, it's event passes the trigger for each pT", 20, 0, 100);
  h_lepSecondBiggestPTPasses = fs->make<TH1F>("h_lepSecondBiggestPTPasses", "Fraction of second biggest pT leptons of which, it's event passes the trigger for each pT", 20, 0, 100);
  h_lepThirdBiggestPTPasses = fs->make<TH1F>("h_lepThirdBiggestPTPasses", "Fraction of third biggest pT leptons of which, it's event passes the trigger for each pT", 20, 0, 100);

  h_triggerLeptonsMinDeltaR = fs->make<TH1F>("h_triggerLeptonsMinDeltaR", "delta r between the two trigger leptons that have the smallest delta r", 100, 0, .1);

  // Set of electrons that came from the same neutralino histograms
  h_lepSetPT = fs->make<TH1F>("lepSetPT", "The total PT of a set of all the leptons that came from the same neutralino", 100, 0, 400);
  h_lepSetE = fs->make<TH1F>("lepSetE", "The total energy of a set of all the leptons that came from the same neutralino", 100, 0, 400);
  h_lepSetSigmaPhi = fs->make<TH1F>("lepSetSigmaPhi", "The standard deviation of the phis in a set of all the leptons that came from the same neutralino", 100, -0.1, 3.2);
  h_lepSetSigmaEta = fs->make<TH1F>("lepSetSigmaEta", "The standard deviation of the etas in a set of all the leptons that came from the same neutralino", 150, -.5, 2.5);

  // Reconstruction and reconstruction checking histograms
  h_recoLeptonJetNum = fs->make<TH1F>("recoLeptonJetNum", "The number of lepton jets that there appear to be based off of only observing electrons and their angle", 15, -0.5, 14.5);
  h_actualJetNum = fs->make<TH1F>("actualJetNum", "The actual number of lepton jets in the event, based on whether neutrinos produced electrons", 15, -0.5, 14.5);
  h_jetNumDiff = fs->make<TH1F>("jetNumDiff", "number of lepton jets reconstructed minus how many lepton jets there actually were", 11, -5.5, 5.5);
  h_recoLeptonNumPerEvent = fs->make<TH1F>("recoLeptonNumPerEvent", "The number of leptons being reconstructed in an event", 15, 0, 15);
  h_recoLeptonNumInOneJetEvents = fs->make<TH1F>("recoLeptonNumInOneJetEvents", "The number of leptons in events where one jet is reconstructed", 15, 0, 15);
  h_notInJetLepton = fs->make<TH1F>("notInJetLepton", "The number of leptons that didn't get put into a jet in each event", 15, 0, 15);
  h_noJetNeutralinoNum = fs->make<TH1F>("noJetNeutralinoNum", "Each entry is an event with a neutralino producing 4900002, which should have no decays", 0, 0, 1);
  h_acceptedEventsNum = fs->make<TH1F>("acceptedEventsNum", "Each entry is an event which was accepted, that is, a algorithm identifies it as a lepton jet", 0, 0, 1);
  h_recoLeptonJetCounter = fs->make<TH1F>("recoLeptonJetCounter", "Each entry represents one lepton jet that was reconstructed", 0, 0, 1);
  h_actualLeptonJetCounter = fs->make<TH1F>("actualLeptonJetCounter", "Each entry represents one lepton jet that there actually is", 0, 0, 1);

  // Four vector comparison histograms
  h_chiPt = fs->make<TH1F>("chiPt", "The pt of all neutralinos in the event", 100, -100, 400);
  h_lepJetPt = fs->make<TH1F>("lepJetPt", "The pt of all lepton jets in the event", 100, -100, 400);
  h_chiLepPtDiff = fs->make<TH1F>("chiLepPtDiff", "The differnce in pt between all pairs of lepton jets and their neutralinos  in the event", 100, -100, 400);

  h_chiPhi = fs->make<TH1F>("chiPhi", "The phi of all neutralinos in the event", 100, -3.2, 3.2);
  h_lepJetPhi = fs->make<TH1F>("lepJetPhi", "The phi of all lepton jets in the event", 100, -3.2, 3.2);
  h_chiLepPhiDiff = fs->make<TH1F>("chiLepPhiDiff", "The differnce in phi between all pairs of lepton jets and their neutralinos  in the event", 100, -3.2, 3.2);

  h_chiEta = fs->make<TH1F>("chiEta", "The eta of all neutralinos in the event", 100, -10, 10);
  h_lepJetEta = fs->make<TH1F>("lepJetEta", "The eta of all lepton jets in the event", 100, -10, 10);
  h_chiLepEtaDiff = fs->make<TH1F>("chiLepEtaDiff", "The differnce in eta between all pairs of lepton jets and their neutralinos  in the event", 100, -10, 10);

  h_chiMass = fs->make<TH1F>("chiMass", "The mass of all neutralinos in the event", 100, -100, 400);
  h_lepJetMass = fs->make<TH1F>("lepJetMass", "The mass of all lepton jets in the event", 100, -100, 400);
  h_chiLepMassDiff = fs->make<TH1F>("chiLepMassDiff", "The differnce in mass between all pairs of lepton jets and their neutralinos  in the event", 100, -100, 400);

  countXY = 0;
  totalXY = 0;

  tree_= fs->make<TTree>("pdfTree","PDF Tree");
  // tree_->Branch("evtId",&evtId_,EventId::contents().c_str());
  tree_->Branch("bosonP4",&bosonP4_,P4Struct::contents().c_str());
  tree_->Branch("decay1P4",&muMinusP4_,P4Struct::contents().c_str());
  tree_->Branch("decay2P4",&muPlusP4_,P4Struct::contents().c_str());
  tree_->Branch("decay1PID",&muMinusPID_,"decay1PID/I");
  tree_->Branch("decay2PID",&muPlusPID_,"decay2PID/I");
  tree_->Branch("bosonPID",&bosonId_,"bosonPID/I");
  tree_->Branch("crossSec", &crossSec, "crossSec/D");
  tree_->Branch("cosTheta", &cosTheta, "cosTheta/D");
  tree_->Branch("tanPhi", &tanPhi, "tanPhi/D");
  tree_->Branch("csTheta", &csTheta, "csTheta/D");
  tree_->Branch("csPhi", &csPhi, "csPhi/D");
  tree_->Branch("mCosThetaPlus", &mCosThetaPlus, "mCosThetaPlus/D");
  tree_->Branch("mCosThetaMinus", &mCosThetaMinus, "mCosThetaMinus/D");
  // tree_->Branch("pdfInfo",&pdfInfo_,PDFInfo::contents().c_str());
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
DimuonHiggsPlusPlus::DimuonHiggsPlusPlus(const edm::ParameterSet& iConfig)

{


  debug_=iConfig.getParameter<int>("debug");
  genPartsTag_=iConfig.getParameter<edm::InputTag>("genPartsTag");
  decayParticlePID_ = iConfig.getParameter<int>("decayParticlePID");
  genInfoProduct_ = iConfig.getParameter<edm::InputTag>("genInfoProduct");
  
  //now do what ever initialization is needed

  genInfoProductToken_ = consumes<GenRunInfoProduct,edm::InRun>(genInfoProduct_);
  genPartsToken_ = consumes<reco::GenParticleCollection>(genPartsTag_);
}


DimuonHiggsPlusPlus::~DimuonHiggsPlusPlus()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DimuonHiggsPlusPlus::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  edm::Handle<reco::GenParticleCollection> genPartsHandle;
  iEvent.getByToken(genPartsToken_,genPartsHandle);
  const reco::GenParticleCollection& genParts = *genPartsHandle;

  bosonId_=0;
  bosonP4_.clear();
  muMinusP4_.clear();
  muPlusP4_.clear();
  muMinusPID_=0;
  muPlusPID_=0;

  math::XYZTLorentzVectorD dimuon;

  double numDarkPhotons = 0;
  double numNeutralinos = 0;

  std::vector<const reco::Candidate*> leptons;   // Analysis electron container
  std::vector<const reco::Candidate*> recoLeptons;  // Reconstruction electron container

  bool thirtyGevOrMoreLeptonExists = false;

  const reco::Candidate* chi1;   // Variable to place first neutralino while looking for the second
  const reco::Candidate* chi2;
  bool chi1Initialized = false;

  int actualJetNum = 0;

  // contains the pdg particle code to select which particle to analyze
  const int muEleP = 13; //positive
  //int muEleN = -13; //negative

  for(auto &part : genParts) // for every particle (part) in this event (genParts)
  {
    // Neutralino delta r calculations
    if(std::abs(part.pdgId()) == 1000022 && part.numberOfDaughters() > 1) // if particle (part) is a neutralino and particle (part) has more than one daughter 
    {
      if(chi1Initialized)      // if the first neutralino has been found and stored
      {
        ROOT::Math::LorentzVector d1 = chi1->p4();
	      chi2 = &part;
        ROOT::Math::LorentzVector d2 = chi2->p4();
	      h_chiR->Fill(reco::deltaR(d1, d2));
      }
      else // else save the first neutralino
      {
	      chi1 = &part;
	      chi1Initialized = true;
      }
    }

    // Counting events with electrons with a Gev >=30
    if(part.numberOfDaughters() == 0 && std::abs(part.pdgId()) == muEleP && part.pt() >= 30)
    {
      thirtyGevOrMoreLeptonExists = true;
    }
    
    // Filling reconstruction leptons container, must be lepton and be greater than a certain gev and less than a certain eta
    if(std::abs(part.pdgId()) == muEleP && part.numberOfDaughters() == 0 && part.pt() > 5 && std::abs(part.eta()) < 2.1)
    {
      recoLeptons.push_back(&part);
    }

    // lepton and dark photon analysis
    if(part.pdgId() == 4900022) // if particle (part) is a dark photon
    {
	    numDarkPhotons++;
      h_darkPhotonPT->Fill(part.pt()); // Enter pT into histogram 
      h_darkPhotonEta->Fill(part.eta());
      if(part.numberOfDaughters() == 2) // if particle (part) has two daughters
      {
	      numDarkPhotons++;
	      h_darkPhotonPT->Fill(part.pt()); // Enter pT into histogram
        h_darkPhotonEta->Fill(part.eta());
    
        const reco::Candidate* daughter1; // Container for final state electrons
	      const reco::Candidate* daughter2;

	      if(abs(part.daughter(0)->pdgId()) == muEleP && abs(part.daughter(1)->pdgId()) == muEleP) // if both daughters of the dark photon (part) are leptons
	      {

	        bool stillLooking = true; 
	        const reco::Candidate* checkNext; // Container for the next particle to be examined
	  
	        checkNext = part.daughter(0); // Set the first particle to be checked as first daughter of dark photon
	        while(stillLooking) // while the final state electron coming from the first daughter hasn't been found
	        {
	          if(checkNext->numberOfDaughters() == 0) // if the particle being checked has no daughters
	          {
	            daughter1 = checkNext; // store this particle as the final state of the first daughter
	            stillLooking = false;
	          }
	          else // else check the daughter of the currently being checked particle, next
	          {
	            checkNext = checkNext->daughter(0);
	          }
	        }

	        // Finding final state particle of the second daughter of the dark photon
	        stillLooking = true;
	        checkNext = part.daughter(1);
	        while(stillLooking)
	        {
	          if(checkNext->numberOfDaughters() == 0)
	          {
	            daughter2 = checkNext;
	            stillLooking = false;
	          }
	          else
	          {
	            checkNext = checkNext->daughter(0);
	          }
	        }

	        // Calculations involving the two final state daughters of the dark photon
	        h_lepFromGammavPT->Fill(daughter1->pt()); // Filling pT of leptons
	        h_lepFromGammavPT->Fill(daughter2->pt());
	  
	        leptons.push_back(daughter1); // Save the leptons for further analysis later
	        leptons.push_back(daughter2);
	  
	        h_lepPhi->Fill(daughter1->phi()); // Filling phi and eta for both leptons
	        h_lepEta->Fill(daughter1->eta());
	  
	        h_lepPhi->Fill(daughter2->phi());
	        h_lepEta->Fill(daughter2->eta());
	  
	        double invariantMass = sqrt(2 * daughter1->pt() * daughter2->pt() *( cosh(daughter1->eta() - daughter2->eta()) - cos(TVector2::Phi_mpi_pi(daughter1->phi() - daughter2->phi()))));
	        h_lepInvariantMass->Fill(invariantMass); // Filling invariant mass

	        double deltaEta = daughter2->eta()-daughter1->eta(); // Filling delta eta and delta r between the two leptons
	        double deltaPhi = reco::deltaPhi(daughter2->phi(), daughter1->phi());
          double deltaXY = (sqrt(daughter1->vx() * daughter1->vx() + daughter1->vy() * daughter1->vy())
              + sqrt(daughter2->vx() * daughter2->vx() + daughter2->vy() * daughter2->vy())) / 2;

          if(deltaXY <= 2) {
            countXY++;
          }
          totalXY++;
	  
	        h_lepDeltaPhi->Fill(deltaPhi);
	        h_lepDeltaEta->Fill(deltaEta);
          h_lepDeltaXY->Fill(deltaXY);

	        ROOT::Math::LorentzVector d1 = daughter1->p4(); // Filing delta r between the two leptons
	        ROOT::Math::LorentzVector d2 = daughter2->p4();
	        h_lepR->Fill(reco::deltaR(d1, d2));
	      }   
	      else
	      {
	        std::cout << "Dark photon daughters are not both leptons" << std::endl;
	      }
      }
      else
      {
	      std::cout << "Dark photon doesn't have 2 daughters" << std::endl;
      }
    }

    // starting from the neutralino, find out how many lepton jets were produced
    // also store all the leptons from this jet into a collection for analysis
    std::vector<const reco::Candidate*> notLepSet;
    std::vector<const reco::Candidate*> lepSet;
    if(abs(part.pdgId()) == 1000022) // if particle is a neutralino
    {
      if(part.numberOfDaughters() == 2 && part.daughter(0)->pdgId() != 1000022 && part.daughter(1)->pdgId() != 1000022) // if neutralino has two daughters which aren't neutralinos
      {
	      numNeutralinos++;

        // Checking whether this particle creates a jet
	      if(part.daughter(0)->pdgId() != 4900002 && part.daughter(1)->pdgId() != 4900002) // if neither daughter is 4900002 (a 4900002 decay from neutralino means no further particles will be produced)
	      {
	        actualJetNum++;
	      }
	      else if(part.daughter(0)->pdgId() == 4900002 || part.daughter(1)->pdgId() == 4900002) // if one of the neutralino's decays is 4900002
      	{
          h_noJetNeutralinoNum->Fill(0);
	        std::cout << "First daughter is " << part.daughter(0)->pdgId() << std::endl;
	        std::cout << "Second daughter is " << part.daughter(1)->pdgId() << std::endl;
	      }

        // Gathering electrons in this jet        
        notLepSet.push_back(part.daughter(0)); // start with the two daughters of the neutralino
        notLepSet.push_back(part.daughter(1));
        std::vector<const reco::Candidate*> tempNotLepSet; // temporary storage, so I don't edit vectors while I'm looping through them
        while(!notLepSet.empty()) // while notLepSet is not empty
        {
          for(auto &notLep : notLepSet) // for all (notLep) in notLepSet
          {
            if(notLep->numberOfDaughters() == 2) // if notLep has two daughters
            {
              if(abs(notLep->daughter(0)->pdgId()) == muEleP && abs(notLep->daughter(1)->pdgId()) == muEleP) // if both daughters of notLep are leptons
              {
                lepSet.push_back(notLep->daughter(0)); // put the leptons into their container
                lepSet.push_back(notLep->daughter(1));
              }
              else if(abs(notLep->daughter(0)->pdgId()) == muEleP || abs(notLep->daughter(1)->pdgId()) == muEleP)
              {
                std::cout << "Leptons were not pair produced, that is, only one daughter is an lepton" << std::endl;
              }
              else
              {
                tempNotLepSet.push_back(notLep->daughter(0)); // put the not leptons into their container
                tempNotLepSet.push_back(notLep->daughter(1));
              }
            }
            else if(notLep->numberOfDaughters() == 1) // if notLep has one daughter
            {
              tempNotLepSet.push_back(notLep->daughter(0)); // put it into its container (leptons should only come from pair production, and having one daughter means pair production isn't possible)
            }
            else if(notLep->numberOfDaughters() == 0) // if notLep has no daughter
            {
              continue; // it can't be an lepton, and there are no more particles after it, so we're done with this particle
            }
            else // if the above cases don't fit, let me know
            {
              std::cout << "While searching for leptons in a jet, a particle with more than 2 daughters was found." << std::endl;
            }
          }
          notLepSet.clear();

          for(auto &notLep : tempNotLepSet)
          {
            if(notLep->numberOfDaughters() == 2 && abs(notLep->daughter(0)->pdgId()) == muEleP && abs(notLep->daughter(1)->pdgId()) == muEleP)
            {
              if(abs(notLep->daughter(0)->pdgId()) == muEleP && abs(notLep->daughter(1)->pdgId()) == muEleP)
              {
                lepSet.push_back(notLep->daughter(0));
                lepSet.push_back(notLep->daughter(1));
              }
              else
              {
                notLepSet.push_back(notLep->daughter(0));
                notLepSet.push_back(notLep->daughter(1));
              }
            }
            else if(notLep->numberOfDaughters() == 1)
            {
              notLepSet.push_back(notLep->daughter(0));
            }
            else if(notLep->numberOfDaughters() == 0)
            {
              continue;
            }
            else
            {
              std::cout << "While searching for leptons in a jet, a particle with more than 2 daughters was found." << std::endl;
            }
          }
          tempNotLepSet.clear();
        }
      }
    }

    // Taking final state leptons from the set of leptons in the same jet
    std::vector<const reco::Candidate*> finalStateLepSet;
    for(auto &lep : lepSet) // for all the leptons in lepSet
    {
      const reco::Candidate* checkNext= lep;
      bool stillLooking = true;
      while(stillLooking) // if the lepton has daughters, go down the chain until I find the last one
      {
        if(checkNext->numberOfDaughters() == 0)
        {
          finalStateLepSet.push_back(checkNext);
          stillLooking = false;
        }
        else
        {
          bool radiatesLep = false;
          int numDaughters = checkNext->numberOfDaughters();
          for(int particleIndex = 0; particleIndex < numDaughters; particleIndex++)
          {
            if(abs(checkNext->daughter(particleIndex)->pdgId()) == muEleP)
            {
              checkNext = checkNext->daughter(particleIndex);
              radiatesLep = true;
              particleIndex = numDaughters;
            }
          }
          if(!radiatesLep)
          {
            std::cout << "Lepton has daughters of which none are leptons" << std::endl;
          }
        }
      }
    }

    // Finding total pt, total energy, of the set of leptons from the same jet   
    double lepSetPT = 0;
    double lepSetE = 0;
    for(auto &lep : finalStateLepSet)
    {
      lepSetPT += lep->pt();
      lepSetE += lep->energy();
    }
    if(lepSetPT != 0)
    {
      h_lepSetPT->Fill(lepSetPT);
      h_lepSetE->Fill(lepSetE);
    
      double lepSetSumPhi = 0;
      double lepSetSumEta = 0;
      // standard deviation of set of leptons
      for(auto &lep : finalStateLepSet) // sum the phi/eta
      {
	      lepSetSumPhi += lep->phi();
	      lepSetSumEta += lep->eta();
      }
      double lepSetMeanPhi = lepSetSumPhi/lepSet.size(); // get the mean of phi/eta
      double lepSetMeanEta = lepSetSumEta/lepSet.size();
      
      double phiMinusMeanSquaresSum = 0;
      double etaMinusMeanSquaresSum = 0;
      for(auto &lep:finalStateLepSet) // get the phi/eta minus their mean and then square that and then add it to a running total (sum)
      {
	      phiMinusMeanSquaresSum += (lep->phi() - lepSetMeanPhi) * (lep->phi() - lepSetMeanPhi);
	      etaMinusMeanSquaresSum += (lep->eta() - lepSetMeanEta) * (lep->eta() - lepSetMeanEta);
      }
      double phiMinusMeanSquaresMean = phiMinusMeanSquaresSum/lepSet.size(); // get the mean of the previous step
      double etaMinusMeanSquaresMean = etaMinusMeanSquaresSum/lepSet.size();
      
      h_lepSetSigmaPhi->Fill(std::sqrt(phiMinusMeanSquaresMean)); // get the standard deviation 
      h_lepSetSigmaEta->Fill(std::sqrt(etaMinusMeanSquaresMean));
    }
  }

  
  
  h_actualJetNum->Fill(actualJetNum);
  if(thirtyGevOrMoreLeptonExists)
  {
    h_30GevOrMoreLeptons->Fill(1);
  }
  if(numDarkPhotons != 0)
  {
    h_darkPhotonNum->Fill(numDarkPhotons); // fill the number of dark photons for this event
  }
  else
  {
    std::cout << "No dark photons in this event" << std::endl;
  }
  if(leptons.size() != 0)
  {
    h_lepFromGammavNum->Fill(leptons.size()); // fill the number of leptons for this event
  }
  else
  {
    std::cout << "No leptons in this event" << std::endl;
  }
  if(numNeutralinos != 0)
  {
    h_neutralinoNum->Fill(numNeutralinos); // fill the number of neutralinos for this event
  }
  else
  {
    std::cout << "No neutralinos in this event" << std::endl;
  }


  double biggestLepPt = 0;
  double secondBiggestLepPt = 0;
  double thirdBiggestLepPt = 0;

  if(recoLeptons.size() >= 4)
  {
    const reco::Candidate* bigPTLep;
    std::vector<const reco::Candidate*> lepsMinusBigPTLep;
    for(auto &lep : recoLeptons){ // for all leptons that came from dark photons in this event
      if(lep->pt() > biggestLepPt && std::abs(lep->eta()) < 2.1)
      { // get the biggest pt lepton, and enter all lepton pts into histogram
        if(biggestLepPt != 0)
        {
          lepsMinusBigPTLep.push_back(bigPTLep);
        }
        bigPTLep = lep;
        biggestLepPt = lep->pt();
      }
      else
      { 
        lepsMinusBigPTLep.push_back(lep);
      }
    }

    const reco::Candidate* secondBigPTLep;
    std::vector<const reco::Candidate*> lepsMinusTwoBigPTLep;
    for(auto &lep : lepsMinusBigPTLep)
    {
      if(lep->pt() > secondBiggestLepPt && std::abs(lep->eta()) < 2.1)
      {
        if(secondBiggestLepPt != 0)
        {
          lepsMinusTwoBigPTLep.push_back(secondBigPTLep);
        }
        secondBigPTLep = lep;
        secondBiggestLepPt = lep->pt();
      }
      else
      {
        lepsMinusBigPTLep.push_back(lep);  
      }
    }
    const reco::Candidate* thirdBigPTLep;
    for(auto &lep : lepsMinusTwoBigPTLep)
    {
      if(lep->pt() > thirdBiggestLepPt && std::abs(lep->eta()) < 2.1)
      {
        thirdBigPTLep = lep;
        thirdBiggestLepPt = lep->pt();
      }
    }

    if(biggestLepPt != 0)
    { // get the pt, eta, phi for the lepton with the biggest pt into histograms
      h_lepBiggestPT->Fill(bigPTLep->pt());
      h_lepBiggestPTEta->Fill(bigPTLep->eta());
      h_lepBiggestPTPhi->Fill(bigPTLep->phi());
    }

    if(secondBiggestLepPt != 0)
    {
      h_lepSecondBiggestPT->Fill(secondBigPTLep->pt());
      h_lepSecondBiggestPTEta->Fill(secondBigPTLep->eta());
      h_lepSecondBiggestPTPhi->Fill(secondBigPTLep->phi());
    } 

    if(thirdBiggestLepPt != 0)
    {
      h_lepThirdBiggestPT->Fill(thirdBigPTLep->pt());
      h_lepThirdBiggestPTEta->Fill(thirdBigPTLep->eta());
      h_lepThirdBiggestPTPhi->Fill(thirdBigPTLep->phi());
    }
  
    if(biggestLepPt != 0 && secondBiggestLepPt != 0 && thirdBiggestLepPt != 0)
    {
      ROOT::Math::LorentzVector lep1 = bigPTLep->p4();
      ROOT::Math::LorentzVector lep2 = secondBigPTLep->p4();
      ROOT::Math::LorentzVector lep3 = thirdBigPTLep->p4();
      double deltaR1 = reco::deltaR(lep1, lep2);
      double deltaR2 = reco::deltaR(lep2, lep3);
      double deltaR3 = reco::deltaR(lep1, lep3);

      if(deltaR1 < deltaR2)
      {
        if(deltaR1 < deltaR3)
        {
          h_triggerLeptonsMinDeltaR->Fill(deltaR1);
        }
        else
        {
          h_triggerLeptonsMinDeltaR->Fill(deltaR3);
        }
      }
      else
      {
        if(deltaR2 < deltaR3)
        {
          h_triggerLeptonsMinDeltaR->Fill(deltaR2);
        }
        else
        {
          h_triggerLeptonsMinDeltaR->Fill(deltaR3);
        }
      }
    }
  
    // reconstructing leptons
    if(!recoLeptons.empty()){
     bool accepted = false;

      int recoJetNum = 0;

      // trigger selection (don't forget to set it later in the trigger combination picker too)
      bool bigEle2ndBigEleTrigger = false;
      bool dileptonTrigger = false;
      bool oneMuonTrigger = true;
      bool twoMuonTrigger = true;
      bool threeMuonTrigger = true;

      // acceptance booleans
      bool twoBigEleAccepted = false;
      bool dileptonAccepted = false;
      bool oneMuonAccepted = false;
      bool twoMuonAccepted = false;
      bool threeMuonAccepted = false;

      // turnon curve parameters
      double oneMuonWeight = 0;
      double twoMuonWeight = 0;
      double threeMuonWeight = 0;

      // required due to compiler seeing unused but set variables as errors
      if(twoBigEleAccepted || dileptonAccepted || oneMuonAccepted || twoMuonAccepted || threeMuonAccepted || oneMuonWeight < twoMuonWeight || twoMuonWeight < threeMuonWeight)
      {
        twoBigEleAccepted = false;
      }

      if(bigEle2ndBigEleTrigger)
      {
        if(biggestLepPt > 25 && secondBiggestLepPt > 5 && std::abs(bigPTLep->eta()) < 2.1)
        {
          twoBigEleAccepted = true;
        }
      }
      if(dileptonTrigger)
      {
        for(auto &lep : lepsMinusBigPTLep)
        {
          ROOT::Math::LorentzVector e1 = bigPTLep->p4();
          ROOT::Math::LorentzVector e2 = lep->p4();
          double deltaR = reco::deltaR(e1, e2);
          if(deltaR <= 0.5)
          {
            dileptonAccepted = true;
            break;
          }
        }
      }
      if(oneMuonTrigger)
      {
        if(biggestLepPt >= 50)
        {
          oneMuonAccepted = true;
        }
        if(biggestLepPt < 5)
        {
          oneMuonWeight = 0.05;
        }
        else if(biggestLepPt >= 5 && biggestLepPt < 10)
        {
          oneMuonWeight = 0.27;
        }
        else if(biggestLepPt >= 10 && biggestLepPt < 15)
        {
          oneMuonWeight = 0.37;
        }
        else if(biggestLepPt >= 15 && biggestLepPt < 20)
        {
          oneMuonWeight = 0.45;
        }
        else if(biggestLepPt >= 20 && biggestLepPt < 25)
        {
          oneMuonWeight = 0.57;
        }
        else if(biggestLepPt >= 25 && biggestLepPt < 30)
        {
          oneMuonWeight = 0.7;
        }
        else if(biggestLepPt >= 30 && biggestLepPt < 35)
        {
          oneMuonWeight = .78;
        }
        else if(biggestLepPt >= 35 && biggestLepPt < 40)
        {
          oneMuonWeight = 0.85;
        }
        else if(biggestLepPt >= 40 && biggestLepPt < 50)
        {
          oneMuonWeight = 0.94;
        }
        else if(biggestLepPt >= 50)
        {
          oneMuonWeight = 0.98;
        }
      }
      if(twoMuonTrigger)
      {
        if(biggestLepPt >= 37 && secondBiggestLepPt >= 27)
        {
          twoMuonAccepted = true;
        }
        if(secondBiggestLepPt < 5)
        {
          twoMuonWeight = 0.1;
        }
        else if(secondBiggestLepPt >= 5 && secondBiggestLepPt < 10)
        {
          twoMuonWeight = 0.18;
        }
        else if(secondBiggestLepPt >= 10 && secondBiggestLepPt < 15)
        {
          twoMuonWeight = 0.29;
        }
        else if(secondBiggestLepPt >= 15 && secondBiggestLepPt < 20)
        {
          twoMuonWeight = 0.41;
        }
        else if(secondBiggestLepPt >= 20 && secondBiggestLepPt < 25)
        {
          twoMuonWeight = .6;
        }
        else if(secondBiggestLepPt >=25 && secondBiggestLepPt < 30)
        {
          twoMuonWeight = .85;
        }
        else if(secondBiggestLepPt >= 30)
        {
          twoMuonWeight = .95;
        }
      }
      if(threeMuonTrigger)
      {
        if(biggestLepPt >= 10 && secondBiggestLepPt >= 5 && thirdBiggestLepPt >= 5)
        {
          threeMuonAccepted = true;
          if(thirdBiggestLepPt < 5)
          {
            threeMuonWeight = 0.3;
          }
          else if(thirdBiggestLepPt <10 && thirdBiggestLepPt >=5)
          {
            threeMuonWeight = 0.8;
          }
          else if(thirdBiggestLepPt >=10)
          {
            threeMuonWeight = 0.9;
          }
        }
      }

      // select which combination of triggers to use (don't forget to set the trigger selection above)
      if(oneMuonAccepted || twoMuonAccepted || threeMuonAccepted)
      {
        accepted = true;
      }

      if(accepted)
      {
        h_acceptedEventsNum->Fill(1);

        recoJetNum = leptonJetReco(recoLeptons);
        h_recoLeptonJetNum->Fill(recoJetNum);
        h_jetNumDiff->Fill(recoJetNum-actualJetNum);
      
        for(int jet = 0; jet < actualJetNum; jet++)
        {
          h_actualLeptonJetCounter->Fill(1);
        }
      
        if(biggestLepPt != 0)
        {
          h_lepBiggestPTPasses->Fill(bigPTLep->pt());//, oneMuonWeight);
        }
        if(secondBiggestLepPt != 0)
        {
          h_lepSecondBiggestPTPasses->Fill(secondBigPTLep->pt());//, twoMuonWeight);
        }
        if(thirdBiggestLepPt != 0)
        {
          h_lepThirdBiggestPTPasses->Fill(thirdBigPTLep->pt());//, threeMuonWeight);
        }
      
        for(int jet = 0; jet < recoJetNum; jet++)
        {
          h_recoLeptonJetCounter->Fill(1);
        }
      }
    }
    else
    {
      std::cout << "No Leptons for this event" << std::endl;
    }
  }
  else
  {
    recoLeptons.clear();
  }

  std::vector<const reco::Candidate*> lepsFromChi1;
  std::vector<const reco::Candidate*> lepsFromChi2;

  // comparing four vector of the lepton jet to the four vector of the neutralino
  for(auto &lep : leptons)
  {
    const reco::Candidate* particleOfFocus =  lep;
    const reco::Candidate* motherOfFocus = lep->mother();

    int neutralinoPdgId = 1000022;
    bool neutralinoNotFound = true;
    while(neutralinoNotFound)
    {
      if(motherOfFocus->numberOfMothers() == 0)
      {
        neutralinoNotFound = false;
      }
      else if(motherOfFocus->pdgId() == neutralinoPdgId)
      {
        neutralinoNotFound = false;
        if(motherOfFocus == chi1)
        {
          lepsFromChi1.push_back(lep);
        }
        else if(motherOfFocus == chi2)
        {
          lepsFromChi2.push_back(lep);
        }
        else
        {
          std::cout << "four vector matching algorithm found a nonmatching neutralino";
        }
      }
      else
      {
        particleOfFocus = motherOfFocus;
        motherOfFocus = particleOfFocus->mother();
      }
    }
  }

  // default constructor doesn't create a null vector, thus the following is performed to allow for a null vector
  ROOT::Math::LorentzVector chi1LepSum = chi1->p4() - chi1->p4();
  ROOT::Math::LorentzVector chi2LepSum = chi2->p4() - chi2->p4();
  for(auto &leps : lepsFromChi1)
  {
    chi1LepSum += leps->p4();
  }
  for(auto &leps : lepsFromChi2)
  {
    chi2LepSum += leps->p4();
  }

  if(!lepsFromChi1.empty())
  {
    h_chiPt->Fill(chi1->p4().pt());
    h_chiEta->Fill(chi1->p4().eta());
    h_chiPhi->Fill(chi1->p4().phi());
    h_chiMass->Fill(chi1->p4().mass());    

    h_lepJetPt->Fill(chi1LepSum.pt());
    h_lepJetEta->Fill(chi1LepSum.eta());
    h_lepJetPhi->Fill(chi1LepSum.phi());
    h_lepJetMass->Fill(chi1LepSum.mass());
    
    h_chiLepPtDiff->Fill(chi1->p4().pt() - chi1LepSum.pt());
    h_chiLepEtaDiff->Fill(chi1->p4().eta() - chi1LepSum.eta());
    h_chiLepPhiDiff->Fill(reco::deltaPhi(chi1->p4().phi(), chi1LepSum.phi()));
    h_chiLepMassDiff->Fill(chi1->p4().mass() - chi1LepSum.mass());


  }

  if(!lepsFromChi2.empty())
  {
    h_chiPt->Fill(chi2->p4().pt());
    h_chiEta->Fill(chi2->p4().eta());
    h_chiPhi->Fill(chi2->p4().phi());
    h_chiMass->Fill(chi2->p4().mass());

    h_lepJetPt->Fill(chi2LepSum.pt());
    h_lepJetEta->Fill(chi2LepSum.eta());
    h_lepJetPhi->Fill(chi2LepSum.phi());
    h_lepJetMass->Fill(chi2LepSum.mass());

    h_chiLepPtDiff->Fill(chi2->p4().pt() - chi2LepSum.pt());
    h_chiLepEtaDiff->Fill(chi2->p4().eta() - chi2LepSum.eta());
    h_chiLepPhiDiff->Fill(reco::deltaPhi(chi2->p4().phi(), chi2LepSum.phi()));
    h_chiLepMassDiff->Fill(chi2->p4().mass() - chi2LepSum.mass());
  }

  //printGenParticleCollection(genParts);

  std::cout << "\n\n===========================================================================================================" << std::endl;
  tree_->Fill();  
}

int DimuonHiggsPlusPlus::leptonJetReco(std::vector<const reco::Candidate*> leptons){
  double leptonJets = 0;
  double deltaRCutoff = 0.5;
  // .6 to .7 seems like the best for hadronization off

  int notInJetLeptonNum = 0;
  int originalLeptonNum = leptons.size();

  h_recoLeptonNumPerEvent->Fill(leptons.size());

  // find the lepton with the highest pt
  std::vector<const reco::Candidate*> smallerPTLeptons;
  std::vector<const reco::Candidate*> notInJetLeptons;
  const reco::Candidate* biggestPTLepton = findBiggestPT(leptons, smallerPTLeptons); 
  

  // find the other leptons that are in the same jet
  while(!smallerPTLeptons.empty()){
    std::cout << "size of smallerPTLeptons " << smallerPTLeptons.size() << std::endl;

    bool isJet = false;
    for(auto &lepton : smallerPTLeptons){
      // double deltaEta = biggestPTLepton->eta() - lepton->eta(); // find deltar between the biggest pt lepton and the other leptons
      // double deltaPhi = biggestPTLepton->phi() - lepton->phi();
      
      // double deltaR = std::sqrt((deltaEta)*(deltaEta)-(deltaPhi)*(deltaPhi));
      // checking github update
      ROOT::Math::LorentzVector e1 = biggestPTLepton->p4();
      ROOT::Math::LorentzVector e2 = lepton->p4();
      double deltaR = reco::deltaR(e1, e2);

      if(deltaR > deltaRCutoff){
	      notInJetLeptons.push_back(lepton);
      }
      else{
	isJet = true;
      }
    }
    if(isJet){
      leptonJets++;
    }
    else{
      notInJetLeptonNum++;
      std::cout << "not a jet with " << smallerPTLeptons.size() << " leptons observed" << std::endl;
    }
    smallerPTLeptons.clear();

    biggestPTLepton = findBiggestPT(notInJetLeptons, smallerPTLeptons);
    notInJetLeptons.clear();
  }
  std::cout << "num of lepton jets " << leptonJets << std::endl;
  if(leptonJets == 1)
  {
    h_recoLeptonNumInOneJetEvents->Fill(originalLeptonNum);
  }
  h_notInJetLepton->Fill(notInJetLeptonNum);

  return leptonJets;
}

const reco::Candidate* DimuonHiggsPlusPlus::findBiggestPT(std::vector<const reco::Candidate*> leptons, std::vector<const reco::Candidate*> &smallerPTLeptons){
  double biggestPT = 0;
  const reco::Candidate* biggestPTLepton;

  for(auto &lepton : leptons){
    if(lepton->pt() > biggestPT){
      if(biggestPT != 0){
	smallerPTLeptons.push_back(biggestPTLepton);
      }
      biggestPTLepton = lepton;
      biggestPT = lepton->pt();
    }
    else{
      smallerPTLeptons.push_back(lepton);
    }
  }
  return biggestPTLepton;
}

const void DimuonHiggsPlusPlus::printGenParticleCollection(const reco::GenParticleCollection& genParts)
{
  const reco::Candidate* daughter1 = nullptr;
  const reco::Candidate* daughter2 = nullptr;

  const reco::Candidate* mother1 = nullptr;
  const reco::Candidate* mother2 = nullptr;

  int eventIndex = 0;

  //Format
  std::cout << std::left << std::setw(10) << "index, " << std::setw(10) << "pdfId, " 
	    << std::setw(10) << "status; " << std::setw(10) << "mother1; "
	    << std::setw(10) << "mother2; " << std::setw(10) << "daughter1; " 
	    << std::setw(10) << "daughter2, " << std::setw(10) << "px, " 
	    << std::setw(10) << "py, " << std::setw(10) << "pz, " 
	    << std::setw(10) << "E, " << std::setw(5) << "mass\n";

  //Prints out all of the particles
  for(auto &part : genParts){

    //Distinguishing Mothers
    if(part.numberOfMothers() == 2)
    {
      mother1 = part.mother(0);
      mother2 = part.mother(1);
    }
    else if(part.numberOfMothers() == 1)
    {
      mother1 = part.mother(0);
      mother2 = part.mother(0);
    }
    
    //Distinguishing Daughters (There can be more than 2 daughters, max of three)
    if(part.numberOfDaughters() == 2)
    {
      daughter1 = part.daughter(0);
      daughter2 = part.daughter(1);
    }
    else if(part.numberOfDaughters() == 1)
    {
      daughter1 = part.daughter(0);
      daughter2 = part.daughter(0);
    }

    std::cout << std::setw(10) << eventIndex << ", " << std::setw(10) << part.pdgId() << ", " << std::setw(10) << part.status() << "; ";

    if(part.numberOfMothers() != 0)
    {
      std::cout << std::setw(10) << getIndexOf(mother1, genParts) << "; " << std::setw(10) << getIndexOf(mother2, genParts) << "; ";
    }
    else
    {
      std::cout << std::setw(10) << 00 << "; " << std::setw(10) << 00 << "; ";
    }

    if(part.numberOfDaughters() != 0 && part.numberOfDaughters() != 3)
    {
      std::cout << std::setw(10) << getIndexOf(daughter1, genParts) << "; " << std::setw(10) << getIndexOf(daughter2, genParts) << "; ";
    }
    else
    {
      std::cout << std::setw(10) << 00 << "; " << std::setw(10) << 00 << "; ";
    }

    std::cout << std::setw(10) << part.px() << ", " << std::setw(10) << part.py() << ", " << std::setw(10) << part.pz() << ", " << std::setw(10) << part.energy() << ", " << std::setw(10) << part.mass() << "\n";
    
    eventIndex++;
  }
}

const int DimuonHiggsPlusPlus::getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts)
{
  int indexOf = 0;
  for(auto &possiblePart : genParts)
  {
    if(&possiblePart == part)
      return indexOf;
    indexOf++;
  }
  return -1;
}

bool DimuonHiggsPlusPlus::isBoson(int pid)
{
  if(pid==23 || abs(pid)==22 || pid==32){
    if(debug_ > 0) std::cout << "\n\nFound Boson\n";
    return true;
  }
  else return false;
}

bool DimuonHiggsPlusPlus::isMuon(int pid){
  if(abs(pid)==11 || abs(pid) ==13){
    if(debug_ > 0) std::cout << "\n\nFound A Muon!\n";
    return true;
  }
  else return false;
}

bool DimuonHiggsPlusPlus::checkBosonStatus( const reco::GenParticleCollection& genParts){
  const reco::Candidate* boson = getBoson(genParts);
  if(boson == nullptr){
    if(debug_ > 0) std::cout << "\nBoson is: "  << boson;
    return false;
  }

  else if( boson->status() != 22){
    if(debug_ > 0)  std::cout <<"\nBoson Status is: "<< boson->status();
    return false;
  }
 
    return true;
 }

const reco::Candidate* DimuonHiggsPlusPlus::getBoson( const reco::GenParticleCollection& genParts)
{
  for(auto &part : genParts){
    if(isBoson(part.pdgId())){
      if(debug_ > 1){
      std::cout << "\npId is: " << part.pdgId();
      std::cout << "\nStatus is: " << part.status();
      }
      return getLastDaughter(&part,part.pdgId());
    }
  }
  return nullptr;
}


const reco::Candidate* DimuonHiggsPlusPlus::getLastDaughter(const reco::Candidate* part,int pid)
{
  for(size_t partNr =0; part && partNr<part->numberOfDaughters();partNr++){
    if(part->daughter(partNr)->pdgId()==pid) return getLastDaughter(part->daughter(partNr),pid);
  }
  return part;
}
       
const reco::Candidate* DimuonHiggsPlusPlus::getDaughter(const reco::Candidate* part,int pid)
{  
  for(size_t partNr =0; part && partNr<part->numberOfDaughters();partNr++){
    if(part->daughter(partNr)->pdgId()==pid) return part->daughter(partNr);
  }
  return nullptr;
}

 const reco::Candidate* DimuonHiggsPlusPlus::getMother(const reco::Candidate* part, int pid)
{
  for(size_t partNr = 0; part && partNr < part->numberOfMothers(); partNr++){
    if(part->mother(partNr)->pdgId() == pid) return getMother(part->mother(partNr),pid);
  
    else if(abs(part->mother(partNr)->pdgId()) == 1 || abs(part->mother(partNr)->pdgId()) == 2 ||
	    abs(part->mother(partNr)->pdgId()) == 3 || abs(part->mother(partNr)->pdgId()) == 4 ||
		 abs(part->mother(partNr)->pdgId()) == 5 || abs(part->mother(partNr)->pdgId()) == 6 ||
	    abs(part->mother(partNr)->pdgId()) == 7 || abs(part->mother(partNr)->pdgId()) == 8 ||
	    abs(part->mother(partNr)->pdgId()) == 23 || abs(part->mother(partNr)->pdgId()) == 32  || 
	    abs(part->mother(partNr)->pdgId()) == 22) return part->mother(partNr);
  }  
  return nullptr;
  
}




// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just after ending the event loop  ------------
void 
DimuonHiggsPlusPlus::endJob() 
{
  h_lepBiggestPTPasses->Divide(h_lepBiggestPT); 
  h_lepSecondBiggestPTPasses->Divide(h_lepSecondBiggestPT); 
  h_lepThirdBiggestPTPasses->Divide(h_lepThirdBiggestPT); 
}

// ------------ method called when starting to processes a run  ------------
  
/*void 
Dimuon::beginRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup)
{
   edm::Handle< GenRunInfoProduct > genInfoProduct;
  iRun.getByToken(genInfoProductToken_, genInfoProduct );
  crossSec = genInfoProduct->internalXSec().value();
  //  tree_->Fill();
  std::cout<< "Cross Section is: "  << crossSec << std::endl;  
 

}
*/

// ------------ method called when ending the processing of a run  ------------

void 
  DimuonHiggsPlusPlus::endRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup)
  {
   edm::Handle< GenRunInfoProduct > genInfoProduct;
  iRun.getByToken(genInfoProductToken_, genInfoProduct );
  crossSec = genInfoProduct->internalXSec().value();
  std::cout<< "Cross Section is: "  << crossSec << std::endl;  

  double d = (double)countXY/(double)totalXY;
  std::cout<< d << std::endl;
 
  }

  
// ------------ method called when starting to processes a luminosity block  ------------
/*
  void 
  Dimuon::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
  void 
  Dimuon::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
  {
  }
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DimuonHiggsPlusPlus::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DimuonHiggsPlusPlus);