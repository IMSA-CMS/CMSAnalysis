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

class DimuonDarkPhoton : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit DimuonDarkPhoton(const edm::ParameterSet&);
  ~DimuonDarkPhoton();

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
  const void fillOSDLHistograms(const reco::Candidate* dileptons[]);

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

  //Histograms for LRSM Doubly Charged Higgs
  //No Histograms are made for comparing L vs R because it will be visible in histograms
  TH1F *h_HigPlusMass, *h_HigPlusPt, *h_HigPlusEta, *h_HigPlusPhi, *h_HigPlusCharge;//Plus is ++ and Minus is --
  TH1F *h_HigMinusMass, *h_HigMinusPt, *h_HigMinusEta, *h_HigMinusPhi, *h_HigMinusCharge;

  //Individual particles
  TH1F *h_MuMinusMass,*h_MuMinusPt,*h_MuMinusEta,*h_MuMinusPhi,*h_MuMinusCharge;
  TH1F *h_MuPlusMass,*h_MuPlusPt,*h_MuPlusEta,*h_MuPlusPhi,*h_MuPlusCharge;
  TH1F *h_thetaMuMinus, *h_thetaMuPlus;

  TH1F *h_ElectMinusMass,*h_ElectMinusPt,*h_ElectMinusEta,*h_ElectMinusPhi,*h_ElectMinusCharge;
  TH1F *h_ElectPlusMass,*h_ElectPlusPt,*h_ElectPlusEta,*h_ElectPlusPhi,*h_ElectPlusCharge;
  TH1F *h_thetaElectMinus, *h_thetaElectPlus;

  //Pt ordered particles for cuts
  //Muons
  TH1F *h_MuMinusPt1,*h_MuMinusEta1,*h_MuMinusPhi1;//One is higher pt
  TH1F *h_MuMinusPt2,*h_MuMinusEta2,*h_MuMinusPhi2;

  TH1F *h_MuPlusPt1,*h_MuPlusEta1,*h_MuPlusPhi1;
  TH1F *h_MuPlusPt2,*h_MuPlusEta2,*h_MuPlusPhi2;

  //Electrons
  TH1F *h_ElectMinusPt1,*h_ElectMinusEta1,*h_ElectMinusPhi1;//One is higher pt
  TH1F *h_ElectMinusPt2,*h_ElectMinusEta2,*h_ElectMinusPhi2;

  TH1F *h_ElectPlusPt1,*h_ElectPlusEta1,*h_ElectPlusPhi1;
  TH1F *h_ElectPlusPt2,*h_ElectPlusEta2,*h_ElectPlusPhi2;

  //Highest pt lepton
  TH1F *h_HighestPtLepton, *h_SecondHighestPtLepton, *h_ThirdHighestPtLepton, *h_FourthHighestPtLepton;

  //Opposite Sign Dileptons(OSDL) for missidentification purposes with individual particles
  TH1F *h_MuMinusMassOSDL,*h_MuMinusPtOSDL,*h_MuMinusEtaOSDL,*h_MuMinusPhiOSDL;
  TH1F *h_MuPlusMassOSDL,*h_MuPlusPtOSDL,*h_MuPlusEtaOSDL,*h_MuPlusPhiOSDL;
  TH1F *h_thetaMuMinusOSDL, *h_thetaMuPlusOSDL;

  TH1F *h_ElectMinusMassOSDL,*h_ElectMinusPtOSDL,*h_ElectMinusEtaOSDL,*h_ElectMinusPhiOSDL;
  TH1F *h_ElectPlusMassOSDL,*h_ElectPlusPtOSDL,*h_ElectPlusEtaOSDL,*h_ElectPlusPhiOSDL;
  TH1F *h_thetaElectMinusOSDL, *h_thetaElectPlusOSDL;

  //Dilepton signals
  //Muon
  TH1F *h_DimuonPt, *h_DimuonEta, *h_DimuonPhi;
  TH1F *h_DimuonMinusMassInvar, *h_DimuonPlusMassInvar;

  //Electron
  TH1F *h_DielectronPt, *h_DielectronEta, *h_DielectronPhi;
  TH1F *h_DielectronMinusMassInvar, *h_DielectronPlusMassInvar;
  
  //General Lepton
  TH1F *h_DileptonPlusMassInvar, *h_DileptonMinusMassInvar;

  //Opposite Sign Dileptons(OSDL) for missidentification purposes with dileptons particles
  TH1F *h_DimuonPtOSDL, *h_DimuonEtaOSDL, *h_DimuonPhiOSDL, *h_DimuonMassInvarOSDL;

  TH1F *h_DielectronPtOSDL, *h_DielectronEtaOSDL, *h_DielectronPhiOSDL, *h_DielectronMassInvarOSDL;

  //General
  TH1F *h_dPhi,*h_dEta, *h_dTheta, *h_dR;

  TH1F *h_massInvar;
  TH1F *h_massInvarZZBackground;
  TH1F *h_FourLeptonMassInvar;
  TH1F *h_cosTheta, *h_tanPhi, *h_csTheta, *h_csPhi;
  TH1F *h_cosThetaPlusInvariantMass, *h_cosThetaMinusInvariantMass;
  
  TH2F *h2_pt1_vs_pt2,*h2_eta1_vs_eta2,*h2_phi1_vs_phi2;



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


