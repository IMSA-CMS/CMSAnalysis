#ifndef combinedGenRecoAnalyzerOriginalFinal_hpp
#define combinedGenRecoAnalyzerOriginalFinal_hpp 

#include <map>
#include <string>
#include <TH1.h>
#include <cmath>
#include <TROOT.h>
#include <TSystem.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include "TGraphErrors.h"
#include "TF1.h"
#include <TCanvas.h>
#include <TLegend.h>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"


class Histograms
{
 public:

  struct matchedList
  {
    const reco::GenParticle* bestGenParticle;
    const reco::RecoCandidate* bestRecoParticle;
    double deltaR;
    double phiError;
    double etaError;
    double ptError;
  };

  void findScales(int index); //finds the scales for the matching graphs 
  void findError(); //finds the error for the scaled graphs 
  void fillTGraphs(); //fills Tgraphs 
  void writeErrorGraphs(); //writes the error graphs 
  void process(const edm::EventBase& event, std::string particle1, int index); //main function that loops through events 
  void writeHistograms(std::string particle1); //writes all the histograms 
  void fillMaps(); //fills the map of matching histograms
  void fillVectors(); //fills vectors containing the acceptance histograms 
  void writeMigrationHists(); //write the migration histograms 
  Histograms(); //constructor that initializes histograms 

 private:

  std::map<std::string, TH1 *> histos; 
  std::vector <TH1 *> histsMu;
  std::vector <TH1 *> histsEle; 
  std::vector<TH1 *> GenHists;
  std::vector<TH1 *> GenTempHists; 
  std::vector<TH1 *> RecoHists; 
  std::vector<TH1 *> RecoTempHists; 
  std::vector<TH1*> GenRelDiffHists;
  std::vector<TH1*> RecoRelDiffHists; 
  //Arrays of Hists for gensim and Reco Invarient Mass at each bin level to measure Migration and Acceptance 
  std::vector <TH1*> gensimHistogramList;
  std::vector <TH1*> recoHistogramList;

  const double scalingdmax = 4000.0; 
  const double scalingdmin = 0.0; 
  const int scalingbinnum = 200;
  const int ptCut = 50; 
  
  //maps for historgrams and TGraphs
  std::map<std::string, TH1*> dictErrorHist;
  std::map<std::string, TF1*> dictErrorFunction;
  std::map<std::string, TGraphErrors*> dictErrorGraph;

  int signFlipCounter[100] = {0}; 
  int binPtCounter[100] = {0}; 

  //creates arrays for all of the different variable types we want to loop through
  std::vector<std::string> barrelStates = {"BB", "BE"};
  std::vector<std::string> parameters = {"Pt","Mass"};
  std::vector<std::string> errorTypes = {"Mean","Sigma"};
  std::vector<std::string> particles = {"Elec", "Mu"};
  std::vector<std::string> lambdas = {"L1", "L16"};
  std::string barrelStateName; //separate barrel state variable to assign one letter barrel state (E or B) for pt 

  std::vector<matchedList> matchParticles(std::vector<const reco::RecoCandidate*> matching, std::vector<const reco::GenParticle*> matchingGen); //takes all of the muon particles from the event and matches by deltaR, returns list of particle pointers, deltaR, phi error, eta error, and pt error
  std::map <TH1 *, std::function<double()>> muonMakeDictionary(const pat::Muon* objectP); //makes a map of muon histograms 
  std::map <TH1 *, std::function<double()>> genMakeDictionary(const reco::GenParticle* objectP, bool isAntiParticle); //makes a map of gen histograms 
  double calculateError(double exp, double theo) const; //finds error
  double changePtUp(double pt, double kappa) const; //scales the Pt up 
  double changePtDown(double pt, double kappa) const; //scales the pt down 
  double findScale(double num, std::vector<double> crossSections, TH1* hist) const; //finds the factor to scale the graphs 
  double findInvariantMass(double particlept, double antiparticlept, const reco::LeafCandidate& particle, const reco::LeafCandidate& antiparticle) const; //finds Invariant Mass with the scaled Pt
  double findDeltaPhi(double recoPhi, double genPhi) const; //calculates delta phi
  double findDeltaR(double recoEta, double genEta, double recoPhi, double genPhi, double phiDif) const; //calculates the delta R value for the pair (with corrections for phi)
  bool isParticle(const reco::GenParticle& p) const; //Function to get (anti)particles that we are interested in from the MC bank and returns 0 is particle has no mother
  void fillScaledGenHists(double GptUp, double GptDown, double GantiPtUp, double GantiPtDown, const reco::GenParticle* particle, const reco::GenParticle* antiparticle, int index); //fills the scaled gen histograms 
  void fillScaledRecoHists(double RantiPtUp, double RantiPtDown, const pat::Muon* antimu, double RptUp, double RptDown, const pat::Muon* regmu, int index); //fills the scaled reco histograms 
  template<typename T> 
    bool checkIsNull (std::vector<T*> matching) const; //checks if vector of matching particles is null
  template<typename T>
  double findInvariantMass(T* particle, T* antiparticle)const; //finds invariant mass
  template<typename T>
  T* findMaxPtParticle(const std::vector<T*>& particles); //loops through vector of particles and returns particle with highest pt 
   
  void fillHistograms (int loopRange,int minCut,int interval,bool isPt,std::vector<matchedList> matchingPairs,std::map<std::string, TH1*> &dictErrorHist, double etaBarrelCutMin, double etaBarrelCutMax); //fills the correct interval histograms for either mass or pt
  void checkMatchingHistograms(const std::vector<const reco::GenParticle*>& matchingGen, const std::vector<const reco::RecoCandidate*>& matchingReco, std::string particle1); //checks for matching histograms 
  
  double etaBarrelCutMu=1.2;
  double etaBarrelCutElec=1.4442;
  double etaEndcapCutElec=1.562;
  int ptLoopRange = 60;
  int massLoopRange = 40;
  int minMassCut = 300;
  int minPtCut = 50;
  int loopRange;
  std::string dictErrorGraphName;
  std::string histName;
  int min;
  int max;
  int ptInterval = 50;
  int massInterval = 100;
  const int numberOfBins = 28;
};

//checks if particle if particle is null
template<typename T>
inline bool Histograms::checkIsNull (std::vector<T*> matching)const
{
  for (auto& particle : matching)
    {
      if (particle!=NULL)
	{
	  return false;
	}
    }
  return true;
}

//calculates the invariant mass of two particles and any type
template<typename T>
inline double Histograms::findInvariantMass(T* particle, T* antiparticle)const
{
  double product = 2*particle->pt()*antiparticle->pt(); 
  double diff = cosh(particle->eta()-antiparticle->eta())-cos(particle->phi()-antiparticle->phi()); 
  double invariantMass = product*diff; 
  if (invariantMass > 0)
    {
      return sqrt(invariantMass);
    }
  else
    {
      return false; 
    }
}

//finds the particle with the highest pT and returns it 
template<typename T>
inline T* Histograms::findMaxPtParticle(const std::vector<T*>& particles)
{ 
  double maxPt = 0;
  T* particle = nullptr;
  for (auto& p : particles)
    {
      if (p->pt()>ptCut && p->pt()>maxPt)
	{
	  maxPt = p->pt(); 
	  if (maxPt != 0)
	    {
	      particle = p; 
	    }
	}
      else
	{
	  continue; 
	}
    } 
    return particle;  
}

#endif 
