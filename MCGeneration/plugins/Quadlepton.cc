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
// Original Author:  Teodor Dimitrov Tchalakov
//         Created:  Summer 2020
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Run.h"
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
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//


// std::cout << "Made it to line" << __LINE__ << std::endl;

class Quadlepton : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit Quadlepton(const edm::ParameterSet&);
  ~Quadlepton();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  const reco::Candidate* getDaughter(const reco::Candidate* part,int pid);
  const reco::Candidate* getLastDaughter(const reco::Candidate* part,int pid);
  const reco::Candidate* getBoson(const reco::GenParticleCollection& genParts);
  const reco::Candidate* getMother(const reco::Candidate* part, int pid);
  //const reco::Candidate* getDYBoson(const reco::Candidate* part int pid)
  bool isBoson(int pid);
  bool isMuon(int pid);
  bool checkBosonStatus(const reco::GenParticleCollection& genParts);

  //Added for LRSM Doubly Charged Higgs Analysis
  const int getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts);
  const void printGenParticleCollection(const reco::GenParticleCollection& genParts);

  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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

  TH1F *h_massInvar;
  TH1F *h_massInvarOSDL;
  
  TH1F *h_massInvarWithCuts;
  TH1F *h_massInvarAcceptance5GeV1Higgs, *h_massInvarAcceptance2GeV1Higgs;
  TH1F *h_massInvarAcceptance5GeV2Higgs, *h_massInvarAcceptance2GeV2Higgs;



  //ZPrime stuff

  P4Struct bosonP4_; // as a sanity check we have the right event...
  P4Struct muMinusP4_;
  P4Struct muPlusP4_;
  int muMinusPID_;
  int muPlusPID_;
  int bosonId_;
  double crossSec, cosTheta, tanPhi, csTheta, csPhi;
  double mCosThetaPlus, mCosThetaMinus;

  int debug_;
  edm::InputTag genPartsTag_;
  int decayParticlePID_;
  edm::InputTag genInfoProduct_;
  edm::EDGetTokenT<GenRunInfoProduct> genInfoProductToken_;
  edm::EDGetTokenT<reco::GenParticleCollection> genPartsToken_;

  // ----------member data ---------------------------
  
};