void DimuonDarkPhoton::beginJob()
{
  //std::cout << "Made it (in beginJob) to line" << __LINE__ << std::endl;

  //Pt Histogram Range
  double ptMaxRange = 2500;
  double massMaxRange = 15000;
  double massInvarMaxRange = 2000;

  edm::Service<TFileService> fs;
  //makes all of the histograms for properties of the interactions
  //Histograms for LRSM Doubly Charged Higgs
  h_HigPlusMass = fs->make<TH1F>("H++ mass", "m", 100, 0., massMaxRange);
  h_HigPlusPt = fs->make<TH1F>("H++ pt", "p_{t}", 500,  0., ptMaxRange);
  h_HigPlusEta = fs->make<TH1F>("H++ eta", "#eta", 100, -10., 10);
  h_HigPlusPhi = fs->make<TH1F>("H++ phi", "#phi", 100,  -3.20, 3.20);
  h_HigPlusCharge = fs->make<TH1F>("H++ charge", "Q", 3, -2.5, 2.5);

  h_HigMinusMass = fs->make<TH1F>("H-- mass", "m", 100, 0., massMaxRange);
  h_HigMinusPt = fs->make<TH1F>("H-- pt", "p_{t}", 500,  0., ptMaxRange);
  h_HigMinusEta = fs->make<TH1F>("H-- eta", "#eta", 100, -10., 10.);
  h_HigMinusPhi = fs->make<TH1F>("H-- phi", "#phi", 100,  -3.20, 3.20);
  h_HigMinusCharge = fs->make<TH1F>("H-- charge", "Q", 3, -2.5, 2.5);
  
  //Muon Histograms
  h_MuPlusMass = fs->make<TH1F>("Muon+ mass", "m", 1000, 0., massMaxRange/1000);
  h_MuPlusPt = fs->make<TH1F>("Muon+ pt", "p_{t}", 500,  0., ptMaxRange);
  h_MuPlusEta = fs->make<TH1F>("Muon+ eta", "#eta", 100, -5., 5.);
  h_MuPlusPhi = fs->make<TH1F>("Muon+ phi", "#phi", 100, -3.15, 3.15);
  h_MuPlusCharge = fs->make<TH1F>("Muon+ charge", "Q", 3, -1.5, 1.5);

  h_MuMinusMass = fs->make<TH1F>("Muon- mass", "m", 1000, 0., massMaxRange/1000);
  h_MuMinusPt = fs->make<TH1F>("Muon- pt", "p_{t}", 500,  0., ptMaxRange);
  h_MuMinusEta = fs->make<TH1F>("Muon- eta", "#eta", 100, -5., 5.);
  h_MuMinusPhi = fs->make<TH1F>("Muon- phi", "#phi", 100, -3.15, 3.15);
  h_MuMinusCharge = fs->make<TH1F>("Muon- charge", "Q", 3, -1.5, 1.5);

  h_thetaMuMinus = fs->make<TH1F>("theta Muon-", "#theta", 100, -3.15, 3.15);      
  h_thetaMuPlus = fs->make<TH1F>("theta Muon+", "#theta", 100, -3.15, 3.15);

  //Pt ordered histograms for cuts
  h_MuPlusPt1 = fs->make<TH1F>("Muon+ pt High", "p_{t}", 500,  0., ptMaxRange);
  h_MuPlusEta1 = fs->make<TH1F>("Muon+ eta High", "#eta", 100, -5., 5.);
  h_MuPlusPhi1 = fs->make<TH1F>("Muon+ phi High", "#phi", 100, -3.15, 3.15);

  h_MuPlusPt2 = fs->make<TH1F>("Muon+ pt Low", "p_{t}", 500,  0., ptMaxRange);
  h_MuPlusEta2 = fs->make<TH1F>("Muon+ eta Low", "#eta", 100, -5., 5.);
  h_MuPlusPhi2 = fs->make<TH1F>("Muon+ phi Low", "#phi", 100, -3.15, 3.15);

  h_MuMinusPt1 = fs->make<TH1F>("Muon- pt High", "p_{t}", 500,  0., ptMaxRange);
  h_MuMinusEta1 = fs->make<TH1F>("Muon- eta High", "#eta", 100, -5., 5.);
  h_MuMinusPhi1 = fs->make<TH1F>("Muon- phi High", "#phi", 100, -3.15, 3.15);

  h_MuMinusPt2 = fs->make<TH1F>("Muon- pt Low", "p_{t}", 500,  0., ptMaxRange);
  h_MuMinusEta2 = fs->make<TH1F>("Muon- eta Low", "#eta", 100, -5., 5.);
  h_MuMinusPhi2 = fs->make<TH1F>("Muon- phi Low", "#phi", 100, -3.15, 3.15);

  //Muon Opposite Sign Dileptons(OSDL) for missidentification
  h_MuPlusMassOSDL = fs->make<TH1F>("Muon+ mass OSDL", "m", 1000, 0., massMaxRange/1000);
  h_MuPlusPtOSDL = fs->make<TH1F>("Muon+ pt OSDL", "p_{t}", 500,  0., ptMaxRange);
  h_MuPlusEtaOSDL = fs->make<TH1F>("Muon+ eta OSDL", "#eta", 100, -5., 5.);
  h_MuPlusPhiOSDL = fs->make<TH1F>("Muon+ phi OSDL", "#phi", 100, -3.15, 3.15);
  //h_MuPlusChargeOSDL = fs->make<TH1F>("Muon+ charge OSDL", "Q", 3, -1.5, 1.5);

  h_MuMinusMassOSDL = fs->make<TH1F>("Muon- mass OSDL", "m", 1000, 0., massMaxRange/1000);
  h_MuMinusPtOSDL = fs->make<TH1F>("Muon- pt OSDL", "p_{t}", 500,  0., ptMaxRange);
  h_MuMinusEtaOSDL = fs->make<TH1F>("Muon- eta OSDL", "#eta", 100, -5., 5.);
  h_MuMinusPhiOSDL = fs->make<TH1F>("Muon- phi OSDL", "#phi", 100, -3.15, 3.15);
  //h_MuMinusChargeOSDL = fs->make<TH1F>("Muon- charge OSDL", "Q", 3, -1.5, 1.5);

  h_thetaMuMinusOSDL = fs->make<TH1F>("theta Muon- OSDL", "#theta", 100, -3.15, 3.15);      
  h_thetaMuPlusOSDL = fs->make<TH1F>("theta Muon+ OSDL", "#theta", 100, -3.15, 3.15);

  //Electron Histograms (Maybe switch to be positron)?
  h_ElectPlusMass = fs->make<TH1F>("Electron+ mass", "m", 1000, 0., 700/1000000);
  h_ElectPlusPt = fs->make<TH1F>("Electron+ pt", "p_{t}", 500,  0., ptMaxRange);
  h_ElectPlusEta = fs->make<TH1F>("Electron+ eta", "#eta", 100, -5., 5.);
  h_ElectPlusPhi = fs->make<TH1F>("Electron+ phi", "#phi", 100, -3.15, 3.15);
  h_ElectPlusCharge = fs->make<TH1F>("Electron+ charge", "Q", 3, -1.5, 1.5);

  h_ElectMinusMass = fs->make<TH1F>("Electron- mass", "m", 1000, 0., 700/1000000);
  h_ElectMinusPt = fs->make<TH1F>("Electron- pt", "p_{t}", 500,  0., ptMaxRange);
  h_ElectMinusEta = fs->make<TH1F>("Electron- eta", "#eta", 100, -5., 5.);
  h_ElectMinusPhi = fs->make<TH1F>("Electron- phi", "#phi", 100, -3.15, 3.15);
  h_ElectMinusCharge = fs->make<TH1F>("Electron- charge", "Q", 3, -1.5, 1.5);

  h_thetaElectMinus = fs->make<TH1F>("theta Electron-", "#theta", 100, -3.15, 3.15);      
  h_thetaElectPlus = fs->make<TH1F>("theta Electron+", "#theta", 100, -3.15, 3.15);

  //Pt ordered histograms for cuts
  h_ElectPlusPt1 = fs->make<TH1F>("Electron+ pt High", "p_{t}", 500,  0., ptMaxRange);
  h_ElectPlusEta1 = fs->make<TH1F>("Electron+ eta High", "#eta", 100, -5., 5.);
  h_ElectPlusPhi1 = fs->make<TH1F>("Electron+ phi High", "#phi", 100, -3.15, 3.15);

  h_ElectPlusPt2 = fs->make<TH1F>("Electron+ pt Low", "p_{t}", 500,  0., ptMaxRange);
  h_ElectPlusEta2 = fs->make<TH1F>("Electron+ eta Low", "#eta", 100, -5., 5.);
  h_ElectPlusPhi2 = fs->make<TH1F>("Electron+ phi Low", "#phi", 100, -3.15, 3.15);

  h_ElectMinusPt1 = fs->make<TH1F>("Electron- pt High", "p_{t}", 500,  0., ptMaxRange);
  h_ElectMinusEta1 = fs->make<TH1F>("Electron- eta High", "#eta", 100, -5., 5.);
  h_ElectMinusPhi1 = fs->make<TH1F>("Electron- phi High", "#phi", 100, -3.15, 3.15);

  h_ElectMinusPt2 = fs->make<TH1F>("Electron- pt Low", "p_{t}", 500,  0., ptMaxRange);
  h_ElectMinusEta2 = fs->make<TH1F>("Electron- eta Low", "#eta", 100, -5., 5.);
  h_ElectMinusPhi2 = fs->make<TH1F>("Electron- phi Low", "#phi", 100, -3.15, 3.15);

  //Electron Opposite Sign Dileptons(OSDL) for missidentification
  h_ElectPlusMassOSDL = fs->make<TH1F>("Electron+ mass OSDL", "m", 1000, 0., 700/1000000);
  h_ElectPlusPtOSDL = fs->make<TH1F>("Electron+ pt OSDL", "p_{t}", 500,  0., ptMaxRange);
  h_ElectPlusEtaOSDL = fs->make<TH1F>("Electron+ eta OSDL", "#eta", 100, -5., 5.);
  h_ElectPlusPhiOSDL = fs->make<TH1F>("Electron+ phi OSDL", "#phi", 100, -3.15, 3.15);
  //h_ElectPlusChargeOSDL = fs->make<TH1F>("Electron+ charge OSDL", "Q", 3, -1.5, 1.5);

  h_ElectMinusMassOSDL = fs->make<TH1F>("Electron- mass OSDL", "m", 1000, 0., 700/100000);
  h_ElectMinusPtOSDL = fs->make<TH1F>("Electron- pt OSDL", "p_{t}", 500,  0., ptMaxRange);
  h_ElectMinusEtaOSDL = fs->make<TH1F>("Electron- eta OSDL", "#eta", 100, -5., 5.);
  h_ElectMinusPhiOSDL = fs->make<TH1F>("Electron- phi OSDL", "#phi", 100, -3.15, 3.15);
  //h_ElectMinusChargeOSDL = fs->make<TH1F>("Electron- charge OSDL", "Q", 3, -1.5, 1.5);

  h_thetaElectMinusOSDL = fs->make<TH1F>("theta Electron- OSDL", "#theta", 100, -3.15, 3.15);      
  h_thetaElectPlusOSDL = fs->make<TH1F>("theta Electron+ OSDL", "#theta", 100, -3.15, 3.15);

  //Pt ordered histogram
  double ptOrderResolution = 400, ptOrderMax = 800;
  h_HighestPtLepton = fs->make<TH1F>("Leading Lepton pT", "p_{t}", ptOrderResolution,  0., ptOrderMax);
  h_SecondHighestPtLepton = fs->make<TH1F>("Second Leading Lepton pT ", "p_{t}", ptOrderResolution,  0., ptOrderMax);
  h_ThirdHighestPtLepton = fs->make<TH1F>("Third Leading Lepton pT", "p_{t}", ptOrderResolution,  0., ptOrderMax);
  h_FourthHighestPtLepton = fs->make<TH1F>("Fourth Leading Lepton pT", "p_{t}", ptOrderResolution,  0., ptOrderMax);

  //Dilepton Signals
  h_DimuonPt = fs->make<TH1F>("Dimuon pt", "Dimuon pt", 500, 0, ptMaxRange);
  h_DimuonEta = fs->make<TH1F>("Dimuon eta", "Dimuon #eta", 100, -5, 5);
  h_DimuonPhi = fs->make<TH1F>("Dimuon phi", "Dimuon #phi", 100, -3.15, 3.15);

  h_DimuonMinusMassInvar = fs->make<TH1F>("Dimuon Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);
  h_DimuonPlusMassInvar = fs->make<TH1F>("Diantimuon Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);

  h_DielectronPt = fs->make<TH1F>("Dielectron pt", "Dielectron pt", 500, 0, ptMaxRange);
  h_DielectronEta = fs->make<TH1F>("Dielectron eta", "Dielectron #eta", 100, -5, 5);
  h_DielectronPhi = fs->make<TH1F>("Dielectron phi", "Dielectron #phi", 100, -3.15, 3.15);

  h_DielectronMinusMassInvar = fs->make<TH1F>("Dielectron Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);
  h_DielectronPlusMassInvar = fs->make<TH1F>("Dipositron Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);

  //General Dilepton Signal
  h_DileptonMinusMassInvar = fs->make<TH1F>("Dilepton- Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);
  h_DileptonPlusMassInvar = fs->make<TH1F>("Dilepton+ Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);

  //Opposite Sign Dileptons(OSDL) for missidentification
  h_DimuonPtOSDL = fs->make<TH1F>("Dimuon pt OSDL", "Dimuon pt", 500, 0, ptMaxRange);
  h_DimuonEtaOSDL = fs->make<TH1F>("Dimuon eta OSDL", "Dimuon #eta", 100, -5, 5);
  h_DimuonPhiOSDL = fs->make<TH1F>("Dimuon phi OSDL", "Dimuon #phi", 100, -3.15, 3.15);
  h_DimuonMassInvarOSDL = fs->make<TH1F>("Dimuon Invariant mass OSDL", "Invariant mass", 100, 0., massInvarMaxRange);

  h_DielectronPtOSDL = fs->make<TH1F>("Dielectron pt OSDL", "Dielectron pt", 500, 0, ptMaxRange);
  h_DielectronEtaOSDL = fs->make<TH1F>("Dielectron eta OSDL", "Dielectron #eta", 100, -5, 5);
  h_DielectronPhiOSDL = fs->make<TH1F>("Dielectron phi OSDL", "Dielectron #phi", 100, -3.15, 3.15);
  h_DielectronMassInvarOSDL = fs->make<TH1F>("Dielectron Invariant mass OSDL", "Invariant mass", 100, 0., massInvarMaxRange);
  
  //General
  h_dPhi = fs->make<TH1F>("delta phi", "#delta #phi", 100, -3.15, 3.15 );//Original
  h_dEta = fs->make<TH1F>("delta eta", "#delta #eta", 100, -5, 5);
  h_dTheta = fs->make<TH1F>("delta theta", "#delta #theta", 100, -3.15, 3.15);//Original
  h_dR = fs->make<TH1F>("delta r", "#delta r", 100, 0, 10);

  h_massInvar = fs->make<TH1F>("Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange);
  h_massInvarZZBackground  = fs->make<TH1F>("Invariant mass ZZ background", "Invariant mass", 100, 0., massInvarMaxRange);
  h_FourLeptonMassInvar = fs->make<TH1F>("Four Lepton Invariant mass", "Invariant mass", 100, 0., massInvarMaxRange*2);
  
  h_cosTheta = fs->make<TH1F>("cosTheta", "cos #theta", 100, -1.01, 1.01);
  h_tanPhi = fs->make<TH1F>("tanPhi", "tan #phi", 100, -1000.0, 1000.0);
  h_csTheta = fs->make<TH1F>("csTheta", "#theta_{CS}", 100, -3.15, 3.15);
  h_csPhi = fs->make<TH1F>("csPhi", "#phi_{CS}", 100, -3.15, 3.15);

  h_cosThetaMinusInvariantMass = fs->make<TH1F>("InvariantMass cosThetaMinus", "InvariantMass cosThetaMinus", 350, 0., 3500.);
  h_cosThetaPlusInvariantMass = fs->make<TH1F>("InvariantMass cosThetaPlus", "InvariantMass cosThetaPlus", 350, 0., 3500.);

  h2_pt1_vs_pt2 = fs->make<TH2F>("pt1_vs_pt2", "p_{t,1} vs. p_{t,2}", 500, 0., 2500., 500, 0., 2500.);
  h2_eta1_vs_eta2 = fs->make<TH2F>("eta1_vs_eta2", "#eta_{1} vs. #eta_{2}", 100, -5., 5., 100, -5., 5.);
  h2_phi1_vs_phi2 = fs->make<TH2F>("phi1_vs_phi2", "#phi_{1} vs. #phi_{2}", 100, -3.15, 3.15, 100, -3.15, 3.15);

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
DimuonDarkPhoton::DimuonDarkPhoton(const edm::ParameterSet& iConfig)

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


DimuonDarkPhoton::~DimuonDarkPhoton()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DimuonDarkPhoton::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

  //makes reco candidate variables for the different types of particles
  const reco::Candidate* higgsPlus = nullptr;
  const reco::Candidate* higgsMinus = nullptr;
  const reco::Candidate* higgsPlusRight = nullptr;
  const reco::Candidate* higgsMinusRight = nullptr;

  //Convenience before classification
  const reco::Candidate* daughter1 = nullptr;
  const reco::Candidate* daughter2 = nullptr;
  //const reco::Candidate* mother1 = nullptr;
  //const reco::Candidate* mother2 = nullptr;

  //Classification variables
  const reco::Candidate* mu1Minus  = nullptr;
  const reco::Candidate* mu1Plus  = nullptr;
  const reco::Candidate* mu2Minus  = nullptr;
  const reco::Candidate* mu2Plus  = nullptr;

  const reco::Candidate* ele1Minus  = nullptr;
  const reco::Candidate* ele1Plus  = nullptr;
  const reco::Candidate* ele2Minus  = nullptr;
  const reco::Candidate* ele2Plus  = nullptr;

  //ZZ background
  const reco::Candidate* zBoson1 = nullptr;
  const reco::Candidate* zBoson2 = nullptr;

  //printGenParticleCollection(genParts);
  //std::exit(1);//uncomment if only printing one particle collection

  //loop through gen particles for four lepton H++ and H-- analysis
  for(auto &part : genParts){
    //if the particle is H++ or H--
    if(abs(part.pdgId()) == 9900041 || abs(part.pdgId()) == 9900042) {
     // std::cout << "Found Higgs at line " << __LINE__ << std::endl;
      bool foundOriginalHiggs = false;
      if(!higgsPlus && part.pdgId() == 9900041) {//assuming only one pair of higgs are produces
	higgsPlus = getLastDaughter(&part, part.pdgId());//To get the latest daughter of itself
	//std::cout << "Found last HiggsL++ at line " << __LINE__ << std::endl;
	daughter1 = getLastDaughter(higgsPlus->daughter(0), higgsPlus->daughter(0)->pdgId());
	//std::cout << "Found last daughter1 at line " << __LINE__ << std::endl;
	daughter2 = getLastDaughter(higgsPlus->daughter(1), higgsPlus->daughter(1)->pdgId());
	//std::cout << "Found last daughter2 at line " << __LINE__ << std::endl;
	foundOriginalHiggs = true;
      }
      else if(!higgsMinus && part.pdgId() == -9900041) {
	higgsMinus = getLastDaughter(&part, part.pdgId());//To get the latest daughter of itself
	//std::cout << "Found last HiggsL-- at line " << __LINE__ << std::endl;
	daughter1 = getLastDaughter(higgsMinus->daughter(0), higgsMinus->daughter(0)->pdgId());
	//std::cout << "Found last daughter1 at line " << __LINE__ << std::endl;
	daughter2 = getLastDaughter(higgsMinus->daughter(1), higgsMinus->daughter(1)->pdgId());
	//std::cout << "Found last daughter2 at line " << __LINE__ << std::endl;
	foundOriginalHiggs = true;
      }
      else if(!higgsPlusRight && part.pdgId() == 9900042) {
	higgsPlusRight = getLastDaughter(&part, part.pdgId());//To get the latest daughter of itself
	//std::cout << "Found last HiggsR++ at line " << __LINE__ << std::endl;
	daughter1 = getLastDaughter(higgsPlusRight->daughter(0), higgsPlusRight->daughter(0)->pdgId());
	//std::cout << "Found last daughter1 at line " << __LINE__ << std::endl;
	daughter2 = getLastDaughter(higgsPlusRight->daughter(1), higgsPlusRight->daughter(1)->pdgId());
	//std::cout << "Found last daughter2 at line " << __LINE__ << std::endl;
	foundOriginalHiggs = true;
      }
      else if(!higgsMinusRight && part.pdgId() == -9900042) {
	higgsMinusRight = getLastDaughter(&part, part.pdgId());//To get the latest daughter of itself
	//std::cout << "Found last HiggsR-- at line " << __LINE__ << std::endl;
	daughter1 = getLastDaughter(higgsMinusRight->daughter(0), higgsMinusRight->daughter(0)->pdgId());
	//std::cout << "Found last daughter1 at line " << __LINE__ << std::endl;
	daughter2 = getLastDaughter(higgsMinusRight->daughter(1), higgsMinusRight->daughter(1)->pdgId());
	//std::cout << "Found last daughter2 at line " << __LINE__ << std::endl;
	foundOriginalHiggs = true;
      }
      

      if(foundOriginalHiggs) {
	//Sort daughters by pt for later use
	if(daughter1->pt() < daughter2->pt()) {
	  const reco::Candidate* tempDaughter1 = daughter1;
	  daughter1 = daughter2;
	  daughter2 = tempDaughter1;
	}
	
	//No need for switch statement becuase only 3 possible leptons m,e,t
	//tau is suppressed accordingn to ALTAS 2018 search for doubly charged Higgs boson in multi-lepton final states
	if(abs(daughter1->pdgId()) == 13 && abs(daughter2->pdgId()) == 13) {
	  //std::cout << "Found muon pair at line " << __LINE__ << std::endl;
	  if(daughter1->pdgId() < 0) {//Same sign leptons
	    mu1Plus = daughter1;
	    mu2Plus = daughter2;
	  }
	  else {
	    mu1Minus = daughter1;
	    mu2Minus = daughter2;
	  }
	}
	else if(abs(daughter1->pdgId()) == 11 && abs(daughter2->pdgId()) == 11) {
	  //std::cout << "Found electron pair at line " << __LINE__ << std::endl;
	  if(daughter1->pdgId() < 0) {//Same sign leptons
	    ele1Plus = daughter1;
	    ele2Plus = daughter2;
	  }
	  else {
	    ele1Minus = daughter1;
	    ele2Minus = daughter2;
	  }
	}
	else if((abs(daughter1->pdgId()) == 13 && abs(daughter2->pdgId()) == 11)
		|| (abs(daughter1->pdgId()) == 11 && abs(daughter2->pdgId()) == 13)) {
	  std::cout << "Differing flavour lepton pair of electron and muon found at line " << __LINE__
		    << " pdf IDs:\n" << daughter1->pdgId() << "\n" << daughter2->pdgId() 
		    << std::endl;
	  //std::exit(1);//Such events exist
	}
	else {
	  std::cout << "Differing flavour lepton pair found at line " << __LINE__
		    << " pdf IDs:\n" << daughter1->pdgId() << "\n" << daughter2->pdgId() << std::endl;
	}
      }
    }
    
    //ZZ background
    if(part.pdgId() == 23) {
      if(!zBoson1 && zBoson1 != getLastDaughter(&part, part.pdgId())) {
	zBoson1 = getLastDaughter(&part, part.pdgId());
	//std::cout << "Found last Z boson1 at line " << __LINE__ << "at index: " << getIndexOf(zBoson1,genParts) << std::endl;
      }
      else if(!zBoson2 && zBoson2 != getLastDaughter(&part, part.pdgId())) {
	zBoson2 = getLastDaughter(&part, part.pdgId());
	//std::cout << "Found last Z boson2 at line " << __LINE__ << "at index: " << getIndexOf(zBoson2,genParts) << std::endl;
      }
    }
  }  

  //Begin Filling Histograms
  //std::cout << "Begin Filling Histograms at line " << __LINE__ << std::endl;

  //Doubly Charged Higgs Histograms
  //It is assumed that if there is a higgsPlus, then there is a higgsMinus due to feynman diagram.
  //Left Higgs
  if(higgsPlus) {
    //std::cout << "Filling HiggsL++ at line " << __LINE__ << std::endl;
    h_HigPlusMass->Fill(higgsPlus->mass());
    h_HigPlusPt->Fill(higgsPlus->pt());
    h_HigPlusEta->Fill(higgsPlus->eta());
    h_HigPlusPhi->Fill(higgsPlus->phi());
    h_HigPlusCharge->Fill(higgsPlus->charge());
    
    //std::cout << "Filling HiggsL-- at line " << __LINE__ << std::endl;
    h_HigMinusMass->Fill(higgsMinus->mass());
    h_HigMinusPt->Fill(higgsMinus->pt());
    h_HigMinusEta->Fill(higgsMinus->eta());
    h_HigMinusPhi->Fill(higgsMinus->phi());
    h_HigMinusCharge->Fill(higgsMinus->charge());
  }

  //Right Higgs
  if(higgsPlusRight) {
    //std::cout << "Filling HiggsR++ at line " << __LINE__ << std::endl;
    h_HigPlusMass->Fill(higgsPlusRight->mass());
    h_HigPlusPt->Fill(higgsPlusRight->pt());
    h_HigPlusEta->Fill(higgsPlusRight->eta());
    h_HigPlusPhi->Fill(higgsPlusRight->phi());
    h_HigPlusCharge->Fill(higgsPlusRight->charge());
    
    //std::cout << "Filling HiggsR-- at line " << __LINE__ << std::endl;
    //It is assumed that if there is a higgsPlus, then there is a higgsMinus due to feynman diagram.
    h_HigMinusMass->Fill(higgsMinusRight->mass());
    h_HigMinusPt->Fill(higgsMinusRight->pt());
    h_HigMinusEta->Fill(higgsMinusRight->eta());
    h_HigMinusPhi->Fill(higgsMinusRight->phi());
    h_HigMinusCharge->Fill(higgsMinusRight->charge());
  }
  

  //Histograms for individual particles
  //Muon
  if(mu1Minus) {
    //std::cout << "Filling muon1 at line " << __LINE__ << std::endl;
    h_MuMinusMass->Fill(mu1Minus->mass());
    h_MuMinusPt->Fill(mu1Minus->pt());
    h_MuMinusEta->Fill(mu1Minus->eta());
    h_MuMinusPhi->Fill(mu1Minus->phi());
    h_thetaMuMinus->Fill(mu1Minus->theta());

    //std::cout << "Filling muon1 as high at line " << __LINE__ << std::endl;
    h_MuMinusPt1->Fill(mu1Minus->pt());
    h_MuMinusEta1->Fill(mu1Minus->eta());
    h_MuMinusPhi1->Fill(mu1Minus->phi());
    //h_thetaMuMinus1->Fill(mu1Minus->theta());

    //std::cout << "Filling muon2 at line " << __LINE__ << std::endl;
    h_MuMinusMass->Fill(mu2Minus->mass());
    h_MuMinusPt->Fill(mu2Minus->pt());
    h_MuMinusEta->Fill(mu2Minus->eta());
    h_MuMinusPhi->Fill(mu2Minus->phi());
    h_thetaMuMinus->Fill(mu2Minus->theta());

    //std::cout << "Filling muon2 as low at line " << __LINE__ << std::endl;
    h_MuMinusPt2->Fill(mu2Minus->pt());
    h_MuMinusEta2->Fill(mu2Minus->eta());
    h_MuMinusPhi2->Fill(mu2Minus->phi());
    //h_thetaMuMinus2->Fill(mu2Minus->theta());
  }

  //Antimuon
  if(mu1Plus) {
    //std::cout << "Filling antimuon1 at line " << __LINE__ << std::endl;
    h_MuPlusMass->Fill(mu1Plus->mass());
    h_MuPlusPt->Fill(mu1Plus->pt());
    h_MuPlusEta->Fill(mu1Plus->eta());
    h_MuPlusPhi->Fill(mu1Plus->phi());
    h_thetaMuPlus->Fill(mu1Plus->theta());

    //std::cout << "Filling antimuon1 as high at line " << __LINE__ << std::endl;
    h_MuPlusPt1->Fill(mu1Plus->pt());
    h_MuPlusEta1->Fill(mu1Plus->eta());
    h_MuPlusPhi1->Fill(mu1Plus->phi());
    //h_thetaMuPlus1->Fill(mu1Plus->theta());

    //std::cout << "Filling antimuon2 at line " << __LINE__ << std::endl;
    h_MuPlusMass->Fill(mu2Plus->mass());
    h_MuPlusPt->Fill(mu2Plus->pt());
    h_MuPlusEta->Fill(mu2Plus->eta());
    h_MuPlusPhi->Fill(mu2Plus->phi());
    h_thetaMuPlus->Fill(mu2Plus->theta());

    //std::cout << "Filling antimuon2 as low at line " << __LINE__ << std::endl;
    h_MuPlusPt2->Fill(mu2Plus->pt());
    h_MuPlusEta2->Fill(mu2Plus->eta());
    h_MuPlusPhi2->Fill(mu2Plus->phi());
    //h_thetaMuPlus2->Fill(mu2Plus->theta());
  }
  
  //Electron
  if(ele1Minus) {
    //std::cout << "Filling electron1 at line " << __LINE__ << std::endl;
    h_ElectMinusMass->Fill(ele1Minus->mass());
    h_ElectMinusPt->Fill(ele1Minus->pt());
    h_ElectMinusEta->Fill(ele1Minus->eta());
    h_ElectMinusPhi->Fill(ele1Minus->phi());
    h_thetaElectMinus->Fill(ele1Minus->theta());

    //std::cout << "Filling electron1 as high at line " << __LINE__ << std::endl;
    h_ElectMinusPt1->Fill(ele1Minus->pt());
    h_ElectMinusEta1->Fill(ele1Minus->eta());
    h_ElectMinusPhi1->Fill(ele1Minus->phi());
    //h_thetaElectMinus1->Fill(ele1Minus->theta());
    
    //std::cout << "Filling electron2 at line " << __LINE__ << std::endl;
    h_ElectMinusMass->Fill(ele2Minus->mass());
    h_ElectMinusPt->Fill(ele2Minus->pt());
    h_ElectMinusEta->Fill(ele2Minus->eta());
    h_ElectMinusPhi->Fill(ele2Minus->phi());
    h_thetaElectMinus->Fill(ele2Minus->theta());

    //std::cout << "Filling electron2 as low at line " << __LINE__ << std::endl;
    h_ElectMinusPt2->Fill(ele2Minus->pt());
    h_ElectMinusEta2->Fill(ele2Minus->eta());
    h_ElectMinusPhi2->Fill(ele2Minus->phi());
    //h_thetaElectMinus2->Fill(ele2Minus->theta());
  }

  //Positron
  if(ele1Plus) {
    //std::cout << "Filling positron1 at line " << __LINE__ << std::endl;
    h_ElectPlusMass->Fill(ele1Plus->mass());
    h_ElectPlusPt->Fill(ele1Plus->pt());
    h_ElectPlusEta->Fill(ele1Plus->eta());
    h_ElectPlusPhi->Fill(ele1Plus->phi());
    h_thetaElectPlus->Fill(ele1Plus->theta());

    //std::cout << "Filling positron1 as high at line " << __LINE__ << std::endl;
    h_ElectPlusPt1->Fill(ele1Plus->pt());
    h_ElectPlusEta1->Fill(ele1Plus->eta());
    h_ElectPlusPhi1->Fill(ele1Plus->phi());
    //h_thetaElectPlus1->Fill(ele1Plus->theta());
    
    //std::cout << "Filling positron2 at line " << __LINE__ << std::endl;
    h_ElectPlusMass->Fill(ele2Plus->mass());
    h_ElectPlusPt->Fill(ele2Plus->pt());
    h_ElectPlusEta->Fill(ele2Plus->eta());
    h_ElectPlusPhi->Fill(ele2Plus->phi());
    h_thetaElectPlus->Fill(ele2Plus->theta());

    //std::cout << "Filling positron2 as low at line " << __LINE__ << std::endl;
    h_ElectPlusPt2->Fill(ele2Plus->pt());
    h_ElectPlusEta2->Fill(ele2Plus->eta());
    h_ElectPlusPhi2->Fill(ele2Plus->phi());
    //h_thetaElectPlus2->Fill(ele2Plus->theta());
  }

  //Iterative data structure for daughters
  const reco::Candidate* dileptons[] {mu1Plus, mu2Plus, mu1Minus, mu2Minus,
                                      ele1Plus, ele2Plus, ele1Minus, ele2Minus};

  //Particles to Analyze
  //std::cout << "Four Lepton Output at line " << __LINE__ << std::endl;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i]) {
      std::cout << dileptons[i]->pdgId() << ", ";
    }
    else {
      std::cout << "0, ";
    }
  }
  std::cout << std::endl;

  //Aanalyze all methods and determine OSDLs
  fillOSDLHistograms(dileptons);

  //Calculating general data for lepton pairs 
  for(int i = 0; i < 8; i += 2) {
    if(dileptons[i]) {
      const reco::Candidate* lepton1 = dileptons[i];
      const reco::Candidate* lepton2 = dileptons[i + 1];
      std::cout << "Found Lepton Pair at line " << __LINE__  
		<< "\nlepton1 pointer " << lepton1
		<< "\nlepton2 pointer " << lepton2 << std::endl;
      
      //calculating the chi values?
      double lepton1KPlus = (1/sqrt(2))*(lepton1->energy() + lepton1->pz());
      double lepton1KMinus = (1/sqrt(2))*(lepton1->energy() - lepton1->pz());
      double lepton2KPlus = (1/sqrt(2))*(lepton2->energy() + lepton2->pz());
      double lepton2KMinus = (1/sqrt(2))*(lepton2->energy() - lepton2->pz());

      double invariantK = (lepton1KPlus*lepton2KMinus - lepton2KPlus*lepton1KMinus);
      
      math::XYZTLorentzVectorD dilepton = lepton1->p4() + lepton2->p4();

      double pseudorapidity = asinh(dilepton.pz()/dilepton.pt());
      //std::cout << "Lepton Pair pseudorapidity is " << pseudorapidity << " eta at line " << __LINE__  << std::endl;

      double phi = acos(dilepton.px()/dilepton.pt());
      //std::cout << "Lepton Pair phi is " << phi << " phi at line " << __LINE__  << std::endl;

      //Decide if to put into seperate histograms
      if(abs(lepton1->pdgId()) == 13) {
	h_DimuonPt->Fill(dilepton.pt());
	h_DimuonEta->Fill(pseudorapidity);
	h_DimuonPhi->Fill(phi);
	//std::cout << "Lepton Pair is muon pair at line " << __LINE__  << std::endl;
      }
      else if(abs(lepton1->pdgId()) == 11) {
	h_DielectronPt->Fill(dilepton.pt());
	h_DielectronEta->Fill(pseudorapidity);
	h_DielectronPhi->Fill(phi);
	//std::cout << "Lepton Pair is electron pair at line " <<  __LINE__  << std::endl;
      }

      double dileptonQ = sqrt(pow(dilepton.energy(),2) - pow(dilepton.pt(),2) - pow(dilepton.pz(),2));


      double denominatorTheta = dileptonQ*sqrt(pow(dileptonQ, 2) + pow(dilepton.pt(), 2));
      double thetaCos = (dilepton.pz()/fabs(dilepton.pz()))*(2/denominatorTheta)*invariantK;
      double thetaCS = acos(thetaCos);

      double denominatorPhi1 = dileptonQ*dilepton.pt();
      double numeratorPhi1 = sqrt(pow(dileptonQ, 2) + pow(dilepton.pt(), 2));
      double deltaX = lepton1->px() - lepton2->px();
      double deltaY = lepton1->py() - lepton2->py();
      double denominatorPhi2 = ((deltaX*dilepton.px()) + (deltaY*dilepton.py()));
      double numeratorPhi2 = ((deltaX*dilepton.py()) + (deltaY*dilepton.px()));
      double numeratorPhi = numeratorPhi1 * numeratorPhi2;
      double denominatorPhi = denominatorPhi1 * denominatorPhi2;

      double phiTan = numeratorPhi/denominatorPhi;
      double phiCS = atan(phiTan);
      
      /*
      double cosTheta = thetaCos;
      double tanPhi = phiTan;
      double csTheta = thetaCS;  NOT USED
      double csPhi = phiCS;
      */

      h_cosTheta->Fill(thetaCos);
      h_csTheta->Fill(thetaCS);
      h_tanPhi->Fill(phiTan);
      h_csPhi->Fill(phiCS);

      double invariantMass = sqrt(2 * lepton1->pt() * lepton2->pt() *(cosh(lepton1->eta() - lepton2->eta()) - cos(TVector2::Phi_mpi_pi(lepton1->phi() - lepton2->phi()))));
      h_massInvar->Fill(invariantMass);
      //std::cout << "Lepton Pair invariant mass is " << invariantMass << " GeV at line " << __LINE__  << std::endl;
      //h_massInvarWithZZBackground->Fill(invariantMass);
      
      //Dilepton invariant mass per values
      //Muons
      if(abs(lepton1->pdgId()) == 13) {
	if(lepton1->charge() > 0) {
	  h_DimuonPlusMassInvar->Fill(invariantMass);
	  h_DileptonPlusMassInvar->Fill(invariantMass);
	}
	else {
	  h_DimuonMinusMassInvar->Fill(invariantMass);
	  h_DileptonMinusMassInvar->Fill(invariantMass);
	}
      }
      //Electrons
      else if(abs(lepton1->pdgId()) == 11) {
	if(lepton1->charge() > 0) {
	  h_DielectronPlusMassInvar->Fill(invariantMass);
	  h_DileptonPlusMassInvar->Fill(invariantMass);
	}
	else {
	  h_DielectronMinusMassInvar->Fill(invariantMass);
	  h_DileptonMinusMassInvar->Fill(invariantMass);
	}
      }
      

      if(thetaCos < 0.0){
	h_cosThetaMinusInvariantMass->Fill(invariantMass);
	mCosThetaMinus = invariantMass;
      }
      else{
	h_cosThetaPlusInvariantMass->Fill(invariantMass);
	mCosThetaPlus = invariantMass;
      }

      //Other
      h_dPhi->Fill(TVector2::Phi_mpi_pi(lepton2->phi() - lepton1->phi()));
      std::cout << "Lepton Pair dPhi is " 
		<< TVector2::Phi_mpi_pi(lepton2->phi() - lepton1->phi()) 
		<< " delta phi at line " << __LINE__  
		<< " " << lepton2->phi() << " " << lepton1->phi() << std::endl;

      h_dEta->Fill(lepton2->eta() - lepton1->eta());
      std::cout << "Lepton Pair dEta is " 
		<< TVector2::Phi_mpi_pi(lepton2->phi() - lepton1->phi()) 
		<< " delta eta at line " << __LINE__  
		<< " " << lepton2->eta() << " " << lepton1->eta() << std::endl;

      h_dTheta->Fill(TVector2::Phi_mpi_pi(lepton2->theta() - lepton1->theta()));
      std::cout << "Lepton Pair dTheta is " 
		<< TVector2::Phi_mpi_pi(lepton2->theta() - lepton1->theta()) 
		<< " delta theta at line " << __LINE__  
		<< " " << lepton2->theta() << " " << lepton1->theta() << std::endl;

      h_dR->Fill(reco::deltaR(lepton2->p4(), lepton1->p4()));
      std::cout << "Lepton Pair dR is " 
		<< reco::deltaR(lepton2->p4(), lepton1->p4()) 
		<< " delta r at line " << __LINE__  << std::endl;

      h2_phi1_vs_phi2->Fill(lepton2->phi(), lepton1->phi());  
      h2_eta1_vs_eta2->Fill(lepton2->eta(), lepton1->eta());
      h2_pt1_vs_pt2->Fill(lepton2->pt(), lepton1->pt());
    }
  }

  //Calculating Four lepton invariant mass
  int numberOfLeptons = 0;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i] != nullptr) {
      numberOfLeptons++;
    }
  }
  if(numberOfLeptons == 4) {
    math::XYZTLorentzVectorD fourLeptonInvariantMass;
    bool foundFirstValue = false;
    for(int i = 0; i < 8; i++) {
      if(dileptons[i]) {
	if(!foundFirstValue) {
	  fourLeptonInvariantMass = dileptons[i]->p4();
	  foundFirstValue = true;
	}
	else {
	  fourLeptonInvariantMass = fourLeptonInvariantMass + dileptons[i]->p4();
	}
      }
    }
    h_FourLeptonMassInvar->Fill(fourLeptonInvariantMass.mass());
  }

  //Calculating highest pt lepton from lepton pairs
  double highestPt = -1;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i] != nullptr) {
      if(dileptons[i]->pt() > highestPt) {
	highestPt = dileptons[i]->pt();
      }
    }
  }
  if(highestPt > 0) {
    h_HighestPtLepton->Fill(highestPt);
    std::cout << "The highest pt lepton is " << highestPt << " GeV at line " << __LINE__ << std::endl;
  }
  else {
    std::cout << "Some other particle (tau) decayed from higgs at line " << __LINE__ << std::endl;
  }

  //Calculating the second highest pt lepton from lepton pairs
  double secondHighestPt = -1;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i] != nullptr) {
      if(dileptons[i]->pt() < highestPt && dileptons[i]->pt() > secondHighestPt) {
	secondHighestPt = dileptons[i]->pt();
      }
    }
  }
  if(secondHighestPt > 0) {
    h_SecondHighestPtLepton->Fill(secondHighestPt);
    std::cout << "The second highest pt lepton is " << secondHighestPt << " GeV at line " << __LINE__ << std::endl;
  }

  //Calculating the third highest pt lepton from lepton pairs
  double thirdHighestPt = -1;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i] != nullptr) {
      if(dileptons[i]->pt() < secondHighestPt && dileptons[i]->pt() > thirdHighestPt) {
	thirdHighestPt = dileptons[i]->pt();
      }
    }
  }
  if(thirdHighestPt > 0) {
    h_ThirdHighestPtLepton->Fill(thirdHighestPt);
    std::cout << "The third highest pt lepton is " << thirdHighestPt << " GeV at line " << __LINE__ << std::endl;
  }

  //Calculating the fourth highest pt lepton from lepton pairs
  double fourthHighestPt = -1;
  for(int i = 0; i < 8; i++) {
    if(dileptons[i] != nullptr) {
      if(dileptons[i]->pt() < thirdHighestPt && dileptons[i]->pt() > fourthHighestPt) {
	fourthHighestPt = dileptons[i]->pt();
      }
    }
  }
  if(fourthHighestPt > 0) {
    h_FourthHighestPtLepton->Fill(fourthHighestPt);
    std::cout << "The fourth highest pt lepton is " << fourthHighestPt << " GeV at line " << __LINE__ << std::endl;
  }


  //ZZ background
  if(zBoson1) {
    daughter1 = getLastDaughter(zBoson1->daughter(0), zBoson1->daughter(0)->pdgId());
    daughter2 = getLastDaughter(zBoson1->daughter(1), zBoson1->daughter(1)->pdgId());

    std::cout << "daughter1 is " << daughter1->pdgId() << " at line " << __LINE__ << " with Index " << getIndexOf(daughter1,genParts) << std::endl;
    std::cout << "daughter2 is " << daughter2->pdgId() << " at line " << __LINE__ << " with Index " << getIndexOf(daughter2,genParts) << std::endl;
    
    double invariantMass = sqrt(2 * daughter1->pt() * daughter2->pt() *(cosh(daughter1->eta() - daughter2->eta()) - cos(TVector2::Phi_mpi_pi(daughter1->phi() - daughter2->phi()))));
    h_massInvarZZBackground->Fill(invariantMass);
    std::cout << "zBoson1 invariantMass is " << invariantMass << " GeV at line " << __LINE__ << std::endl;
  }

  if(zBoson2) {
    daughter1 = getLastDaughter(zBoson2->daughter(0), zBoson2->daughter(0)->pdgId());
    daughter2 = getLastDaughter(zBoson2->daughter(1), zBoson2->daughter(1)->pdgId());

    std::cout << "daughter1 is " << daughter1->pdgId() << " at line " << __LINE__ << " with Index " << getIndexOf(daughter1,genParts) << std::endl;
    std::cout << "daughter2 is " << daughter2->pdgId() << " at line " << __LINE__ << " with Index " << getIndexOf(daughter2,genParts) << std::endl;
    
    double invariantMass = sqrt(2 * daughter1->pt() * daughter2->pt() *(cosh(daughter1->eta() - daughter2->eta()) - cos(TVector2::Phi_mpi_pi(daughter1->phi() - daughter2->phi()))));
    h_massInvarZZBackground->Fill(invariantMass);
    std::cout << "zBoson2 invariantMass is " << invariantMass << " GeV at line " << __LINE__ << std::endl;
  }

  std::cout << "\n\n===========================================================================================================" << std::endl;
  tree_->Fill();  
}

