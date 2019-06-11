#include <TROOT.h>
#include <TSystem.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h> 
#include <string>
#include <TH1.h>
#include <map>
#include <cmath>
#include "TGraphErrors.h"
#include "TF1.h"
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

#include "combinedGenRecoAnalyzer.h" 
#include <TLorentzVector.h>
double highPtSignFlipWrongCounter= 0;
double highPtSignFlipRightCounter= 0;
Histograms::Histograms()
{
  TH1::SetDefaultSumw2();
  histos["histTkLayWMeas"] = new TH1D("histTkLayWMeas","TrackerLayersWithMeasurement",25,-0.5,24.5); //counts number of layers with hits
  histos["histVdPxHits"] = new TH1D("histVdPxHits", "ValidPixelHits",25,-0.5,24.5); //number of hits in the pixel layers that are valid
  histos["histVdMuHits"] = new TH1D("histVdMuHits", "ValidMuonHits", 25,-0.5,54.5); //number of valid hits in the muon chambers
  histos["histPtRes"] = new TH1D("histPtRes", "Ptresolution", 25, -0.15, 0.80); 
  histos["histDxy"] = new TH1D("histDxy", "Dxy", 25, -0.70, 0.70); 
  histos["histMatchMuStation"] = new TH1D("histMatchMuStation", "MatchedMuonStations", 20, -0.5, 19.5); //number of muon stations that match to the muon tracks 

  histos["histPt"] = new TH1D("histPt", "Pt", 200, 0.0, 2000);
  histos["histEta"] = new TH1D("histEta", "#Eta", 50, -4.0, 4.0);
  histos["histPhi"] = new TH1D("histPhi", "#Phi", 30, -3.2, 3.2);

  histos["histCollinsSoper"] = new TH1D("histCollinsSoper", "CSAngle", 50,-1.0,1.0);
  histos["histIncorrectCollinsSoper"] = new TH1D("histIncorrectCollinsSoper", "IncorrectCollinsSoperAngle", 50,-1.0,1.0);
  histos["histDifferenceAngle"] = new TH1D("histDifferenceAngle", "DifferenceCollinsSoperAngle", 50,0.0,1.0);
  histos["histRandomCollinsSoper"] = new TH1D("histRandomCollinsSoper", "RandomCollinsSoperAngle", 50,-1.0,1.0);
  histos["histHighPtCollinsSoper"] = new TH1D("histHighPtCollinsSoper", "HighPtCollinsSoperAngle", 50,-1.0,1.0);
  histos["histMinusGenPt"] = new TH1D("histMinusGenPt","Pt",100,0.,500.);
  histos["histMinusGenEta"] = new TH1D("histMinusGenEta", "Eta",50,-3.5,3.5);
  histos["histMinusGenPhi"] = new TH1D("histMinusGentPhi", "Phi", 50,-3.15,3.15);
  histos["histPlusGenPt"] = new TH1D("histPlusGenPt","Pt",100,0.,500.);
  histos["histPlusGenEta"] = new TH1D("histPlusGenEta", "Eta",50,-3.5,3.5);
  histos["histPlusGenPhi"] = new TH1D("histPlusGentPhi", "Phi", 50,-3.15,3.15);

  histos["histInvariantMass"] = new TH1D("histInvariantMass","Mass Dist",300,0.,3000);
  histos["GentemphistInvariantMass"] = new TH1D("GtemphistInvariantMass","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["GentemphistNewMassScaleUp"] = new TH1D("GtemphistInvariantMassUp","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["GentemphistNewMassScaleDown"] = new TH1D("GtemphistInvariantMassDown","Mass Dist",scalingbinnum,scalingdmin,scalingdmax);  
  histos["GentemphistNewMassScaleUpDown"] = new TH1D("GtemphistNewMassScaleUpDown", "Mass Dist", scalingbinnum, scalingdmin, scalingdmax); 
  histos["RecotemphistInvariantMass"] = new TH1D("RtemphistInvariantMass","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["RecotemphistNewMassScaleUp"] = new TH1D("RtemphistInvariantMassUp","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["RecotemphistNewMassScaleDown"] = new TH1D("RtemphistInvariantMassDown","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["RecotemphistNewMassScaleUpDown"] = new TH1D("RtemphistNewMassScaleUpDown", "Mass Dist", scalingbinnum, scalingdmin, scalingdmax); 

  histos["GenhistInvariantMass"] = new TH1D("GhistInvariantMass","Mass Dist",scalingbinnum,scalingdmin,scalingdmax);  
  histos["GenhistNewMassScaleUp"] = new TH1D("GhistNewMassScaleUp", "histNewMassScaleUp", scalingbinnum, scalingdmin, scalingdmax); 
  histos["GenhistNewMassScaleDown"] = new TH1D("GhistNewMassScaleDown", "histNewMassScaleDown", scalingbinnum, scalingdmin, scalingdmax); 
  histos["GenhistNewMassScaleUpDown"] = new TH1D("GhistNewMassScaleUpDown", "histNewMassMuScaleUpAntiScaleDown", scalingbinnum, scalingdmin, scalingdmax);

  histos["RecohistCollinsSoper"] = new TH1D("RecohistCollinsSoper", "RecoCollinsSoperAngle", 50,-1.0,1.0);
  histos["RecohistIncorrectCollinsSoper"] = new TH1D("RecohistIncorrectCollinsSoper", "RecoIncorrectCollinsSoperAngle", 50,-1.0,1.0);
  histos["DeltaCosThetaOppositeSign"] =new TH1D("DeltaCosThetaOppositeSign", "DeltaCosThetaOppositeSign", 50,-2.0,2.0);
  //histos["RecohistDifferenceAngle"] = new TH1D("histDifferenceAngle", "RecoDifferenceCollinsSoperAngle", 50,0.0,1.0);
  //histos["RecohistRandomCollinsSoper"] = new TH1D("histRandomCollinsSoper", "RecoRandomCollinsSoperAngle", 50,-1.0,1.0);
  //histos["REcohistHighPtCollinsSoper"] = new TH1D("histHighPtCollinsSoper", "RecoHighPtCollinsSoperAngle", 50,-1.0,1.0);
  histos["DeltaCosThetaSignFlipsRandom"] = new TH1D("DeltaCosThetaSignFlipsRandom", "DeltaCosThetaSignFlipsRandom", 50,-2.0,2.0);
  histos["DeltaCosThetaSignFlipsHighPt"] = new TH1D("DeltaCosThetaSignFlipsHighPt", "DeltaCosThetaSignFlipsHighPt", 50,-2.0,2.0);
  histos["DeltaCosThetaSignFlipsLowEta"] = new TH1D("DeltaCosThetaSignFlipsLowEta", "DeltaCosThetaSignFlipsLowEta", 50,-2.0,2.0);

  histos["CorrectRandom"] = new TH1I("CorrectRandom", "CorrectRandom", 2, 0, 2);
  histos["CorrectHighPt"] = new TH1I("CorrectHighPt", "CorrectHighPt", 2, 0, 2);
  histos["CorrectLowEta"] = new TH1I("CorrectLowEta", "CorrectLowEta", 2, 0, 2);

  histos["RecoSignFlipsPt"] = new TH1D("RecoSignFlipsPt", "RecoSignFlipsPt", 50,0,500);
  histos["RecoSignFlipsPhi"] = new TH1D("RecoSignFlipsPhi", "RecoSignFlipsPhi", 100,-3.5,3.5);
  histos["RecoSignFlipsEta"] = new TH1D("RecoSignFlipsEta", "RecoSignFlipsEta", 100,-3.15,3.15);
  histos["RecoPt"] = new TH1D("RecoPt", "RecoPt", 50,0,500);
  histos["RecoPhi"] = new TH1D("RecoPhi", "RecoPhi", 100,-3.5,3.5);
  histos["RecoEta"] = new TH1D("RecoEta", "RecoEta", 100,-3.15,3.15);

  histos["RecohistInvariantMass"] = new TH1D("RhistInvariantMass","Mass Dist",scalingbinnum,scalingdmin,scalingdmax); 
  histos["RecohistNewMassScaleUp"] = new TH1D("RhistNewMassScaleUp", "histNewMassScaleUp", scalingbinnum, scalingdmin, scalingdmax); 
  histos["RecohistNewMassScaleDown"] = new TH1D("RhistNewMassScaleDown", "histNewMassScaleDown", scalingbinnum, scalingdmin, scalingdmax);  
  histos["RecohistNewMassScaleUpDown"] = new TH1D("RhistNewMassScaleUpDown", "histNewMassMuScaleUpAntiScaleDown", scalingbinnum, scalingdmin, scalingdmax); 

  histos["Genhistrelup"] = new TH1D("Ghistrelup", "Gen Scaled Up", scalingbinnum, scalingdmin, scalingdmax);  
  histos["Genhistreldn"] = new TH1D("Ghistreldn", "Gen Scaled Down", scalingbinnum, scalingdmin, scalingdmax); 
  histos["Genhistrelupdn"] = new TH1D("Ghistrelupdn", "Gen Mu Scaled Up AntiMu Scaled Down", scalingbinnum, scalingdmin, scalingdmax); 

  histos["Recohistrelup"] = new TH1D("Rhistrelup", "Reco Scaled Up", scalingbinnum, scalingdmin, scalingdmax); 
  histos["Recohistreldn"] = new TH1D("Rhistreldn", "Reco Scaled Down", scalingbinnum, scalingdmin, scalingdmax); 
  histos["Recohistrelupdn"] = new TH1D("Rhistrelupdn", "Reco Mu scaled up AntiMu scaled down", scalingbinnum, scalingdmin, scalingdmax);

  //Histograms to calculate acceptance 
  histos["totalGenSimBinsHist"] = new TH1D("Total Gen Sim Hist", "Acceptance", numberOfBins, 300, 3200);
  histos["acceptedRecoBinsHist"] = new TH1D("Total Reco Matched hist", "Acceptance of Events from Gen Sim to Reco", numberOfBins, 300, 3200);
  
  //array of hists to write for muons
  histsMu = {histos["histMinusGenPt"], histos["histMinusGenEta"], histos["histMinusGenPhi"], histos["histPlusGenPt"], histos["histPlusGenEta"], histos["histPlusGenPhi"], histos["histInvariantMass"], histos["histTkLayWMeas"], histos["histVdPxHits"], histos["histVdMuHits"], histos["histPtRes"], histos["histDxy"], histos["histMatchMuStation"], histos["histPt"], histos["histEta"], histos["histPhi"]};
  //array of hists to write for electrons
  histsEle = {histos["histMinusGenPt"], histos["histMinusGenEta"], histos["histMinusGenPhi"],histos["histPlusGenPt"], histos["histPlusGenEta"], histos["histPlusGenPhi"], histos["histInvariantMass"], histos["histPt"], histos["histEta"], histos["histPhi"]};

  GenHists = {histos["GenhistInvariantMass"], histos["GenhistNewMassScaleUp"], histos["GenhistNewMassScaleDown"], histos["GenhistNewMassScaleUpDown"]}; 
  GenTempHists = {histos["GentemphistInvariantMass"], histos["GentemphistNewMassScaleUp"], histos["GentemphistNewMassScaleDown"], histos["GentemphistNewMassScaleUpDown"]}; 
  RecoHists = {histos["RecohistInvariantMass"], histos["RecohistNewMassScaleUp"], histos["RecohistNewMassScaleDown"], histos["RecohistNewMassScaleUpDown"]};//, histos["RecohistCollinsSoper"]};//histos["RecohistRandomCollinsSoper"], histos["RecohistHighPtCollinsSoper"], histos["RecohistIncorrectCollinsSoper"], histos["RecohistDifferenceAngle"]}; 
  RecoTempHists = {histos["RecotemphistInvariantMass"], histos["RecotemphistNewMassScaleUp"], histos["RecotemphistNewMassScaleDown"], histos["RecotemphistNewMassScaleUpDown"]}; 

  GenRelDiffHists = {histos["Genhistrelup"], histos["Genhistreldn"], histos["Genhistrelupdn"]};
  RecoRelDiffHists = {histos["Recohistrelup"], histos["Recohistreldn"], histos["Recohistrelupdn"]};

}
struct matchedList; 

double Histograms::calculateError(double exp, double theo)const
{
  return ((exp-theo)/theo); 
}

double Histograms::changePtUp(double pt, double kappa) const
{
  return (pt/(1-(pt*kappa)));
}
double Histograms::changePtDown(double pt, double kappa) const
{
  return (pt/(1+(pt*kappa)));
}

double Histograms::findScale(double num, std::vector<double> crossSections, TH1* hist) const
{
  double scale = crossSections[num]*40*1000/(hist->Integral());
  return scale; 
}
 
double Histograms::findInvariantMass(double particlept, double antiparticlept, const reco::LeafCandidate& particle, const reco::LeafCandidate& antiparticle) const
{
  double product = 2*particlept*antiparticlept; 
  double diff = cosh(particle.eta()-antiparticle.eta())-cos(particle.phi()-antiparticle.phi()); 
  double invariantMass = product*diff; 
  if (invariantMass > 0)
    {
      return sqrt(invariantMass);
    }
 
  else
    {
      return 0; 
    }
}

double Histograms::findDeltaPhi(double recoPhi, double genPhi) const
{
  const double pi = 3.1415926535897932384;
  double actualPhiDif = recoPhi-genPhi;
  double phiDif = fabs(actualPhiDif);
  //  std::cout << "recoPhi " << recoPhi << " genPhi " << genPhi << " phiDif before " << phiDif;
  if(phiDif>pi)
    {
      phiDif = (2*pi)-phiDif;
    }
  //  std::cout << " phiDif after " << phiDif << '\n';
  return phiDif; 
}

double Histograms::findDeltaR(double recoEta, double genEta, double recoPhi, double genPhi, double phiDif) const
{  
  //  std::cout << "Phi dif: " << phiDif;
  double etaDif = recoEta-genEta; 
  double deltaR = sqrt((etaDif*etaDif)+(phiDif*phiDif));
  //  std::cout << " recoEta " << recoEta << " genEta " << genEta << " etaDif " << etaDif << " deltaR " << deltaR << '\n';
  return deltaR; 
}

std::map <TH1 *, std::function<double()>> Histograms::muonMakeDictionary(const pat::Muon* objectP)
{  
  std::map <TH1 *, std::function<double()>> hists;
  auto histTkLayWMeas1 = [objectP](){return objectP->globalTrack()->hitPattern().trackerLayersWithMeasurement();};
  hists[histos["histTkLayWMeas"]] = histTkLayWMeas1;
  auto histVdPxHits1 = [objectP](){return objectP->globalTrack()->hitPattern().numberOfValidPixelHits();};
  hists[histos["histVdPxHits"]] = histVdPxHits1;
  auto histVdMuHists1 = [objectP](){return objectP->globalTrack()->hitPattern().numberOfValidMuonHits();};
  hists[histos["histVdMuHits"]] = histVdMuHists1;
            
  auto histsDxy = [objectP](){return objectP->tunePMuonBestTrack()->dxy();};
  hists[histos["histDxy"]] =  histsDxy;
  auto histsMatchMuStation = [objectP](){return objectP->numberOfMatchedStations();};
  hists[histos["histMatchMuStation"]] = histsMatchMuStation;
  auto histsPt = [objectP](){return objectP->tunePMuonBestTrack()->pt();};
  hists[histos["histPt"]] = histsPt;
  auto histsEta = [objectP](){return objectP->tunePMuonBestTrack()->eta();};
  hists[histos["histEta"]] = histsEta;
  auto histsPhi = [objectP](){return objectP->tunePMuonBestTrack()->phi();};
  hists[histos["histPhi"]] = histsPhi;
   
  return hists;
}

std::map <TH1 *, std::function<double()>> Histograms::genMakeDictionary(const reco::GenParticle* objectP, bool isAntiParticle)
{
  std::map <TH1 *, std::function<double()>> hists;

  if (!isAntiParticle)
    {
      auto ptMinusFunc = [objectP](){return objectP->pt();};
      hists[histos["histMinusGenPt"]] = ptMinusFunc;
      auto etaMinusFunc = [objectP](){return objectP->eta();};
      hists[histos["histMinusGenEta"]] = etaMinusFunc;
      auto phiMinusFunc = [objectP](){return objectP->phi();};
      hists[histos["histMinusGenPhi"]] = phiMinusFunc;
    }
  else
    {
      auto ptPlusFunc = [objectP](){return objectP->pt();};
      hists[histos["histPlusGenPt"]] = ptPlusFunc;
      auto etaPlusFunc = [objectP](){return objectP->eta();};
      hists[histos["histPlusGenEta"]] = etaPlusFunc;
      auto phiPlusFunc = [objectP](){return objectP->phi();};
      hists[histos["histPlusGenPhi"]] = phiPlusFunc;
           
    }
  return hists;
}

bool Histograms::isParticle(const reco::GenParticle& p) const
{
  const reco::Candidate* nu = p.mother();
  int motherId = nu->pdgId();
  bool isParticle = true; 
  while (abs(motherId) > 6) //not a quark
    {
      if(nu->mother() && nu)
	{
	  nu = nu->mother(); 
	  motherId = nu->pdgId();
	  if (abs(motherId) > 13)
	    { //not a particle
	      isParticle = false; 
	    }
	  if (abs(motherId) < 7)
	    {//#is a quark 
	      isParticle = true;
	    }
	}		  
      else
	{
	  std::cout<<"error"<<std::endl; 
	  return false; 
	}
    }
  return isParticle;
}

void Histograms::fillScaledGenHists(double GptUp, double GptDown, double GantiPtUp, double GantiPtDown, const reco::GenParticle* particle, const reco::GenParticle* antiparticle, int index)
{ 
  std::vector<double> massCuts = {300, 800, 1300, 2000, 3000}; 
  double GmassUp = findInvariantMass(GptUp, GantiPtUp, *particle, *antiparticle); 
  double GmassDown = findInvariantMass(GptDown, GantiPtDown, *particle, *antiparticle);
  double GmassUpDown = findInvariantMass(GptUp, GantiPtDown, *particle, *antiparticle); 
  double Gmass1 = findInvariantMass(particle->pt(), antiparticle->pt(), *particle, *antiparticle); 
                
  std::vector<double> Gmasses = {Gmass1, GmassUp, GmassDown, GmassUpDown}; 
  if (particle->pt()>ptCut && antiparticle->pt()>ptCut)
    {
      if (Gmass1 > massCuts[index] && Gmass1 < massCuts[index+1])
	{
	  if (index == 0)
	    {
	      for (unsigned int m = 0; m < Gmasses.size(); m++)
		{
		  if (Gmasses[m]!= 0)
		    {
		      GenHists[m]->Fill(Gmasses[m]); 
		    }
		}
	    }
	  else
	    {
	      for(unsigned int n = 0; n<Gmasses.size(); n++)
		{
		  if (Gmasses[n]!= 0)
		    {
		      GenTempHists[n]->Fill(Gmasses[n]);
		    }
		}
	    }		    
	}		    
    }
}

void Histograms::fillScaledRecoHists(double RantiPtUp, double RantiPtDown, const pat::Muon* antimu, double RptUp, double RptDown, const pat::Muon* regmu, int index)
{ 
  std::vector<double> massCuts = {300, 800, 1300, 2000, 3000};
 
  double RmassUp = findInvariantMass(RptUp, RantiPtUp, *regmu, *antimu);
  double RmassDown = findInvariantMass(RptDown, RantiPtDown, *regmu, *antimu);
  double Rmass1 = findInvariantMass(regmu->pt(), antimu->pt(), *regmu, *antimu); 
  double RmassUpDown = findInvariantMass(RptUp, RantiPtDown, *regmu, *antimu);
  std::vector<double> Rmasses = {Rmass1, RmassUp, RmassDown, RmassUpDown}; 
  if (antimu->pt() > ptCut && regmu->pt() > ptCut)
    {	        
      if (Rmass1 > massCuts[index] && Rmass1 < massCuts[index+1])
	{		
	  if (index == 0)
	    {
	      for (unsigned int j=0; j < Rmasses.size(); j++)
		{
		  if (Rmasses[j] != 0)
		    {
		      RecoHists[j]->Fill(Rmasses[j]);
				      
		    }
		}
	    }
	  else
	    {
	      for (unsigned int k=0; k < Rmasses.size(); k++)
		{
		  if(Rmasses[k] != 0)
		    {
		      RecoTempHists[k]->Fill(Rmasses[k]); 
		    }
		}
	    } 
				
	}	
    }
}

std::vector<Histograms::matchedList> Histograms::matchParticles(std::vector<const reco::RecoCandidate*> matching, std::vector<const reco::GenParticle*> matchingGen)
{
  //start with a high value, only really needs to be higher than the cutoff delta R
  double deltaRMin = 100;
  //vector that will be the return
  std::vector<matchedList> matchingBestPairs;
  matchedList pairDataList;
  //counters to set the indices to if the particle is best match
  int matchingBestPairsCounter = 0;
  int matchingCounter;
  int matchingGenCounter = 0;
  //give a high default value so it won't pass the delta R limit unless it is reset with a real value
  double deltaR = 100;
  //keeps track of when the best particles are assigned 
  int bestGenIndex = 0;
  int bestIndex = 0;

//  {
//    double bestPt = 0;
//    const reco::RecoCandidate* recomatch = nullptr;
//    for (auto reco : matching)
//      {
//	if (reco && reco->pt() > bestPt)
//	  {
//	    bestPt = reco->pt();
//	    recomatch = reco;
//	  }
//	  }
//    
//    if (recomatch)
//      {
//	for (auto gen : matchingGen)
//	  {
//	    if (gen && gen->charge() == -1)
//	      {
//		double phiDif = findDeltaPhi(recomatch->phi(), gen->phi());
//		std::cout << "Delta R: " << findDeltaR(recomatch->eta(), gen->eta(), recomatch->phi(), gen->phi(), phiDif) << std::endl;
//	      }
//	  }
//      }
//    
//  }
//
  
  //loops through while there are still at least one gen and reco particle left that have not been matched and set to null
  while (!checkIsNull(matchingGen) && !checkIsNull(matching))
    {

      //goes through all possible particle combinations of gen and reco particles
      for(auto& genParticle : matchingGen)
	{
	  //checks that the particle was not already matched and set to null
	  if (genParticle!=NULL)
	    {
	      matchingCounter = 0;
	      for(auto& recoParticle : matching)
		{
		  if (recoParticle!=NULL)
		    {
		      double phiDif = findDeltaPhi(recoParticle->phi(), genParticle->phi()); 
		      deltaR = findDeltaR(recoParticle->eta(), genParticle->eta(), recoParticle->phi(), genParticle->phi(), phiDif); 
		      //if this delta R is better than the previous best one, keeps track of the information by assigning values ot pariDataList
		      if (deltaR < deltaRMin)
			{
			  pairDataList.bestGenParticle = genParticle;
			  bestGenIndex = matchingGenCounter;
			  pairDataList.bestRecoParticle = recoParticle;
			  bestIndex = matchingCounter;
			  pairDataList.deltaR = deltaR;
			  deltaRMin = deltaR;
			  pairDataList.phiError=(phiDif);
			  pairDataList.etaError=calculateError(recoParticle->eta(), genParticle->eta());
			  pairDataList.ptError=calculateError(recoParticle->pt(), genParticle->pt());;
			}
		    }
		  ++matchingCounter;
		}

	    }
	  ++matchingGenCounter;
	}
      //      std::cout << "Gen particle Pt " << pairDataList.bestGenParticle->pt() << " phi " << pairDataList.bestGenParticle->phi() << " eta " << pairDataList.bestGenParticle->eta() << " matched Pt " << pairDataList.bestRecoParticle->pt() << " phi " << pairDataList.bestRecoParticle->phi() << " eta " << pairDataList.bestRecoParticle->eta() << " with deltaR " << deltaRMin << '\n';

      //makes an additional delta R cut and fills matching best pairs, resets values for the next loop
      //checks if the final (and best) delta R value for the matches passes the cut
      if(deltaRMin<0.1)
	{
	  //keeps track of that match by adding it to the vector that will be returned
	  matchingBestPairs.push_back(pairDataList);

	  //	  std::cout << "Matching pt: " << pairDataList.bestGenParticle->pt() << " Gen charge " << pairDataList.bestGenParticle->charge() <<  << '\n';

	  ++matchingBestPairsCounter;
	  double genParticlePt = pairDataList.bestGenParticle->pt();
	  double indexDecimal = genParticlePt/50-1;
	  int index = (int)indexDecimal;
	  ++binPtCounter[index];
	  if (pairDataList.bestGenParticle->charge()!=pairDataList.bestRecoParticle->charge())
	    {
	      ++signFlipCounter[index];
	    }
	  
	}
      //sets the best matches to null so they are not matched again
      matchingGen[bestGenIndex] = nullptr;
      matching[bestIndex] = nullptr;
      matchingCounter = 0;
      matchingGenCounter = 0;
      //resets the values of delta R to prepare for another round of matching
      deltaRMin = 100;
      deltaR = 100;
    }

//  if (matchingBestPairs.size() == 2)
//    {
//      std::cout << "Highest pt: " << (matchingBestPairs[0].bestRecoParticle->pt() > matchingBestPairs[1].bestRecoParticle->pt() ? 0 : 1) << " Negative charge: " << (matchingBestPairs[0].bestGenParticle->charge() == -1 ? 0 : 1) << '\n';
//    }

  return matchingBestPairs;
}

void Histograms::fillMaps()
{
  //fill maps by looping through the different combinations of parameters, barrelStates, and errorTypes
  for (auto& parameter : parameters)
    {
      for (auto& barrelState : barrelStates)
	{
	  for (auto& errorType : errorTypes)
	    {
	      if (parameter == "Pt")
		{
		  loopRange = ptLoopRange;
		  barrelStateName = barrelState[1]; //only uses the second letter in barrelState for pt
		}
	      else
		{
		  loopRange = massLoopRange;
		  barrelStateName = barrelState;
		}
	      dictErrorGraphName = parameter+"Error"+errorType+barrelStateName;
	      //creates an empt TGraphErrors for each combination of variables
	      dictErrorGraph[dictErrorGraphName] = new TGraphErrors();
	    }
	  //iterate through each range desired for pt and mass to create the histograms
	  for (int i = 0; i<loopRange; ++i)
	    {
	      if (parameter == "Pt")
		{
		  min=minPtCut + ptInterval*i;
		  max = min+ptInterval;
		}
	      else
		{
		  min=minMassCut + massInterval*i;
		  max = min+massInterval;
		}
	      histName = "hist"+parameter+"Error"+barrelStateName+std::to_string(min)+"to"+std::to_string(max);
	      dictErrorHist[histName] = new TH1D(histName.c_str(),histName.c_str(),100,-1,1);
	    }
	}
    }
}
double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar)
{
  //=***********************************************************************
  //
  // 1) cos(theta) = 2 Q^-1 (Q^2+Qt^2)^-1 (Ele^+ Elebar^- - Ele^- Elebar^+)
  //
  // ?) cos(theta) = (2/(Q.Mag()*sqrt(Q.Mag()^2 + Q.Pt()^2 
  //          * (Eleplus * Elebarminus - Eleminus * Elebarplus)
  //
  //
  //=***********************************************************************
  double Eleplus  = 1.0/sqrt(2.0) * (Ele.E() + Ele.Z());
  double Eleminus = 1.0/sqrt(2.0) * (Ele.E() - Ele.Z());

  double Elebarplus  = 1.0/sqrt(2.0) * (Elebar.E() + Elebar.Z());
  double Elebarminus = 1.0/sqrt(2.0) * (Elebar.E() - Elebar.Z());

  TLorentzVector Q(Ele+Elebar);

  //std::cout << "\nQ.Mag = " << Q.Mag() << " Mag of Mag = " << sqrt(pow(Q.Mag(), 2));
  // double costheta = 2.0 / Q.Mag() / sqrt(pow(Q.Mag(), 2) + pow(Q.Pt(), 2)) *
  //  (Eleplus * Elebarminus - Eleminus * Elebarplus);

  //Fix her!!!!
  //std::cout<<Ele.Pt()<<" " << Ele.E()<< " " << Elebar.Pt()<<" " << Elebar.E()<<std::endl;
  //std::cout << "\nQ.Mag = " << Q.Mag() << " Q.Pt = " << Q.Pt() << " Ele+ = " << Eleplus << " Elebar- = " << Elebarminus << " Ele- = " << Eleminus << " Elebar+ = " << Elebarplus << " Pz = " << Ele.E() << " E = " << Ele.Z() << " Pz = " << Elebar.E() << " E = " << Elebar.Z();
  double costheta = 2.0 / (Q.Mag() * sqrt(pow(Q.Mag(),2) + pow(Q.Pt(),2))) * (Eleplus * Elebarminus - Eleminus * Elebarplus);
  //std::cout << "\n(0) cosTheta = " << costheta;
  if (Q.Pz()<0.0) costheta = -costheta;
  //std::cout << "\n(1) cosTheta = " << costheta;
  return costheta;
}

double cosThetaCollinsSoper(const reco::Candidate* particle1, const reco::Candidate* particle2, float RecoMass) 
//start collinssoper
{  
  //std::cout << "big bop";
  //std::cout << "starting collinper\n";
  TLorentzVector Ele;
  TLorentzVector Elebar;
  //std::cout << "/nEt1 = " << Et1 << " Eta1 = " << Eta1 << " Phi1 = " << Phi1 << " En1 = " << En1;
  //std::cout << "/nEt2 = " << Et2 << " Eta2 = " << Eta2 << " Phi2 = " << Phi2 << " En2 = " << En2;

  //if (ChargeEle1<0) 
  //{
  float Et1 = particle1->et();
  float Et2 = particle2->et();
  float Eta1 = particle1->eta();
  float Eta2 = particle2->eta();
  float Phi1 = particle1->phi();
  float Phi2 = particle2->phi();
  float En1 = particle1->energy();
  float En2 = particle2->energy();
  Ele.SetPtEtaPhiE(Et1,Eta1,Phi1,En1);
  Elebar.SetPtEtaPhiE(Et2,Eta2,Phi2,En2);
  //}
  //if (ChargeEle1>0) 
  //{
  //Ele.SetPtEtaPhiE(Et2,Eta2,Phi2,En2);
  //Elebar.SetPtEtaPhiE(Et1,Eta1,Phi1,En1);
  //}
  TLorentzVector Q(Ele+Elebar);
  
  double costheta = calculateCosTheta(Ele, Elebar);


  //=************************************************************************
  //
  // 2) tanphi = (Q^2 + Qt^2)^1/2 / Q (Dt dot R unit) /(Dt dot Qt unit)
  //
  //= ************************************************************************ /
  TLorentzVector Pbeam(0.0, 0.0,  4000., 4000.); // beam momentum in lab frame
  TLorentzVector Ptarget(0.0, 0.0, -4000., 4000.); // beam momentum in lab frame
  // std::cout << "\n(2) cosTheta = " << costheta;

  TLorentzVector D(Ele-Elebar);
  // unit vector on R direction
  TVector3 R = Pbeam.Vect().Cross(Q.Vect());
  TVector3 Runit = R.Unit();
  // unit vector on Qt
  TVector3 Qt = Q.Vect(); Qt.SetZ(0);
  TVector3 Qtunit = Qt.Unit();
  TVector3 Dt = D.Vect(); Dt.SetZ(0);
  double tanphi = sqrt(pow(Q.Mag(), 2) + pow(Q.Perp(), 2)) / Q.Mag() * Dt.Dot(Runit) / 
    Dt.Dot(Qtunit);
  //std::cout << "tanphi equals ";
  //std::cout << tanphi;
  if (Q.Pz()<0.0) 
    tanphi = -tanphi;
  //std::cout << "\n(3) cosTheta = " << costheta;
  //h1_TanPhiCollinsSoperCorrect_->Fill(tanphi,newweight);
  //=************************************************************************
  //
  // 3) sin2(theta) = Q^-2 Dt^2 - Q^-2 (Q^2 + Qt^2)^-1 * (Dt dot Qt)^2
  //
  //=************************************************************************ /
  //double dt_qt = D.X()*Q.X() + D.Y()*Q.Y();
  //double sin2theta = pow(D.Pt()/Q.Mag(), 2)

  //- 1.0/pow(Q.Mag(), 2)/(pow(Q.Mag(), 2) + pow(Q.Pt(), 2))*pow(dt_qt, 2);

  //h1_Sin2AngleCollinsSoperCorrect_->Fill(sin2theta,newweight);
  
  //std::cout << "\n(4) cosTheta = " << cosThetaCS << " = " << costheta;
  //std::cout << "finishing collinssoper\n";
  //cosThetaCS = costheta;
  //std::cout << "\n(5) cosTheta = " << costheta<<std::endl;;
  //tanPhi = tanphi;
  //sin2Theta = sin2theta;
  return costheta;
}

void Histograms::fillHistograms (int loopRange,int minCut,int interval,bool isPt,std::vector<matchedList> matchingPairs,std::map<std::string, TH1*> &dictErrorHist, double etaBarrelCutMin, double etaBarrelCutMax)
{ 
  //std::cout << __LINE__ << std::endl;
  //int counter = 0;
  if (matchingPairs.size()==0)
    {
      return;
    }
  matchedList collinsSoper=matchingPairs[0];
  //matchedList collinsSoper = new matchedList;
  //std::cout << __LINE__ << std::endl;
  matchedList antiCollinsSoper=matchingPairs[0];
  //auto antiCollinsSoper = new matchedList;
  if (matchingPairs.size()>2)
    {
      std::cout<<"too many pairs"<<std::endl;
    }

  //if (matchingPairs.size()==2)
  //{
  //std::cout<<"1: "<<matchingPairs[0].bestRecoParticle->charge()<<std::endl;
  //std::cout<<"2: "<<matchingPairs[1].bestRecoParticle->charge()<<std::endl;
  //}
  //std::cout << __LINE__ << std::endl;






 

  for (auto& pair : matchingPairs)
    {
      //if (counter == 0)
      //{
      //  collinsSoper = pair;
      //  antiCollinsSoper = pair;
      //  counter+=1;
      //}
      if (pair.bestRecoParticle->charge()<0)
	{
	  collinsSoper = pair;
	}
      else
	{
	  antiCollinsSoper = pair;
	}

	
      //std::cout << __LINE__ << std::endl;
      //loop through the different pt ranges (50-3100)
      for (int i = 0; i<loopRange; ++i)
	{
	  int min = minCut + interval*i;
	  int max = min + interval;
	  //for the mass, there must be two pairs
	  if (isPt || matchingPairs.size() >= 2)
	    {
	      for (auto& barrelState : barrelStates)
		{
		  double recoParameterValue;
		  double genParameterValue;
		  //set specific values for pt vs mass
		  if (isPt)
		    {
		      barrelStateName = barrelState[1];
		      recoParameterValue = pair.bestRecoParticle->pt();
		      genParameterValue = pair.bestGenParticle->pt();
		    }
		  else
		    {
		      barrelStateName = barrelState;
		      recoParameterValue = findInvariantMass(matchingPairs[0].bestRecoParticle,matchingPairs[1].bestRecoParticle);
		      genParameterValue = findInvariantMass(matchingPairs[0].bestGenParticle,matchingPairs[1].bestGenParticle);
		    }
		  //std::cout << __LINE__ << std::endl;
		  //determine if this is the correct range to fill the histogram for pt error
		  if (genParameterValue>min && genParameterValue<max)
		    {
		      //checks if the eta matches for the given barrelState that the loop is on and adds the error to the histogram if it does match
		      if ((fabs(pair.bestGenParticle->eta())<etaBarrelCutMin && barrelState=="BB") || (fabs(pair.bestGenParticle->eta())>etaBarrelCutMax && barrelState=="BE"))
			{
			  double error = calculateError(recoParameterValue,genParameterValue);
			  std::string histNameEnd = barrelStateName+std::to_string(min)+"to"+std::to_string(max);
			  std::string histName;
			  //std::cout << __LINE__ << std::endl;
			  if (isPt)
			    {
			      histName = "histPtError" + histNameEnd;
			    }
			  else
			    {
			      histName = "histMassError" + histNameEnd;
			    }
			  dictErrorHist[histName]->Fill(error);
			}
		    }	      
		}
	    }
	}
      //don't loop through all matchingPairs if loop is for mass
      if (!isPt)
	{
	  break;
	}
    
  //std::cout<<pcount<<std::endl;
  //double angle = 0;


  

      bool differentSigns = collinsSoper.bestRecoParticle->charge() != antiCollinsSoper.bestRecoParticle->charge();
      //bool differentSignsBase = matchingPairs[0].bestRecoParticle->charge() != matchingPairs[1].bestRecoParticle->charge();
      //std::cout<<"differentSigns is "<<differentSigns<<"  differentSignsBase is "<<differentSignsBase<<std::endl;
      if (differentSigns)
	{
	  //std::cout<<"particle: "<<collinsSoper.bestRecoParticle->et()<<"   "<<collinsSoper.bestRecoParticle->eta()<<"   "<<collinsSoper.bestRecoParticle->phi()<<"   "<<collinsSoper.bestRecoParticle->energy()<<"      "<<collinsSoper.bestRecoParticle->charge()<<std::endl;
	  //std::cout<<"anti particle: "<<antiCollinsSoper.bestRecoParticle->et()<<"   "<<antiCollinsSoper.bestRecoParticle->eta()<<"   "<<antiCollinsSoper.bestRecoParticle->phi()<<"   "<<antiCollinsSoper.bestRecoParticle->energy()<<"      "<<antiCollinsSoper.bestRecoParticle->charge()<<std::endl;
	  //std::cout<<"invariant mass: "<<findInvariantMass(collinsSoper.bestRecoParticle, antiCollinsSoper.bestRecoParticle)<<std::endl;
	  double recoAngle = cosThetaCollinsSoper(collinsSoper.bestRecoParticle, antiCollinsSoper.bestRecoParticle, findInvariantMass(collinsSoper.bestRecoParticle, antiCollinsSoper.bestRecoParticle));
	  double incorrectAngle = cosThetaCollinsSoper(antiCollinsSoper.bestRecoParticle, collinsSoper.bestRecoParticle, findInvariantMass(collinsSoper.bestRecoParticle, antiCollinsSoper.bestRecoParticle));
	  matchedList genNegative;
	  matchedList genPositive;
	  if (matchingPairs[0].bestGenParticle->charge()<0)
	    {
	      genNegative = matchingPairs[0];
	      genPositive = matchingPairs[1];
	    }
	  else
	    {
	      genNegative = matchingPairs[1];
	      genPositive = matchingPairs[0];
	    }
	  
	  
	  double genAngle = cosThetaCollinsSoper(genNegative.bestGenParticle, genPositive.bestGenParticle, findInvariantMass(genNegative.bestGenParticle, genPositive.bestGenParticle));
	  //std::cout<<__LINE__<<"angle is "<<angle<<std::endl;
	  //std::cout<<"****************************"<<std::endl;
	  //	std::cout<<angle<<std::endl;
	  histos["RecohistCollinsSoper"]->Fill(recoAngle);
	  histos["RecohistIncorrectCollinsSoper"]->Fill(incorrectAngle);
	  histos["DeltaCosThetaOppositeSign"]->Fill(recoAngle-genAngle);
	  //std::cout << __LINE__ << std::endl;
	}
    }
  
  if (isPt)
    return;
  if (matchingPairs.size()==2)
    {
      
      //subset of same sign reco particles (sign flips)
      if (matchingPairs[0].bestRecoParticle->charge()==matchingPairs[1].bestRecoParticle->charge())
	{
	  //	  if (matchingPairs[0].bestRecoParticle->pt() > matchingPairs[1].bestRecoParticle->pt() && matchingPairs[0].bestGenParticle->pt() < matchingPairs[1].bestGenParticle->pt())
	  //	    	    std::cout << "Particle 0: Pt " << matchingPairs[0].bestRecoParticle->pt() << " True Pt " << matchingPairs[0].bestGenParticle->pt() << " Eta " << matchingPairs[0].bestRecoParticle->eta() << " True eta " << matchingPairs[0].bestGenParticle->eta() << " charge " << matchingPairs[0].bestRecoParticle->charge() << " true charge " << matchingPairs[0].bestGenParticle->charge() << "\nParticle 1: Pt " << matchingPairs[1].bestRecoParticle->pt() << " True Pt " << matchingPairs[1].bestGenParticle->pt() << " Eta " << matchingPairs[1].bestRecoParticle->eta() << " True eta " << matchingPairs[1].bestGenParticle->eta() << " charge " << matchingPairs[1].bestRecoParticle->charge() << " true charge " << matchingPairs[1].bestGenParticle->charge() << '\n';

	  double recoRandomAngle;
	  int random = std::rand()%2;
	  //randomly choose "first" particle for the Collins Soper Angle calculation
	  
	 
	  if (random==0)
	    {
	      recoRandomAngle = cosThetaCollinsSoper(matchingPairs[0].bestRecoParticle, matchingPairs[1].bestRecoParticle, findInvariantMass(matchingPairs[0].bestRecoParticle, matchingPairs[1].bestRecoParticle));
	      histos["CorrectRandom"]->Fill(matchingPairs[0].bestGenParticle->charge() == -1 ? 1 : 0);
	    }
	  else
	    {
	      recoRandomAngle = cosThetaCollinsSoper(matchingPairs[1].bestRecoParticle, matchingPairs[0].bestRecoParticle, findInvariantMass(matchingPairs[1].bestRecoParticle, matchingPairs[0].bestRecoParticle));
	      histos["CorrectRandom"]->Fill(matchingPairs[1].bestGenParticle->charge() == -1 ? 1 : 0);
	    }
	  
	  
	  //choose the high pT as the first particle for Collins Soper
	  matchedList highPtPair;
	  matchedList lowPtPair;
	  if (matchingPairs[0].bestRecoParticle->pt()>matchingPairs[1].bestRecoParticle->pt())
	    {
	      highPtPair = matchingPairs[0];
	      lowPtPair = matchingPairs[1];
	    }
	  else
	    {
	      highPtPair = matchingPairs[1];
	      lowPtPair = matchingPairs[0];
	    }
	  histos["CorrectHighPt"]->Fill(highPtPair.bestGenParticle->charge() == -1 ? 1 : 0);

	  double recoHighPtAngle = cosThetaCollinsSoper(highPtPair.bestRecoParticle, lowPtPair.bestRecoParticle, findInvariantMass(highPtPair.bestRecoParticle, lowPtPair.bestRecoParticle));
	  matchedList etaPair1;
	  matchedList etaPair2;
	  if (std::abs(matchingPairs[0].bestRecoParticle->eta())<std::abs(matchingPairs[1].bestRecoParticle->eta()))
	    {
	      if (matchingPairs[0].bestRecoParticle->charge()<0)
		{
		  etaPair1 = matchingPairs[0];
		  etaPair2 = matchingPairs[1];
		}
	      else
		{
		  etaPair1 = matchingPairs[1];
		  etaPair2 = matchingPairs[0];
		}
	    }
	  else
	    {
	      if (matchingPairs[0].bestRecoParticle->charge()<0)
		{
		  etaPair1 = matchingPairs[1];
		  etaPair2 = matchingPairs[0];
		}
	      else
		{
		  etaPair1 = matchingPairs[0];
		  etaPair2 = matchingPairs[1];
		}
	      //	      etaPair1 = matchingPairs[1];
	      //etaPair2 = matchingPairs[0];
	    }
	  histos["CorrectLowEta"]->Fill(etaPair1.bestGenParticle->charge() == -1 ? 1 : 0);
	  double recoLowEtaAngle = cosThetaCollinsSoper(etaPair1.bestRecoParticle, etaPair2.bestRecoParticle, findInvariantMass(etaPair1.bestRecoParticle, etaPair2.bestRecoParticle));
	  

	  //keeps track of how many times the higher pT is actually the electron
	  if (highPtPair.bestGenParticle->charge()<0)
	    {
	      highPtSignFlipRightCounter +=1;
	    }
	  else
	    {
	      highPtSignFlipWrongCounter+=1;
	    }
	  

	  matchedList genNegative;
	  matchedList genPositive;
	  if (matchingPairs[0].bestGenParticle->charge()<0)
	    {
	      genNegative = matchingPairs[0];
	      genPositive = matchingPairs[1];
	    }
	  else
	    {
	      genNegative = matchingPairs[1];
	      genPositive = matchingPairs[0];
	    }


	  double genAngle = cosThetaCollinsSoper(genNegative.bestGenParticle, genPositive.bestGenParticle, findInvariantMass(genNegative.bestGenParticle, genPositive.bestGenParticle));
	  
	  //std::cout<<"genAngle is "<<genAngle<<std::endl;
	  //std::cout<<"recoRandomAngle is "<<recoRandomAngle<<std::endl;
	  //std::cout<<"recoHighPtAngle is "<<recoHighPtAngle<<std::endl;
	  histos["DeltaCosThetaSignFlipsRandom"]->Fill(recoRandomAngle-genAngle);
	  histos["DeltaCosThetaSignFlipsHighPt"]->Fill(recoHighPtAngle-genAngle);
	  histos["DeltaCosThetaSignFlipsLowEta"]->Fill(recoLowEtaAngle-genAngle);

	  //adds to histograms that keeps track of pT, phi, and eta for sign flips vs regular
	  for (unsigned int i = 0; i<matchingPairs.size(); i++)
	    {
	  
	      histos["RecoSignFlipsPt"]->Fill(matchingPairs[i].bestRecoParticle->pt());
	      histos["RecoSignFlipsPhi"]->Fill(matchingPairs[i].bestRecoParticle->phi());
	      histos["RecoSignFlipsEta"]->Fill(matchingPairs[i].bestRecoParticle->eta());
	  
	    }
	}
      
      //no sign flips
      else
	{
	  for (unsigned int i = 0; i<matchingPairs.size(); i++)
	    {
	  
	      histos["RecoPt"]->Fill(matchingPairs[i].bestRecoParticle->pt());
	      histos["RecoPhi"]->Fill(matchingPairs[i].bestRecoParticle->phi());
	      histos["RecoEta"]->Fill(matchingPairs[i].bestRecoParticle->eta());
	  
	    }
	  
	}
    }
}

/*void FillMatchingCollinsSoper(std::vector<Histogram::matchedList> matchingPairs, std::string particle1)
  {
  const std::vector<const reco::RecoCandidate*> electron = nullptr;
  const std::vector<const reco::RecoCandidate*> antielectron = nullptr; 
  for (auto& pair: matchingPairs)
  {
  if (particle1 == "electron")
  {
  if (initCounter == 0)
  {
  electron = &pair.bestRecoParticle;
  antielectron = &pair.bestRecoParticle;
  }
  else
  {
  if (pair.bestRecoParticle.charge()<0)
  {
  electron= &pair.bestRecoParticle;
  }
  else if (pair.bestRecoParticle.charge()>0)
  {
  antielectron = &pair.bestRecoParticle;
  }
  }
  }
  }
  }
*/

void Histograms::checkMatchingHistograms(const std::vector<const reco::GenParticle*>& matchingGen, const std::vector<const reco::RecoCandidate*>& matchingReco, std::string particle1)
{
//  if (matchingReco.size() == 2 && matchingReco[0]->charge() == matchingReco[1]->charge())
//    {
//      if ((matchingReco[0]->pt() > matchingReco[1]->pt() && matchingReco[0]->charge() != -1) || (matchingReco[0]->pt() < matchingReco[1]->pt() && matchingReco[1]->charge() != -1))
//	{
//	std::cout << "Highest pT is not negative\n";
//	std::cout << "Values: particle 0 Pt " << matchingReco[0]->pt() << " charge " << matchingReco[0]->charge() << " particle 1 Pt " << matchingReco[1]->pt() << " charge " << matchingReco[1]->charge() << '\n';
//	}
//    }

  if(matchingReco.size()!=0 && matchingGen.size()!=0)
    {
      auto matchingPairs = matchParticles(matchingReco,matchingGen);
      double etaBarrelCutMin;
      double etaBarrelCutMax;
      //set specific eta cuts for electrons vs muons
      if (particle1=="muon")
	{
	  etaBarrelCutMin=etaBarrelCutMu;
	  etaBarrelCutMax=etaBarrelCutMu;
	}
      else
	{
	  etaBarrelCutMin=etaBarrelCutElec;
	  etaBarrelCutMax=etaEndcapCutElec;
	}

      //      std::cout << "Matched pairs size: " << matchingPairs.size() << std::endl;

      //fill the interval histograms for pt and mass
      fillHistograms(ptLoopRange,minPtCut,ptInterval,true, matchingPairs,dictErrorHist,etaBarrelCutMin,etaBarrelCutMax);
      fillHistograms(massLoopRange,minMassCut,massInterval,false,matchingPairs,dictErrorHist,etaBarrelCutMin,etaBarrelCutMax);
      for(int i = 0; i < numberOfBins; ++i)
	{
	  int minMass = minMassCut + 100*i;
	  int maxMass = minMass + 100;

	  if(matchingPairs.size() >= 2)
	    {
	      double genInvMass = findInvariantMass(matchingPairs[0].bestGenParticle,matchingPairs[1].bestGenParticle);
	      double recoInvMass = findInvariantMass(matchingPairs[0].bestRecoParticle,matchingPairs[1].bestRecoParticle);
	      if(genInvMass > minMass && genInvMass < maxMass)
		{
		  gensimHistogramList.at(i)->Fill(genInvMass);
		  recoHistogramList.at(i)->Fill(recoInvMass);
		}   
	    }   
	}
    }
}

void Histograms::fillTGraphs()
{
  const int medianLower = 1900; 
  const int medianUpper = 3600; 
  //loop for filling the error TGraphs
  for (auto& parameter : parameters)
    {
      for (auto& barrelState : barrelStates)
	{
	  double xError;
	  if (parameter == "Pt")
	    {
	      loopRange = ptLoopRange;
	      xError = 25; //horizontal error bar
	      barrelStateName = barrelState[1];
	    }
	  else
	    {
	      loopRange = massLoopRange;
	      xError = 50;
	      barrelStateName = barrelState;
	    }
	  //loop through each interval
	  for (int i = 0; i<loopRange;++i)
	    {
	      int min;
	      int max;
	      //set the range and barrel state names separately for pT and Mass
	      if (parameter == "Pt")
		{
		  min = minPtCut + ptInterval*i;
		  max = min+ptInterval;
		}
	      else
		{
		  min = minMassCut + massInterval*i;
		  max = min+massInterval;
		}
	      std::string histName = "hist" + parameter+"Error"+barrelStateName+std::to_string(min)+"to"+std::to_string(max);
	      //try to fit the gaussian curves to the error hists, must have enough entries to fit without errors
	      if (dictErrorHist[histName]->GetEntries()>50)
		{
		  //fit gaussian and write the error hist
		  dictErrorHist[histName]->Fit("gaus","","",-0.05,0.05);
		  dictErrorHist[histName]->GetXaxis()->SetTitle("Percent Error");
                  dictErrorHist[histName]->Write();
		  //exract the parameters for each error type (sigma or mean)
		  for (auto& errorType : errorTypes)
		    {
		      std::string functionName = parameter+"Error"+errorType+barrelStateName;
		      dictErrorFunction[functionName] = dictErrorHist[histName]->GetFunction("gaus");
		      double median = (min+max)/2; 
		      double mean = dictErrorFunction[functionName]->GetParameter(1); 
		      double stdDev=dictErrorFunction[functionName]->GetParameter(2);
		      double stdDevError=dictErrorFunction[functionName]->GetParError(2);
		      int index = dictErrorGraph[functionName]->GetN();
		      std::cout<<"index "<<index<<std::endl; 
		      //implement Mass/Pt cut for only displaying the points for the ranges with a 'good' gaussian fit
		      if ((parameter=="Pt" && median<medianLower) || (parameter=="Mass" && median<medianUpper))
			{
			  //set points separately for mean and sigma
			  if (errorType=="Mean")
			    {
			      dictErrorGraph[functionName]->SetPoint(index, median, mean);
			      dictErrorGraph[functionName]->SetPointError(index, xError, stdDev);
			    }
			  else
			    {
			      dictErrorGraph[functionName]->SetPoint(index, median, stdDev);
			      dictErrorGraph[functionName]->SetPointError(index, xError, stdDevError);
			    }
			    
			}
		    }
		}
	    }
	}
    }
}





void Histograms::process(const edm::EventBase& event, std::string particle1, int index)
{ 
  const double kappa = 0.00005; 
  //Get Events Tree and create handles for GEN and RECO
  std::vector<double> massCuts = {300, 800, 1300, 2000, 3000};
  edm::Handle<std::vector<pat::Muon>> muons;
  event.getByLabel(std::string("slimmedMuons"), muons);
  edm::Handle<std::vector<pat::Electron>> electrons;
  event.getByLabel(std::string("slimmedElectrons"), electrons);
  edm::Handle<std::vector<reco::GenParticle>> genParticles;
  event.getByLabel(std::string("prunedGenParticles"), genParticles);		  
  std::vector<const reco::GenParticle*> matchingGen;
  std::vector<const reco::RecoCandidate*> matchingReco;
  std::vector<const reco::GenParticle*> mus;
  std::vector<const reco::GenParticle*> antimus; 
  double GantiPtUp = 0; 
  double GptUp = 0; 
  double GantiPtDown = 0;
  double GptDown = 0;
  //Event Loop
  //Initialize muon objects
  const reco::GenParticle* particle = nullptr;
  const reco::GenParticle* antiparticle = nullptr; 
  const reco::GenParticle* genMu = nullptr;
  const reco::GenParticle* genAntiMu = nullptr; 
  //Begin GEN looping
  //Loop through Particle list&
  for (const auto& p : *genParticles)
    {	
      //Check for muon or electron
      if ((p.pdgId() == 13 || p.pdgId() == 11))
	{
	  if (p.status() == 1 && !particle && (p.mother()->status() != 1))
	    { 
	      if (isParticle(p))
		{
		  particle = &p;
		}
	      if (particle)
		{
		  if (particle->pt()>ptCut)
		    {
		      matchingGen.push_back(particle); 
		      mus.push_back(particle); 
		    }
		}
	    }
	}
      //Check for antimuon
      if ((p.pdgId() == -13 || p.pdgId() == -11))
	{
	  if (p.status() == 1 && !antiparticle && (p.mother()->status() != 1))
	    {
	      if (isParticle(p))
		{
		  antiparticle = &p;
		} 
	      if (antiparticle)
		{
		  if (antiparticle->pt()>ptCut)
		    {
		      matchingGen.push_back(antiparticle); 
		      antimus.push_back(antiparticle); 
		      
		    }
		}
	    }
	} 
    }
  double invariantMass1 = findInvariantMass(particle, antiparticle);
  histos["totalGenSimBinsHist"]->Fill(invariantMass1);

  if (particle && antiparticle)
    {
      double invariantMass = findInvariantMass(particle->pt(), antiparticle->pt(), *particle, *antiparticle); 
      double angle = cosThetaCollinsSoper(particle, antiparticle, invariantMass);
      double incorrectAngle = cosThetaCollinsSoper(antiparticle, particle, invariantMass);
      double difference = std::abs(angle-incorrectAngle);
      double randomAngle = 0;
      int random = std::rand()%2;
      //std::cout<<random<<std::endl;
      if (random==0)
	{
	  randomAngle = cosThetaCollinsSoper(particle, antiparticle, invariantMass);;
	}
      else
	{
	  randomAngle = cosThetaCollinsSoper(antiparticle, particle, invariantMass);
	}
      double histAngle = 0;
      if (particle->pt()>antiparticle->pt())
	{
	  histAngle = cosThetaCollinsSoper(particle, antiparticle, invariantMass);;
	}
      else
	{
	  histAngle = cosThetaCollinsSoper(antiparticle, particle, invariantMass);
	}

      if (particle->charge()==antiparticle->charge())
	{
	  std::cout<<"Signs same"<<std::endl;
	}
      histos["histCollinsSoper"]->Fill(angle);
      histos["histIncorrectCollinsSoper"]->Fill(incorrectAngle);
      histos["histDifferenceAngle"]->Fill(difference);
      histos["histRandomCollinsSoper"]->Fill(randomAngle);
      histos["histHighPtCollinsSoper"]->Fill(histAngle);
      histos["histInvariantMass"]->Fill(invariantMass);
      std::map<TH1 *, std::function<double()>> hists1 = genMakeDictionary(particle, false);
      std::map<TH1 *, std::function<double()>> hists2 = genMakeDictionary(antiparticle, true);
      for (auto pa : hists1)
	{
	  pa.first->Fill(pa.second());
	}
      for (auto pa : hists2)
	{
	  pa.first->Fill(pa.second());
	} 
      if(antiparticle->pt()>53 && particle->pt()>53)
	{
	  histos["acceptedRecoBinsHist"]->Fill(invariantMass1); 
	}
      genMu = findMaxPtParticle(mus);
      genAntiMu = findMaxPtParticle(antimus); 
      if (genMu == 0 || genAntiMu == 0)
	return;
      else
	{
	  GptUp = changePtUp(genMu->pt(), kappa);
	  GptDown = changePtDown(genMu->pt(), kappa); 
	  GantiPtUp = changePtUp(genAntiMu->pt(), kappa);
	  GantiPtDown = changePtDown(genAntiMu->pt(), kappa); 
	  fillScaledGenHists(GptUp, GptDown, GantiPtUp, GantiPtDown, genMu, genAntiMu, index);  
	}
    }	         
  //reco loop     
  if (particle1 == "muon")
    {
      int initCounter1 = 0; 
      double RantiPtUp = 0;
      double RantiPtDown = 0; 
      double RptUp = 0;
      double RptDown = 0;
      const pat::Muon* antimu = nullptr; 
      const pat::Muon* regmu = nullptr;
      const pat::Muon* muon = nullptr;
      const pat::Muon* antimuon = nullptr; 
      std::vector<const pat::Muon*> Mus; 
      std::vector<const pat::Muon*> Antimus; 

      for (const auto& p : *muons)
	{
	  if (p.globalTrack().isNonnull())
	    {
	      std::map<TH1 *, std::function<double()>> hists = muonMakeDictionary(&p);
	      histos["histPtRes"]->Fill(p.tunePMuonBestTrack()->ptError()/p.tunePMuonBestTrack()->pt());
	      for (auto& pa : hists)
		{
		  pa.first->Fill(pa.second());
		}
	      if (p.pt()>ptCut)
		{
		  matchingReco.push_back(&p); 
		}
	    }
	  if (initCounter1 == 0)
	    {
	      antimuon = &p;
	      muon = &p;
	      initCounter1+=1; 
	    }
	  if (p.charge()>0 && p.pt()>ptCut)
	    {
	      antimuon = &p;  
	      Antimus.push_back(antimuon); 
	    }
	  if (p.charge()<0 && p.pt()>ptCut)
	    {
	      muon = &p; 
	      Mus.push_back(muon);
	    }
	}
      regmu = findMaxPtParticle(Mus);
      antimu = findMaxPtParticle(Antimus); 
      if (regmu==0 || antimu==0)
	return;
      else
	{
	  RantiPtUp = changePtUp(antimu->pt(), kappa);
	  RantiPtDown = changePtDown(antimu->pt(), kappa);
	  RptUp = changePtUp(regmu->pt(), kappa);
	  RptDown = changePtDown(regmu->pt(), kappa);
	  fillScaledRecoHists(RantiPtUp, RantiPtDown, antimu, RptUp, RptDown, regmu, index); 
	}
    
    }
  //loop through electron hists
  else
    {
      for (const auto& p : *electrons)
	{       
	  histos["histPt"]->Fill(p.pt()); 
	  histos["histEta"]->Fill(p.superCluster()->eta());
	  histos["histPhi"]->Fill(p.superCluster()->phi());
	  if (p.pt()>ptCut)
	    {
	      matchingReco.push_back(&p); 
	    }
	}
    } 

//  if (matchingReco.size() == 2 && matchingGen.size() == 2 && matchingReco[0]->charge() == matchingReco[1]->charge())
//    {
//      auto biggestPt = matchingReco[0]->pt() > matchingReco[1]->pt() ? matchingReco[0] : matchingReco[1];
//      auto negative = matchingGen[0]->charge() == -1 ? matchingGen[0] : matchingGen[1];
//      double phiDif = findDeltaPhi(biggestPt->phi(), negative->phi()); 
//      double deltaR = findDeltaR(biggestPt->eta(), negative->eta(), biggestPt->phi(), negative->phi(), phiDif); 
//      std::cout << "DeltaR: " << deltaR << '\n';
//    }
//  else
//    return;

  checkMatchingHistograms(matchingGen, matchingReco, particle1);    
}
	
//function that finds the scales 
void Histograms::findScales(int index)
{
  std::vector<double> crossSections = {0.5798, 0.01902, 0.003909, 0.001052}; //lambda 16 TeV cross sections 
  //std::vector<double> crossSections = {372.7, 208.8, 116.8, 49.84}; // lambda 1 TeV cross sections 
  if (index == 0)
    { 
      double Gscale = findScale(index, crossSections, histos["GenhistInvariantMass"]);
      double Rscale = findScale(index, crossSections, histos["RecohistInvariantMass"]); 
      for (TH1* hist : GenHists)
	{
	  hist->Scale(Gscale);
	}
      for (TH1* hist : RecoHists)
	{
	  hist->Scale(Rscale); 
	}	  
    }
  else
    {
      double Gscale = findScale(index, crossSections, histos["GentemphistInvariantMass"]);
      double Rscale = findScale(index, crossSections, histos["RecotemphistInvariantMass"]); 
      for (unsigned int j = 0; j < GenHists.size(); j++)
	{
	  GenTempHists[j]->Scale(Gscale);
	  GenHists[j]->Add(GenTempHists[j]);
	  GenTempHists[j]->Reset();
	}
      for(unsigned int j = 0; j < RecoHists.size(); j++)
	{
	  RecoTempHists[j]->Scale(Rscale);
	  RecoHists[j]->Add(RecoTempHists[j]);
	  RecoTempHists[j]->Reset(); 
	}	    
    }
}

void Histograms::findError()
{
  int startbin = int(((300)/(scalingdmax/scalingbinnum))+1);
  std::vector<std::vector<TH1*>> hists = {GenHists, RecoHists};
  std::vector<std::vector<TH1*>> relDiffHists = {GenRelDiffHists, RecoRelDiffHists};
  for (unsigned int i = 0; i < hists.size(); i++)
    { 
      for (int j = startbin; j<(scalingbinnum+1); j++)
	{
	  if (hists[i][0]->Integral(j, scalingbinnum) > 0)
	    {
	      double normalBin = hists[i][0]->Integral(j, scalingbinnum);
	      double regerr = 0; 
	      hists[i][0]->IntegralAndError(j, scalingbinnum, regerr);
	      for (unsigned int y = 1; y < hists[i].size(); y++)
		{
		  double scaledBin = hists[i][y]->Integral(j, scalingbinnum);
		  double reldiff = (scaledBin-normalBin)/normalBin; 
		  double err = 0;
		  hists[i][y]->IntegralAndError(j, scalingbinnum, err);
		  double scalederror = sqrt(((1/normalBin)*err)*((1/normalBin)*err)+(((-scaledBin)/(normalBin*normalBin))*regerr)*(((-scaledBin)/(normalBin*normalBin))*regerr));
		  relDiffHists[i][y-1]->AddBinContent(j, fabs(reldiff));
		  relDiffHists[i][y-1]->SetBinError(j, scalederror);
		}
	    }
	}
    }
}

void Histograms::writeHistograms(std::string particle1)
{
  std::vector<std::vector<TH1*>> relDiffHists = {GenRelDiffHists, RecoRelDiffHists};
  TCanvas* c1 = new TCanvas("c1", "Gen Overlayed Scaled Up and Scaled Down");
  TLegend* scaleLegend = new TLegend(0.16,0.63,0.45,0.91); 
  scaleLegend->AddEntry(histos["Genhistrelup"], "Scaled-up", "l");
  scaleLegend->AddEntry(histos["Genhistreldn"], "Scaled-down", "l");
  relDiffHists[0][0]->SetOption("E1");
  relDiffHists[0][1]->SetOption("E1");
  relDiffHists[0][0]->Draw();
  relDiffHists[0][0]->SetLineColor(1);
  relDiffHists[0][1]->Draw("SAME");
  relDiffHists[0][1]->SetLineColor(2);
  scaleLegend->Draw();
  c1->Update();

  TCanvas* c2 = new TCanvas("c2", "Reco Overlayed Scaled Up and Scaled Down"); 
  TLegend* scaleLegend2 = new TLegend(0.16,0.63,0.45,0.91); 
  scaleLegend2->AddEntry(histos["Recohistrelup"], "Scaled-up", "l"); 
  scaleLegend2->AddEntry(histos["Recohistreldn"], "Scaled-down", "l"); 
  relDiffHists[1][0]->SetOption("E1"); 
  relDiffHists[1][1]->SetOption("E1"); 
  relDiffHists[1][0]->Draw(); 
  relDiffHists[1][0]->SetLineColor(1); 
  relDiffHists[1][1]->Draw("SAME"); 
  relDiffHists[1][1]->SetLineColor(2);  
  scaleLegend2->Draw(); 
  c2->Update(); 
//  histos["histCollinsSoper"]->Write();
//  histos["histIncorrectCollinsSoper"]->Write();
//  histos["histDifferenceAngle"]->Write();
//  histos["histRandomCollinsSoper"]->Write();
//  histos["histHighPtCollinsSoper"]->Write();
//  histos["RecohistCollinsSoper"]->Write();
//  histos["DeltaCosThetaOppositeSign"]->Write();
//  histos["RecohistIncorrectCollinsSoper"]->Write();
//  histos["DeltaCosThetaSignFlipsRandom"]->Write();
//  histos["DeltaCosThetaSignFlipsHighPt"]->Write();
//  histos["DeltaCosThetaSignFlipsLowEta"]->Write();
//
//  histos["RecoSignFlipsPt"]->Write();
//  histos["RecoSignFlipsPhi"]->Write();
//  histos["RecoSignFlipsEta"]->Write();
//  histos["RecoPt"]->Write();
//  histos["RecoPhi"]->Write();
//  histos["RecoEta"]->Write();
  for (auto& hist : histos)
    {
      hist.second->SetDrawOption("hist");
      hist.second->Write();
    }


  //write hists for muons
  if (particle1 == "muon")
    {
      for (auto i : histsMu)
	{
	  i->SetDrawOption("hist"); 
	  i->Write();
	}
    }
  else
    {
      //write hists for electrons 
      for (auto i: histsEle)
	{
	  i->SetDrawOption("hist"); 
	  i->Write();
	}
    }
  for (unsigned int i = 0; i < GenHists.size(); i++)
    {
      GenHists[i]->Write(); 
      RecoHists[i]->Write(); 
    }

  for (unsigned int i = 0; i < relDiffHists[0].size(); i++)
    {
      relDiffHists[0][i]->Write(); 
      relDiffHists[1][i]->Write(); 
    }
  c1->Write(); 
  c2->Write();
}

void Histograms::writeErrorGraphs()
{
  //set axis titles and draw error graphs
  for (auto& parameter : parameters)
    {
      for (auto& barrelState : barrelStates)
	{
	  for (auto& errorType : errorTypes)
	    {
	      std::string xAxisTitle;
	      if (parameter == "Pt")
		{
		  barrelStateName = barrelState[1];
		  xAxisTitle = "Pt Cut";
		}
	      else
		{
		  barrelStateName = barrelState;
		  xAxisTitle = "Mass Cut";
		}
	      std::string functionName = parameter+"Error"+errorType+barrelStateName;
	      dictErrorGraph[functionName]->SetName(functionName.c_str());
	      dictErrorGraph[functionName]->SetTitle(functionName.c_str());
	      dictErrorGraph[functionName]->GetXaxis()->SetTitle(xAxisTitle.c_str());
	      dictErrorGraph[functionName]->GetYaxis()->SetTitle("Standard Deviation");
	      //dictErrorGraph[functionName]->SetDrawOption("hist"); 
	      dictErrorGraph[functionName]->Draw("ap");
	      dictErrorGraph[functionName]->Write();
	    }
	}
    }
}

void Histograms::fillVectors()
{
  for(int i = 0; i < numberOfBins; ++i)
    {
      const std::string migrationStr = "Migration";
      gensimHistogramList.push_back(new TH1D(("gensimHistBin" + std::to_string(i)).c_str(), migrationStr.c_str(),100, (i * 100 + minMassCut - 200), (i * 100 + minMassCut + 300)));
      recoHistogramList.push_back(new TH1D(("recoHistBin" + std::to_string(i)).c_str(), migrationStr.c_str(),100, (i * 100 + minMassCut - 200), (i * 100 + minMassCut + 300)));
    }
}

void Histograms::writeMigrationHists()
{
  //Write the MIgration hists 
  for(int i = 0; i < numberOfBins; ++i)
    {
      gensimHistogramList[i]->Write();
      recoHistogramList[i]->Write();
    }
  std::cout << "Writing the acceptance hists to the root file \n";
  histos["totalGenSimBinsHist"]->Write();
  histos["totalGenSimBinsHist"]->SetDrawOption("hist"); 
  histos["acceptedRecoBinsHist"]->Write();
  
  //Calculate the Acceptance 
  histos["acceptedRecoBinsHist"]->Divide(histos["totalGenSimBinsHist"]);
  histos["acceptedRecoBinsHist"]->Write();
}
int main(int argc, char* argv[])
{ 
  //std::vector<std::string> filenames = {"CITo2MuM300_Lam16ConLL.txt","CITo2MuM800_Lam16ConLL.txt"/*, "CITo2MuM1300_Lam16ConLL.txt", "CITo2MuM2000_Lam16ConLL.txt"*/};  //muon text files
  std::ofstream myfile ("output.txt");
  std::vector<std::string> mass300;
  std::vector<std::string> mass800;
  std::vector<std::string> mass1300;
  std::vector<std::string> lambdas = {/*"Lam10",*/"Lam16","Lam22","Lam34"};
  std::vector<std::string> interference = {"Con","Des"};
  std::vector<std::string> helicity = {"RR","LL","LR"};
  for(unsigned int l = 0; l < lambdas.size(); ++l)
    {
      for(unsigned int i = 0; i < interference.size(); ++i)
	{
	  for(unsigned int h = 0; h < helicity.size(); ++h)
	    {
	      std::string fileName300="textfiles/CITo2EM300_"+lambdas[l]+interference[i]+helicity[h]+".txt";
	      std::string fileName800="textfiles/CITo2EM800_"+lambdas[l]+interference[i]+helicity[h]+".txt";
	      std::string fileName1300="textfiles/CITo2EM1300_"+lambdas[l]+interference[i]+helicity[h]+".txt";
	      mass300.push_back(fileName300);
	      mass800.push_back(fileName800);
	      mass1300.push_back(fileName1300);
	    }
	}

    }
  //std::vector<std::string> mass2000 = {"CITo2EM2000_Lam16ConRR.txt"}; //electron text files
  //std::cout<<"First entry in 800 mass" << mass800[0]<<std::endl;
  std::vector<std::vector<std::string>> filenames = {mass300,mass800,mass1300};
  //std::cout<<filenames[1][0]<<std::endl;
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");
  optutl::CommandLineParser parser ("Analyze FWLite Histograms");
  parser.addOption("p", optutl::CommandLineParser::kString, "Particle", "");
  parser.parseArguments (argc, argv);
  std::string particle1=parser.stringValue("p");
  unsigned int outputEvery_ = parser.integerValue("outputEvery");
  int ievt=0;
  std::string w_content="";
  

  std::vector<std::vector<std::string>> array(filenames.size());
  
  //create 2D array of mass ranges and different types
  for (unsigned int i = 0; i < filenames.size(); i++)
    {
      //std::cout<<__LINE__<<std::endl;
      
      //array[i].push_back(filenames[i]);
      //std::cout<<filenames[i][i]<<std::endl;
      for (unsigned int j = 0; j < filenames[i].size(); j++)
	{
	  //std::cout<<__LINE__<<std::endl;
	  std::ifstream ifs; 
	  
	  ifs.open(filenames[i][j], std::ifstream::in);
	  if (!ifs)
	    throw std::runtime_error("File " + filenames[i][j] + " not found!");
	  myfile<<"opened file"<<std::endl;
	  while (ifs)
	    {
	      while(std::getline(ifs, w_content))
		{
		  if (!w_content.empty())
		    {
		      //std::cout<<array[j].size()<<std::endl;
		      //std::cout<<array[j][i].size()<<std::endl;
		      //std::cout<<i<<" "<<j<<std::endl;
		      //std::cout<<"w_content"<<w_content<<std::endl;
		      //std::cout<<"size: "<<array[i].size()<<std::endl;
		      //std::cout<<__LINE__<<std::endl;
		      array[i].push_back(w_content);
		      //std::cout<<"size: "<<array[i].size()<<std::endl;
		  
		    }
		}
	      break; 
	    }
	}
    }
  std::string eossrc = "root://cmsxrootd.fnal.gov//";
  gROOT->SetBatch(true);
  //std::cout<<__LINE__<<std::endl;
  // load FWLite C++ libraries
  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libDataFormatsFWLite.so");
  FWLiteEnabler::enable();
  //Create output ROOT file
  TFile *of = new TFile("genOutput.root", "recreate");
  Histograms histograms;
  histograms.fillMaps();
  //std::cout<<__LINE__<<std::endl;
  histograms.fillVectors(); 
  //std::cout<<__LINE__<<std::endl;
  //Loop over ROOT file list
  for (unsigned int index = 0; index < array.size(); ++index) //masses
    {
      highPtSignFlipWrongCounter = 0;
      highPtSignFlipRightCounter = 0;
      //for(unsigned int types=0; types<filenames[index].size(); ++types)
      //{
      std::cout<<"# of root files: "<<array[index].size()<<std::endl;
      for(unsigned int c=0; c<array[index].size(); ++c)
	{
	  auto& x = array[index][c];
	  myfile<<"\n Now Reading file:"<<std::endl; 
	  myfile<<x<<std::endl; 
	  std::string fileStr= eossrc+x;
	  TFile* file = TFile::Open(fileStr.c_str(), "READ");
	  if (!file)
	    {
	      continue;
	    } 
	  fwlite::Event ev(file);
	  std::cerr << "Events: " << ev.size() << std::endl;
	  for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt)
	    {
	      edm::EventBase const & event = ev;
	      if(outputEvery_!=0 ? (ievt>0 && ievt%outputEvery_==0) : false) 
		std::cout<<" processing event: "<<ievt<<std::endl; 
	      histograms.process(event, particle1, index); 
	    }
	  myfile<<"Done with all events"<<std::endl;
	  
	  file->Close();
	  myfile<<"closed the file"<<std::endl;
	  delete file;
	}
      //}
      myfile<<"high pt sign flip wrong counter: "<<highPtSignFlipWrongCounter<<std::endl;
      myfile<<"high pt sign flip right counter: "<<highPtSignFlipRightCounter<<std::endl;
      double percentageWrong = highPtSignFlipWrongCounter/(highPtSignFlipWrongCounter+highPtSignFlipRightCounter);
      myfile<<"Percentage Wrong: "<<percentageWrong<<std::endl;
      double percentageRight = highPtSignFlipRightCounter/(highPtSignFlipWrongCounter+highPtSignFlipRightCounter);
      myfile<<"Percentage Right: "<<percentageRight<<std::endl;
      of->cd(); 
      histograms.findScales(index); 
    }
  of->cd(); 
  histograms.fillTGraphs();   
  histograms.findError(); 
  histograms.writeHistograms(particle1); 
  histograms.writeErrorGraphs(); 
  histograms.writeMigrationHists(); 
 
  of->Close(); 
  myfile.close();

  return 0; 
}

   