void Quadlepton::beginJob()
{
  //std::cout << "Made it (in beginJob) to line" << __LINE__ << std::endl;

  //Pt Histogram Range
 // double ptMaxRange = 1000;
  //double massMaxRange = 15000;
  double massInvarMaxRange = 5700;

  edm::Service<TFileService> fs;

  h_massInvar = fs->make<TH1F>("Invariant mass", "Invariant mass", 100, 0., 1400);
  h_massInvarOSDL = fs->make<TH1F>("Invariant mass OSDL", "Invariant mass OSDL", 100, 0., massInvarMaxRange);

  h_massInvarWithCuts = fs->make<TH1F>("Invariant mass with cuts", "Invariant mass", 100, 0., 1400);
  h_massInvarAcceptance5GeV1Higgs = fs->make<TH1F>("Invariant mass Acceptance 5 GeV with 1 Higgs", "Invariant mass", 100, 0., 1400);
  h_massInvarAcceptance2GeV1Higgs = fs->make<TH1F>("Invariant mass Acceptance 2 GeV with 1 Higgs", "Invariant mass", 100, 0., 1400);
  h_massInvarAcceptance5GeV2Higgs = fs->make<TH1F>("Invariant mass Acceptance 5 GeV with 2 Higgs", "Invariant mass", 100, 0., 1400);
  h_massInvarAcceptance2GeV2Higgs = fs->make<TH1F>("Invariant mass Acceptance 2 GeV with 2 Higgs", "Invariant mass", 100, 0., 1400);

  //Tree of extra data, NOT USED for LRSM Doubly Charged Higgs Analysis
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
Quadlepton::Quadlepton(const edm::ParameterSet& iConfig)

{

  //std::cout << "Made it (in constructor) to line" << __LINE__ << std::endl;

  debug_=iConfig.getParameter<int>("debug");
  genPartsTag_=iConfig.getParameter<edm::InputTag>("genPartsTag");
  decayParticlePID_ = iConfig.getParameter<int>("decayParticlePID");
  genInfoProduct_ = iConfig.getParameter<edm::InputTag>("genInfoProduct");
  
  //now do what ever initialization is needed

  genInfoProductToken_ = consumes<GenRunInfoProduct,edm::InRun>(genInfoProduct_);
  genPartsToken_ = consumes<reco::GenParticleCollection>(genPartsTag_);
}


Quadlepton::~Quadlepton()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
Quadlepton::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //std::cout << "Made it (in analyze) to line" << __LINE__ << std::endl;
  using namespace edm;
  edm::Handle<reco::GenParticleCollection> genPartsHandle;
  iEvent.getByToken(genPartsToken_,genPartsHandle);
  const reco::GenParticleCollection& genParts = *genPartsHandle;

  //clears some property vectors
  bosonId_=0;
  bosonP4_.clear();
  muMinusP4_.clear();
  muPlusP4_.clear();
  muMinusPID_=0;
  muPlusPID_=0;

  //printGenParticleCollection(genParts);
  //std::exit(1);//uncomment if only printing one particle collection

  
  std::vector<reco::GenParticle> leptons;
  //loop through gen particles for four leptons
  for(auto &part : genParts){
    if(part.numberOfDaughters() == 0 && (abs(part.pdgId()) == 13 || abs(part.pdgId()) == 11))
      {
	if(leptons.size() == 0)
	  {
	    //std::cout << "Added first lepton found at line " << __LINE__ << std::endl;
	    //std::cout << "Inserted lepton " << part.pdgId() << " at line " << __LINE__ << " with pt " << part.pt() << " at index " << 0 << std::endl;
	    leptons.push_back(part);
	  }
	else
	  {
	    bool sortedLepton = false;
	    for(int i = 0; !sortedLepton && i < (int) leptons.size(); i++)
	      {
		//std::cout << "Checking lepton " << part.pdgId() << " at line " << __LINE__ << " with pt " << part.pt() << " compared to index " << i << std::endl;
		if(part.pt() <= leptons[i].pt())
		  {
		    leptons.insert(leptons.begin() + i, part);
		    //std::cout << "Inserted lepton " << part.pdgId() << " at line " << __LINE__ << " with pt " << part.pt() << " at index " << i << std::endl;
		    sortedLepton = true;
		  }
	      }
	    if(!sortedLepton)
	      {
		leptons.push_back(part);
	      }
	  }
      }
  }

  //std::cout << "Found " << leptons.size() << " Final State Leptons at line " << __LINE__ << std::endl;
  for(int i = 0; i < (int) leptons.size(); i++)
    std::cout << &leptons.at(i) << " " << leptons.at(i).pdgId() << ", ";

  reco::GenParticle* leptonMinus1 = nullptr;
  reco::GenParticle* leptonPlus1 = nullptr;
  reco::GenParticle* leptonMinus2 = nullptr;
  reco::GenParticle* leptonPlus2 = nullptr;
  
  for(int i = leptons.size() - 1; i >= 0; i--)
    {
      if(leptons[i].charge() < 0)
	{
	  if(!leptonPlus1) leptonPlus1 = &leptons[i];
	  else if(!leptonPlus2) leptonPlus2 = &leptons[i];
	}
      else if(leptons[i].charge() > 0)
	{
	  if(!leptonMinus1) leptonMinus1 = &leptons[i];
	  else if(!leptonMinus2) leptonMinus2 = &leptons[i];
	}
    }

  std::cerr << "Made it to line " << __LINE__ << " with leptons\n" << leptonMinus1 << "\n" << leptonPlus1 << "\n" << leptonMinus2 << "\n" << leptonPlus2 << std::endl; 

  
  if(leptonMinus1 && leptonPlus1 && leptonMinus2 && leptonPlus2)
    {
      //SSDL
      double invariantMass1 = sqrt(2 * leptonMinus1->pt() * leptonMinus2->pt() *(cosh(leptonMinus1->eta() - leptonMinus2->eta()) - cos(TVector2::Phi_mpi_pi(leptonMinus1->phi() - leptonMinus2->phi()))));

      double invariantMass2 = sqrt(2 * leptonPlus1->pt() * leptonPlus2->pt() *(cosh(leptonPlus1->eta() - leptonPlus2->eta()) - cos(TVector2::Phi_mpi_pi(leptonPlus1->phi() - leptonPlus2->phi()))));

      h_massInvar->Fill(invariantMass1);
      h_massInvar->Fill(invariantMass2);

      //OSDL
      double invariantMass1OSDL = sqrt(2 * leptonMinus1->pt() * leptonPlus1->pt() *(cosh(leptonMinus1->eta() - leptonPlus1->eta()) - cos(TVector2::Phi_mpi_pi(leptonMinus1->phi() - leptonPlus1->phi()))));

      double invariantMass2OSDL = sqrt(2 * leptonMinus2->pt() * leptonPlus2->pt() *(cosh(leptonMinus2->eta() - leptonPlus2->eta()) - cos(TVector2::Phi_mpi_pi(leptonMinus2->phi() - leptonPlus2->phi()))));

      h_massInvarOSDL->Fill(invariantMass1OSDL);
      h_massInvarOSDL->Fill(invariantMass2OSDL);
    }

  //Invariant Mass cuts by pT in GeV
  double firstCut = 30, secondCut = 30, thirdCut = 20, fourthCut = 20;
 // std::cout << "Applying cuts: " << firstCut << ", " << secondCut << ", " << thirdCut << ", " << fourthCut << " from highest to lowest pt at line " << __LINE__ << std::endl;

  std::vector<reco::GenParticle*> fourLeptonEvent = {leptonMinus1, leptonPlus1, leptonMinus2, leptonPlus2};
  for(int i = 0; i < (int) fourLeptonEvent.size(); i++)
    {
      std::cout << fourLeptonEvent.at(i) << ' ' << fourLeptonEvent.at(i)->pt() << std::endl;
    }

  for(int i = 0; i < (int) fourLeptonEvent.size(); i++)
    {
      const reco::GenParticle* largest = nullptr;
      int largestIndex = i;
      for(int x = i; x < (int) fourLeptonEvent.size(); x++)
	{
	  if(largest == nullptr || largest->pt() < fourLeptonEvent[x]->pt())
	    {
	      largest = fourLeptonEvent[x];
	      largestIndex = x;
	    }
	}
      //std::cout << "Found Largest at line " << __LINE__ << " with pt " << fourLeptonEvent[largestIndex]->pt() << std::endl;
      reco::GenParticle* temp = fourLeptonEvent[i];
      fourLeptonEvent[i] = fourLeptonEvent[largestIndex];
      fourLeptonEvent[largestIndex] = temp;
    }
  
  //std::cout << "Printing Sorted Four Leptons at line " << __LINE__ << std::endl;
  for(int i = 0; i < (int) fourLeptonEvent.size(); i++)
    {
      std::cout << fourLeptonEvent.at(i) << ' ' << fourLeptonEvent.at(i)->pt() << std::endl;
    }
  
  if(fourLeptonEvent[0]->pt() > firstCut && fourLeptonEvent[1]->pt() > secondCut
     && fourLeptonEvent[2]->pt() > thirdCut && fourLeptonEvent[3]->pt() > fourthCut)
    {
      for(int i = fourLeptonEvent.size() - 1; i >= 0; i--)
	{
	  if(fourLeptonEvent[i]->charge() < 0)
	    {
	      if(!leptonPlus1) leptonPlus1 = fourLeptonEvent[i];
	      else if(!leptonPlus2) leptonPlus2 = fourLeptonEvent[i];
	    }
	  else if(fourLeptonEvent[i]->charge() > 0)
	    {
	      if(!leptonMinus1) leptonMinus1 = fourLeptonEvent[i];
	      else if(!leptonMinus2) leptonMinus2 = fourLeptonEvent[i];
	    }
	}

      if(leptonMinus1 && leptonPlus1 && leptonMinus2 && leptonPlus2)
	{
	  //SSDL
	  double invariantMass1 = sqrt(2 * leptonMinus1->pt() * leptonMinus2->pt() *(cosh(leptonMinus1->eta() - leptonMinus2->eta()) - cos(TVector2::Phi_mpi_pi(leptonMinus1->phi() - leptonMinus2->phi()))));

	  double invariantMass2 = sqrt(2 * leptonPlus1->pt() * leptonPlus2->pt() *(cosh(leptonPlus1->eta() - leptonPlus2->eta()) - cos(TVector2::Phi_mpi_pi(leptonPlus1->phi() - leptonPlus2->phi()))));

	  h_massInvarWithCuts->Fill(invariantMass1);
	  h_massInvarWithCuts->Fill(invariantMass2);

	  //Calculate number of accepted events for +-5 GeV and +-2 GeV
	  double higgsMass = 200;//GeV
	  //std::cout << "Calculating acceptance for mass of " << higgsMass << " at line " << __LINE__ << std::endl;

	  //+-5GeV 1 higgs event
	  if(abs(invariantMass1 - higgsMass) <= 5 || abs(invariantMass2 - higgsMass) <= 5)
	    {
	      h_massInvarAcceptance5GeV1Higgs->Fill(higgsMass);
	      //std::cout << "Accepted event from masses " << invariantMass1 << " and " << invariantMass2 << " at line " << __LINE__ << " for +-5 GeV acceptance for 1 higgs" << std::endl;
	    }
	  else
	    {
	      std::cout << "Acceptance for event of masses " << invariantMass1 << " and " << invariantMass2 << " +-5 GeV not accepted for 1 higgs" << std::endl;
	    }

	  //+-2GeV 1 higgs event
	  if(abs(invariantMass1 - higgsMass) <= 2 || abs(invariantMass2 - higgsMass) <= 2)
	    {
	      h_massInvarAcceptance2GeV1Higgs->Fill(higgsMass);
	      //std::cout << "Accepted event from masses " << invariantMass1 << " and " << invariantMass2 << " at line " << __LINE__ << " for +-2 GeV acceptance for 1 higgs" << std::endl;
	    }
	  else
	    {
	      std::cout << "Acceptance for event of masses " << invariantMass1 << " and " << invariantMass2 << " +-2 GeV not accepted for 1 higgs" << std::endl;
	    }

	  //+-5Gev 2 higgs event
	  if(abs(invariantMass1 - higgsMass) <= 5 && abs(invariantMass2 - higgsMass) <= 5)
	    {
	      h_massInvarAcceptance5GeV2Higgs->Fill(higgsMass);
	     // std::cout << "Accepted event from masses " << invariantMass1 << " and " << invariantMass2 << " at line " << __LINE__ << " for +-5 GeV acceptance for 2 higgs" << std::endl;
	    }
	  else
	    {
	      std::cout << "Acceptance for event of masses " << invariantMass1 << " and " << invariantMass2 << " +-5 GeV not accepted for 2 higgs" << std::endl;
	    }

	  //+-2GeV 2 higgs event
	  if(abs(invariantMass1 - higgsMass) <= 2 && abs(invariantMass2 - higgsMass) <= 2)
	    {
	      h_massInvarAcceptance2GeV2Higgs->Fill(higgsMass);
	      //std::cout << "Accepted event from masses " << invariantMass1 << " and " << invariantMass2 << " at line " << __LINE__ << " for +-2 GeV acceptance for 2 higgs" << std::endl;
	    }
	  else
	    {
	      std::cout << "Acceptance for event of masses " << invariantMass1 << " and " << invariantMass2 << " +-2 GeV not accepted for 2 higgs" << std::endl;
	    }
	}
    }
  else
    {
      std::cerr << "Kinematics Cuts Failed at line " << __LINE__ << std::endl;
    }  
  
  

 // std::cout << "\n\n===========================================================================================================" << std::endl;
  tree_->Fill();  
}