bool DimuonDarkPhoton::isBoson(int pid)
{
  if(pid==23 || abs(pid)==22 || pid==32){
    if(debug_ > 0) std::cout << "\n\nFound Boson\n";
    return true;
  }
  else return false;
}

bool DimuonDarkPhoton::isMuon(int pid){
  if(abs(pid)==11 || abs(pid) ==13){
    if(debug_ > 0) std::cout << "\n\nFound A Muon!\n";
    return true;
  }
  else return false;
}

bool DimuonDarkPhoton::checkBosonStatus( const reco::GenParticleCollection& genParts){
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

const reco::Candidate* DimuonDarkPhoton::getBoson( const reco::GenParticleCollection& genParts)
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
const reco::Candidate* DimuonDarkPhoton::getLastDaughter(const reco::Candidate* part, int pid)
{
  for(size_t partNr = 0; part && partNr < part->numberOfDaughters(); partNr++){
    if(part->daughter(partNr)->pdgId() == pid)
      return getLastDaughter(part->daughter(partNr),pid);
  }
  return part;
}
       
const reco::Candidate* DimuonDarkPhoton::getDaughter(const reco::Candidate* part,int pid)
{  
  for(size_t partNr = 0; part && partNr<part->numberOfDaughters(); partNr++)
  {
    if(part->daughter(partNr)->pdgId() == pid) return part->daughter(partNr);
  }
  return nullptr;
}

const reco::Candidate* DimuonDarkPhoton::getMother(const reco::Candidate* part, int pid)
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

const int DimuonDarkPhoton::getIndexOf(const reco::Candidate* part, const reco::GenParticleCollection& genParts)
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
const void DimuonDarkPhoton::printGenParticleCollection(const reco::GenParticleCollection& genParts)
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
const void DimuonDarkPhoton::fillOSDLHistograms(const reco::Candidate* dileptons[])
{
  //Assumed that OSDL cannot be made from flavour mixing. 
  //Assumed Lepton Order in array: mu1Plus, mu2Plus, mu1Minus, mu2Minus,
                                 //ele1Plus, ele2Plus, ele1Minus, ele2Minus

  int startingIndex = -1;
  int numberOfConsecutiveLeptons = 0;
  for(int i = 0; i < 2; i++) {//Maybe change to while loop in future
    numberOfConsecutiveLeptons = 0;
    for(int x = i*4; x < 4*(i+1); x++) {
      if(dileptons[x] && numberOfConsecutiveLeptons == (x % 4)) {
	numberOfConsecutiveLeptons++;
      }
      if(numberOfConsecutiveLeptons == 4) {
	startingIndex = i * 4;
	std::cout << "Found SSDL pair with same flavour to apply as OSDL at line " << __LINE__ << std::endl;
      }
    }
  }
  //std::cout << "The starting index is " << startingIndex
	   // << " at line " << __LINE__ << std::endl;

  if(startingIndex >= 0) {
    //std::cout << "Begin filling OSDL Histograms at line " << __LINE__ << std::endl;
    //Opposite sign leptons are paired by what order they are in terms of pt
    const reco::Candidate* lepton1Plus = dileptons[startingIndex];
    const reco::Candidate* lepton2Plus = dileptons[startingIndex+1];
    const reco::Candidate* lepton1Minus = dileptons[startingIndex+2];
    const reco::Candidate* lepton2Minus = dileptons[startingIndex+3];

    //Plus
    math::XYZTLorentzVectorD dileptonPlus = lepton1Plus->p4() + lepton1Minus->p4();

    double pseudorapidityPlus = asinh(dileptonPlus.pz()/dileptonPlus.pt());
    //std::cout << "Lepton Pair Plus pseudorapidity is " << pseudorapidityPlus << " eta at line " << __LINE__  << std::endl;

    double phiPlus = acos(dileptonPlus.px()/dileptonPlus.pt());
    //std::cout << "Lepton Pair Plus phi is " << phiPlus << " phi at line " << __LINE__  << std::endl;

    double dileptonPlusInvariantMass = sqrt(2 * lepton1Plus->pt() * lepton1Minus->pt() *(cosh(lepton1Plus->eta() - lepton1Minus->eta()) - cos(TVector2::Phi_mpi_pi(lepton1Plus->phi() - lepton1Minus->phi()))));
    
    //Minus
    math::XYZTLorentzVectorD dileptonMinus = lepton2Plus->p4() + lepton2Minus->p4();

    double pseudorapidityMinus = asinh(dileptonMinus.pz()/dileptonMinus.pt());
    //std::cout << "Lepton Pair Minus pseudorapidity is " << pseudorapidityMinus << " eta at line " << __LINE__  << std::endl;

    double phiMinus = acos(dileptonMinus.px()/dileptonMinus.pt());
    //std::cout << "Lepton Pair Minus phi is " << phiMinus << " phi at line " << __LINE__  << std::endl;

    double dileptonMinusInvariantMass = sqrt(2 * lepton2Plus->pt() * lepton2Minus->pt() *(cosh(lepton2Plus->eta() - lepton2Minus->eta()) - cos(TVector2::Phi_mpi_pi(lepton2Plus->phi() - lepton2Minus->phi()))));
    
    if(abs(lepton1Plus->pdgId()) == 13) {
      //std::cout << "Filling OSDL Muon Plus at line " << __LINE__ << std::endl;
      h_MuPlusMassOSDL->Fill(lepton1Plus->mass());
      h_MuPlusMassOSDL->Fill(lepton1Minus->mass());
      h_MuPlusPtOSDL->Fill(lepton1Plus->pt());
      h_MuPlusPtOSDL->Fill(lepton1Minus->pt());
      h_MuPlusEtaOSDL->Fill(lepton1Plus->eta());
      h_MuPlusEtaOSDL->Fill(lepton1Minus->eta());
      h_MuPlusPhiOSDL->Fill(lepton1Plus->phi());
      h_MuPlusPhiOSDL->Fill(lepton1Minus->phi());
      h_thetaMuPlusOSDL->Fill(lepton1Plus->theta());
      h_thetaMuPlusOSDL->Fill(lepton1Minus->theta());

      //std::cout << "Filling OSDL Muon Minus at line " << __LINE__ << std::endl;
      h_MuMinusMassOSDL->Fill(lepton2Plus->mass());
      h_MuMinusMassOSDL->Fill(lepton2Minus->mass());
      h_MuMinusPtOSDL->Fill(lepton2Plus->pt());
      h_MuMinusPtOSDL->Fill(lepton2Minus->pt());
      h_MuMinusEtaOSDL->Fill(lepton2Plus->eta());
      h_MuMinusEtaOSDL->Fill(lepton2Minus->eta());
      h_MuMinusPhiOSDL->Fill(lepton2Plus->phi());
      h_MuMinusPhiOSDL->Fill(lepton2Minus->phi());
      h_thetaMuMinusOSDL->Fill(lepton2Plus->theta());
      h_thetaMuMinusOSDL->Fill(lepton2Minus->theta());

      h_DimuonPtOSDL->Fill(dileptonPlus.pt());
      h_DimuonEtaOSDL->Fill(pseudorapidityPlus);
      h_DimuonPhiOSDL->Fill(phiPlus);
      h_DimuonMassInvarOSDL->Fill(dileptonPlusInvariantMass);

      h_DimuonPtOSDL->Fill(dileptonMinus.pt());
      h_DimuonEtaOSDL->Fill(pseudorapidityMinus);
      h_DimuonPhiOSDL->Fill(phiMinus);
      h_DimuonMassInvarOSDL->Fill(dileptonMinusInvariantMass);
    }
    else if(abs(lepton1Plus->pdgId()) == 11) {
      //std::cout << "Filling OSDL Electron Plus at line " << __LINE__ << std::endl;
      h_ElectPlusMassOSDL->Fill(lepton1Plus->mass());
      h_ElectPlusMassOSDL->Fill(lepton1Minus->mass());
      h_ElectPlusPtOSDL->Fill(lepton1Plus->pt());
      h_ElectPlusPtOSDL->Fill(lepton1Minus->pt());
      h_ElectPlusEtaOSDL->Fill(lepton1Plus->eta());
      h_ElectPlusEtaOSDL->Fill(lepton1Minus->eta());
      h_ElectPlusPhiOSDL->Fill(lepton1Plus->phi());
      h_ElectPlusPhiOSDL->Fill(lepton1Minus->phi());
      h_thetaElectPlusOSDL->Fill(lepton1Plus->theta());
      h_thetaElectPlusOSDL->Fill(lepton1Minus->theta());

      //std::cout << "Filling OSDL Electron Minus at line " << __LINE__ << std::endl;
      h_ElectMinusMassOSDL->Fill(lepton2Plus->mass());
      h_ElectMinusMassOSDL->Fill(lepton2Minus->mass());
      h_ElectMinusPtOSDL->Fill(lepton2Plus->pt());
      h_ElectMinusPtOSDL->Fill(lepton2Minus->pt());
      h_ElectMinusEtaOSDL->Fill(lepton2Plus->eta());
      h_ElectMinusEtaOSDL->Fill(lepton2Minus->eta());
      h_ElectMinusPhiOSDL->Fill(lepton2Plus->phi());
      h_ElectMinusPhiOSDL->Fill(lepton2Minus->phi());
      h_thetaElectMinusOSDL->Fill(lepton2Plus->theta());
      h_thetaElectMinusOSDL->Fill(lepton2Minus->theta());

      h_DielectronPtOSDL->Fill(dileptonPlus.pt());
      h_DielectronEtaOSDL->Fill(pseudorapidityPlus);
      h_DielectronPhiOSDL->Fill(phiPlus);
      h_DielectronMassInvarOSDL->Fill(dileptonPlusInvariantMass);

      h_DielectronPtOSDL->Fill(dileptonMinus.pt());
      h_DielectronEtaOSDL->Fill(pseudorapidityMinus);
      h_DielectronPhiOSDL->Fill(phiMinus);
      h_DielectronMassInvarOSDL->Fill(dileptonMinusInvariantMass);
    }
    else {
      std::cout << "Something went wrong at line " << __LINE__ << std::endl;
    }
  }
  else {
    std::cout << "Four leptons are not all the same flavour or there are not four at line " << __LINE__ << std::endl;
  }
}


// ------------ method called once each job just before starting event loop  ------------

// ------------ method called once each job just after ending the event loop  ------------
void 
DimuonDarkPhoton::endJob() 
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
// DimuonDarkPhoton::endRun(edm::Run const& iRun, edm::EventSetup const& iEventSetup)
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
DimuonDarkPhoton::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DimuonDarkPhoton);