bool Quadlepton::isBoson(int pid)
{
  if(pid==23 || abs(pid)==22 || pid==32){
    if(debug_ > 0) std::cout << "\n\nFound Boson\n";
    return true;
  }
  else return false;
}

bool Quadlepton::isMuon(int pid){
  if(abs(pid)==11 || abs(pid) ==13){
    if(debug_ > 0) std::cout << "\n\nFound A Muon!\n";
    return true;
  }
  else return false;
}

bool Quadlepton::checkBosonStatus( const reco::GenParticleCollection& genParts){
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

const reco::Candidate* Quadlepton::getBoson( const reco::GenParticleCollection& genParts)
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


//if the daughter's pdgId matches the parent particle id, then it returns that particle as the last daughter particle, otherwise it will return the particle itself
//essentially, it gets the most final state particle (the last generation) from any particle
//would this not cause a recursive loop?
const reco::Candidate* Quadlepton::getLastDaughter(const reco::Candidate* part, int pid)
{
  for(size_t partNr = 0; part && partNr < part->numberOfDaughters(); partNr++){
    if(part->daughter(partNr)->pdgId() == pid)
      return getLastDaughter(part->daughter(partNr),pid);
  }
  return part;
}
       
const reco::Candidate* Quadlepton::getDaughter(const reco::Candidate* part,int pid)
{  
  for(size_t partNr = 0; part && partNr<part->numberOfDaughters(); partNr++)
  {
    if(part->daughter(partNr)->pdgId() == pid) return part->daughter(partNr);
  }
  return nullptr;
}

const reco::Candidate* Quadlepton::getMother(const reco::Candidate* part, int pid)
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

const int Quadlepton::getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts)
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
const void Quadlepton::printGenParticleCollection(const reco::GenParticleCollection& genParts)
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

// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just after ending the event loop  ------------
void 
Quadlepton::endJob() 
{
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

// void 
// Quadlepton::endRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup)
// {
//   edm::Handle< GenRunInfoProduct > genInfoProduct;
//   iRun.getByToken(genInfoProductToken_, genInfoProduct );
//   crossSec = genInfoProduct->internalXSec().value();
//   std::cout<< "Cross Section is: "  << crossSec << std::endl;  
// }

  
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
Quadlepton::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Quadlepton);
