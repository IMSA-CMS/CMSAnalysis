#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "TLorentzVector.h"

LRWeightModule::LRWeightModule(const std::string& filename) :
  TextOutputModule(filename)
{
  pythia.init();
}

void LRWeightModule::finalize()
{
  // The last one, which otherwise would be missed
  writeCurrentWeights();
}

bool LRWeightModule::process(const edm::EventBase& event)
{
  if (firstTime || getFileParams() != currentFileParams)
    {
      if (firstTime)
	{
	  firstTime = false;
	}
      else
	{
	  writeCurrentWeights();
	}
      currentFileParams = getFileParams();
      lrWeight.clear();
      rlWeight.clear();
    }

  edm::Handle<std::vector<reco::GenParticle>> genParticles;
  event.getByLabel(std::string("prunedGenParticles"), genParticles);	
  edm::Handle<GenEventInfoProduct> genInfo;
  event.getByLabel(std::string("generator"), genInfo);
  
  std::string lambdaString = getFileParams().getLambda();
  double lambda = lambdaString == "100k" ? 100000000 : std::stod(lambdaString) * 1000;
  int interference = getFileParams().getInterference() == Interference::constructive() ? -1 : 1;

  auto weights = calculateWeights(*genInfo, *genParticles, lambda, interference);
  lrWeight.addNumber(weights.first);
  rlWeight.addNumber(weights.second);

  return true;
}

void LRWeightModule::writeCurrentWeights()
{
  std::string filename = formattedFilename(currentFileParams);
  writeLine(filename + "\tLR:\t" + std::to_string(lrWeight.average()) + "\tRL:\t" 
	    + std::to_string(rlWeight.average()));
}

std::string LRWeightModule::formattedFilename(const FileParams& params)
{
  std::string particleString = params.getLeptonType() == LeptonType::electron() ? "E" : "Mu";
  std::string interferenceString = params.getInterference() == Interference::constructive() ? "Con" : "Des";
  std::string massString = (params.getYear() == "2017" || params.getYear() == "2018") ? params.massCutString20172018() : params.getMassRange();

  return "CITo2" + particleString + "_Lam" + params.getLambda() + "TeV" + interferenceString + params.getHelicity()
    + "_" + massString;
}

// Jarvis' code
// Bug-fixed 8/6/19
std::pair<double, double> LRWeightModule::calculateWeights(const GenEventInfoProduct& genEventInfoProduct, 
							   const std::vector<reco::GenParticle>& gen,
							   double lam, int inter) const
{ 
  double sH,tH,uH, sH2, tH2, uH2;

  const reco::Candidate* boson = nullptr;
  const reco::Candidate* leptons;

    double muonpdgId;
    double muonpt;
    double muoneta;
    double muonphi;
    double muoncharge;

  //double antiMuonpdgId;
  //double antiMuonpt;
  //double antiMuoneta;
  //double antiMuonphi;
  //double antiMuoncharge;
  
  double quarkpx=0,antiquarkpx=0, leptonPluspx=0, leptonMinuspx=0;
  double quarkpy=0,antiquarkpy=0, leptonPluspy=0, leptonMinuspy=0;
  double quarkpz=0,antiquarkpz=0, leptonPluspz=0, leptonMinuspz=0;
  double quarkenergy=0,antiquarkenergy=0, leptonPlusenergy=0, leptonMinusenergy=0;

  //Store final state leptons in these variables
  const reco::Candidate* muMinus;
  const reco::Candidate* muPlus;

  //Initialize lambdaSquare and etaLR using default value
  double qCLambda=lam;//?
  double qCLambda2=qCLambda*qCLambda;
  int qCetaLR = inter;

  // Running coupling constat-> changes event by event and accessible in miniAOD
  double genEventalphaEM =genEventInfoProduct.alphaQED(); //not sure if it's executable here
  double alpEM=genEventalphaEM;

  const reco::Candidate* quark = nullptr;
  const reco::Candidate* antiquark = nullptr;
  const reco::Candidate* leptonPlus = nullptr;
  const reco::Candidate* leptonMinus = nullptr;

//Begin particle loop
//  for(const reco::GenParticle& particle : prunedGenParticlesCollection){
  // I will assume the pruned Particle is the parameter gen you passed in
  for(auto& particle : gen)
    {
     //Continue to next particle if not quark, protons or leptons
    if (abs(particle.pdgId())!= 11 && abs(particle.pdgId()) != 13)
      {
	//	std::cout << "PDG ID: " << particle.pdgId() << "\n";
	continue;
      };
    // Check for final state lepton
    if ((particle.pdgId() == 13 || particle.pdgId() == 11) &&  particle.status() == 1){
      
      // Function call to get quark and boson from final state lepton
      boson = getBosonMother(particle);
      leptons =getLeptonMother(particle, false);// leptons here are quarks
      if (!leptons)
	{
	  //	  std::cout << "Lepton pointer is null!" << std::endl;
	  continue;
	}

      if (leptons != nullptr)
	{
	//Trying to store particle into final state lepton by changing its type from genParticle into Reco::Candidate
	
	muMinus = particle.clone();	
	
	if (muMinus == nullptr)
	  {
	    //	  std::cout << "muMinus is null!" << std::endl;
	  }
	}
      if (boson != nullptr){
//	std::cout<< "Z boson is present!" << std::endl;
//	std::cout << "- BOSON INFORMATION: " << std::endl;
//	std::cout <<"boson ID: " << boson->pdgId() << std::endl;
//	std::cout <<"boson Energy: " << boson->energy()<< std::endl;
//	std::cout <<"boson px: " <<boson->px() <<std::endl;
//	std::cout <<"boson py: " <<boson->py() << std::endl;
//	std::cout <<"boson pz: " << boson->pz() <<std::endl;
//	std::cout <<"boson pt: " << boson->pt() <<std::endl;
//	std::cout <<"boson eta: " << boson->eta() <<std::endl;
//	std::cout <<"boson phi: " << boson->phi() <<std::endl;
//	std::cout << std::endl;	
      }
      //Assigning quarks to leptons
      quark = leptons;
//      std::cout << std::endl;
//      std::cout << " - QUARK INFORMATION: " << std::endl;
//      std::cout <<"quark ID: " << quark->pdgId()<< std::endl;
//      std::cout <<"quark Energy: " << quark->energy()<< std::endl;
//      std::cout <<"quark px: " <<quark->px() <<std::endl;
//      std::cout <<"quark py: " <<quark->py() << std::endl;
//      std::cout <<"quark pz: " << quark->pz() <<std::endl;
//      std::cout <<"quark pt: " << quark->pt() <<std::endl;
//      std::cout <<"quark eta: " << quark->eta() <<std::endl;
//      std::cout <<"quark phi: " << quark->phi() <<std::endl;
//      std::cout << std::endl;
//      std::cout << std::endl;

      quarkenergy =  quark->energy();
      quarkpx = quark->px();
      quarkpy = quark->py();
      quarkpz =  quark->pz();

      //Get antiquark
      const reco::Candidate* otherLepton = getLeptonMother(particle, true);
      if (!otherLepton)
	{
	  std::cout << "otherLepton pointer is null!" << std::endl;
	  continue;
	}

      antiquark = otherLepton;
      antiquarkenergy =  antiquark->energy();
      antiquarkpx = antiquark->px();
      antiquarkpy = antiquark->py();
      antiquarkpz =  antiquark->pz();

//      std::cout << std::endl;
//      std::cout << "- ANTIQUARK INFORMATION: " << std::endl;
//      std::cout <<"antiquark ID: " << antiquark->pdgId() << std::endl;
//      std::cout <<"antiquark Energy: " << antiquark->energy()<< std::endl;
//      std::cout <<"antiquark px: " <<antiquark->px() <<std::endl;
//      std::cout <<"antiquark py: " <<antiquark->py() << std::endl;
//      std::cout <<"antiquark pz: " << antiquark->pz() <<std::endl;
//      std::cout <<"antiquark pt: " << antiquark->pt() <<std::endl;
//      std::cout <<"antiquark eta: " << antiquark->eta() <<std::endl;
//      std::cout <<"antiquark phi: " << antiquark->phi() <<std::endl;
//      std::cout << std::endl;	
//      std::cout << std::endl;
      
            //Get initial lepton for contact interaction
      if (boson == nullptr){      
	if (leptons->daughter(0)->pdgId()>0)
	  {
	    leptonMinus = leptons->daughter(0);
	    muonpdgId= (leptons->daughter(0))->pdgId();
	    muonpt= (leptons->daughter(0))->pt();
	    muoneta= (leptons->daughter(0))->eta();
	    muonphi= (leptons->daughter(0))->phi();
	    muoncharge= (leptons->daughter(0))->charge();
	    
	    leptonMinusenergy = leptonMinus->energy();
	    leptonMinuspx = leptonMinus->px();
	    leptonMinuspy = leptonMinus->py();
	    leptonMinuspz = leptonMinus->pz();
	    
	    if (leptons->daughter(1) !=nullptr){
	      leptonPlus = leptons->daughter(1);	      
	      //antiMuonpdgId = (leptons->daughter(1))->pdgId();
	      //antiMuonpt = (leptons->daughter(1))->pt();
	      //antiMuoneta = (leptons->daughter(1))->eta();
	      //antiMuonphi = (leptons->daughter(1))->phi();
	      //antiMuoncharge = (leptons->daughter(1))->charge();
	    
	      
	      leptonPlusenergy = leptonPlus->energy();
	      leptonPluspx = leptonPlus->px();
	      leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
	    }

	  }

	else
	  {
	    leptonPlus = leptons->daughter(0);
	    //antiMuonpdgId = (leptons->daughter(0))->pdgId();
	    //antiMuonpt = (leptons->daughter(0))->pt();
	    //antiMuoneta = (leptons->daughter(0))->eta();
	    //antiMuonphi = (leptons->daughter(0))->phi();
	    //antiMuoncharge = (leptons->daughter(0))->charge();
	    
	    
	    leptonPlusenergy = leptonPlus->energy();
	    leptonPluspx = leptonPlus->px();
	    leptonPluspy = leptonPlus->py();
	    leptonPluspz = leptonPlus->pz();
	    
	    if (leptons->daughter(1) !=nullptr){
	      leptonMinus = leptons->daughter(1);
	      muonpdgId= (leptons->daughter(1))->pdgId();
	      muonpt= (leptons->daughter(1))->pt();
	      muoneta= (leptons->daughter(1))->eta();
	      muonphi= (leptons->daughter(1))->phi();
	      muoncharge= (leptons->daughter(1))->charge();
	    
	      leptonMinusenergy = leptonMinus->energy();
	      leptonMinuspx = leptonMinus->px();
	      leptonMinuspy = leptonMinus->py();
	      leptonMinuspz = leptonMinus->pz();
	    }
	  }
      }

      //Initial lepton for Drell-Yan
      if (boson != nullptr){      
	if (boson->daughter(0)->pdgId()>0)
	  {
	    leptonMinus = boson->daughter(0);
	    muonpdgId= (boson->daughter(0))->pdgId();
	    muonpt= (boson->daughter(0))->pt();
	    muoneta= (boson->daughter(0))->eta();
	    muonphi= (boson->daughter(0))->phi();
	    muoncharge= (boson->daughter(0))->charge();
	    

	    leptonMinusenergy = leptonMinus->energy();
	    leptonMinuspx = leptonMinus->px();
	    leptonMinuspy = leptonMinus->py();
	    leptonMinuspz = leptonMinus->pz();
	    
	    if (boson->daughter(1) !=nullptr){
	      leptonPlus = boson->daughter(1);	      
	      //antiMuonpdgId = (boson->daughter(1))->pdgId();
	      //antiMuonpt = (boson->daughter(1))->pt();
	      //antiMuoneta = (boson->daughter(1))->eta();
	      //antiMuonphi = (boson->daughter(1))->phi();
	      //antiMuoncharge = (boson->daughter(1))->charge();
	  	      
	      leptonPlusenergy = leptonPlus->energy();
	      leptonPluspx = leptonPlus->px();
	      leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
	    }

	  }

	else
	  {
	    leptonPlus = boson->daughter(0);
	    //antiMuonpdgId = (boson->daughter(0))->pdgId();
	    //antiMuonpt = (boson->daughter(0))->pt();
	    //antiMuoneta = (boson->daughter(0))->eta();
	    //antiMuonphi = (boson->daughter(0))->phi();
	    //antiMuoncharge = (boson->daughter(0))->charge();
	  	    
	    leptonPlusenergy = leptonPlus->energy();
	    leptonPluspx = leptonPlus->px();
	    leptonPluspy = leptonPlus->py();
	    leptonPluspz = leptonPlus->pz();
	    
	    if (boson->daughter(1) !=nullptr){
	      leptonMinus = boson->daughter(1);
	      muonpdgId= (boson->daughter(1))->pdgId();
	      muonpt= (boson->daughter(1))->pt();
	      muoneta= (boson->daughter(1))->eta();
	      muonphi= (boson->daughter(1))->phi();
	      muoncharge= (boson->daughter(1))->charge();
	  
	      leptonMinusenergy = leptonMinus->energy();
	      leptonMinuspx = leptonMinus->px();
	      leptonMinuspy = leptonMinus->py();
	      leptonMinuspz = leptonMinus->pz();
	    }
	  }

      }
    }
    
    if ((particle.pdgId() == -13  || particle.pdgId() == -11) &&  particle.status() == 1){

      boson = getBosonMother(particle);
      leptons =getLeptonMother(particle, false);

      if (!leptons)
	{
	  //	  std::cout << "Lepton pointer is null!" << std::endl;
          continue;
        }

      if (leptons != nullptr){
        muPlus = particle.clone();

	if (!muPlus){
	  //	  std::cout << "muPlus is null!" << std::endl;
	}
      }

      quark = leptons;
      const reco::Candidate* otherLepton = getLeptonMother(particle, true);

      if (!otherLepton)
	{
	  //	  std::cout << "Lepton pointer is null!" << std::endl;
          continue;
        }

      antiquark = otherLepton;

      quarkenergy =  quark->energy();
      quarkpx = quark->px();
      quarkpy = quark->py();
      quarkpz =  quark->pz();

      antiquarkenergy =  antiquark->energy();
      antiquarkpx = antiquark->px();
      antiquarkpy = antiquark->py();
      antiquarkpz =  antiquark->pz();

      if (leptons == nullptr){
	//	std::cout << "leptons is null" << std::endl;
      }

      //Initial leptons for contact interaction                                 
      if (boson == nullptr){
        if (leptons->daughter(0)->pdgId()>0)
          {
            leptonMinus = leptons->daughter(0);
            muonpdgId= (leptons->daughter(0))->pdgId();
            muonpt= (leptons->daughter(0))->pt();
            muoneta= (leptons->daughter(0))->eta();
            muonphi= (leptons->daughter(0))->phi();
            muoncharge= (leptons->daughter(0))->charge();

            leptonMinusenergy = leptonMinus->energy();
            leptonMinuspx = leptonMinus->px();
            leptonMinuspy = leptonMinus->py();
            leptonMinuspz = leptonMinus->pz();

	    if (leptons->daughter(1) !=nullptr){
              leptonPlus = leptons->daughter(1);
              //antiMuonpdgId = (leptons->daughter(1))->pdgId();
              //antiMuonpt = (leptons->daughter(1))->pt();
              //antiMuoneta = (leptons->daughter(1))->eta();
	      //antiMuonphi = (leptons->daughter(1))->phi();
              //antiMuoncharge = (leptons->daughter(1))->charge();

              leptonPlusenergy = leptonPlus->energy();
              leptonPluspx = leptonPlus->px();
              leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
            }
          }

	else
          {
            leptonPlus = leptons->daughter(0);
            //antiMuonpdgId = (leptons->daughter(0))->pdgId();
            //antiMuonpt = (leptons->daughter(0))->pt();
            //antiMuoneta = (leptons->daughter(0))->eta();
            //antiMuonphi = (leptons->daughter(0))->phi();
            //antiMuoncharge = (leptons->daughter(0))->charge();

	    leptonPlusenergy = leptonPlus->energy();
            leptonPluspx = leptonPlus->px();
            leptonPluspy = leptonPlus->py();
            leptonPluspz = leptonPlus->pz();
	   
	    if (leptons->daughter(1) !=nullptr){
              leptonMinus = leptons->daughter(1);
              muonpdgId= (leptons->daughter(1))->pdgId();
              muonpt= (leptons->daughter(1))->pt();
              muoneta= (leptons->daughter(1))->eta();
              muonphi= (leptons->daughter(1))->phi();
              muoncharge= (leptons->daughter(1))->charge();
           
              leptonMinusenergy = leptonMinus->energy();
              leptonMinuspx = leptonMinus->px();
              leptonMinuspy = leptonMinus->py();
              leptonMinuspz = leptonMinus->pz();
            }

          }
      };


      //Initial lepton for Drell_Yan                                            
      if (boson != nullptr){
        if (boson->daughter(0)->pdgId()>0)
          {
            leptonMinus = boson->daughter(0);
            muonpdgId= (boson->daughter(0))->pdgId();
            muonpt= (boson->daughter(0))->pt();
            muoneta= (boson->daughter(0))->eta();
            muonphi= (boson->daughter(0))->phi();
            muoncharge= (boson->daughter(0))->charge();

            leptonMinusenergy = leptonMinus->energy();
            leptonMinuspx = leptonMinus->px();
            leptonMinuspy = leptonMinus->py();
            leptonMinuspz = leptonMinus->pz();

	    if (boson->daughter(1) !=nullptr){
              leptonPlus = boson->daughter(1);
              //antiMuonpdgId = (boson->daughter(1))->pdgId();
              //antiMuonpt = (boson->daughter(1))->pt();
              //antiMuoneta = (boson->daughter(1))->eta();
              //antiMuonphi = (boson->daughter(1))->phi();
              //antiMuoncharge = (boson->daughter(1))->charge();

              leptonPlusenergy = leptonPlus->energy();
              leptonPluspx = leptonPlus->px();
              leptonPluspy = leptonPlus->py();
              leptonPluspz = leptonPlus->pz();
            }
	    if (false)
	      {
	    std::cout << "muon pdgID :" <<  muonpdgId << std::endl;
	    std::cout << "muon pt :" <<  muonpt << std::endl;
	    std::cout << "muon eta :" <<  muoneta << std::endl;
	    std::cout << "muon phi :" <<  muonphi << std::endl;
	    std::cout << "muon charge :" <<  muoncharge << std::endl;
	      }
	    
          }
	else
          {
            leptonPlus = boson->daughter(0);
            //antiMuonpdgId = (boson->daughter(0))->pdgId();
            //antiMuonpt = (boson->daughter(0))->pt();
            //antiMuoneta = (boson->daughter(0))->eta();
            //antiMuonphi = (boson->daughter(0))->phi();
            //antiMuoncharge = (boson->daughter(0))->charge();

            leptonPlusenergy = leptonPlus->energy();
            leptonPluspx = leptonPlus->px();
            leptonPluspy = leptonPlus->py();
            leptonPluspz = leptonPlus->pz();

	    if (boson->daughter(1) !=nullptr){
              leptonMinus = boson->daughter(1);
              muonpdgId= (boson->daughter(1))->pdgId();
              muonpt= (boson->daughter(1))->pt();
              muoneta= (boson->daughter(1))->eta();
              muonphi= (boson->daughter(1))->phi();
              muoncharge= (boson->daughter(1))->charge();
	      leptonMinusenergy = leptonMinus->energy();
              leptonMinuspx = leptonMinus->px();
              leptonMinuspy = leptonMinus->py();
              leptonMinuspz = leptonMinus->pz();
            }
          }
      }
    }

  }//end for loop
  //Check for right leptons                                               
  if (boson == nullptr)
    {
      if (leptonPlus == nullptr || leptonMinus == nullptr)
	{
	  //	  std::cout << "No Leptons found in bank!\n";
	  return {0, 0};
	}
      if ((leptonPlus->status() != 23) || (leptonMinus->status() != 23))
	{
	  //	  std::cout << "WRONG INITIAL LEPTON!" << std::endl;
	}
    if ((leptonPlus->status() == 23) && (leptonMinus->status() == 23)) 
      {
	//	std::cout<< "RIGHT INITIAL LEPTON" << std::endl;
      }
    }

  //Print out initial leptons                                             
//  std::cout << " - INITIAL LEPTON INFORMATION:  " << std::endl;
//  std::cout << std::endl;
//
//  std::cout << "+ LEPTON:" <<std::endl;
//  std::cout <<"leptonMinus ID:"     << leptonMinus->pdgId() <<std::endl;
//  std::cout <<"leptonMinus status:"     << leptonMinus->status() <<std::endl;
//  std::cout <<"leptonMinus Energy: " << leptonMinus->energy()<< std::endl;
//  std::cout <<"leptonMinus px: " <<leptonMinus->px() <<std::endl;
//  std::cout <<"leptonMinus py: " <<leptonMinus->py() << std::endl;
//  std::cout <<"leptonMinus pz: " << leptonMinus->pz() <<std::endl;
//  std::cout <<"leptonMinus pt: " << leptonMinus->pt() <<std::endl;
//  std::cout <<"leptonMinus eta: " << leptonMinus->eta() <<std::endl;
//  std::cout <<"leptonMinus phi: " << leptonMinus->phi() <<std::endl;
//
//  std::cout << std::endl;
//  std::cout << std::endl;
//  std::cout << std::endl;
//
//  std::cout << "+ ANTILEPTON" << std::endl;
//  std::cout <<"leptonPlus ID:" << leptonPlus->pdgId() <<std::endl;
//  std::cout <<"leptonMinus status:" << leptonPlus->status() <<std::endl;
//  std::cout <<"leptonPlus Energy: " << leptonPlus->energy()<< std::endl;
//  std::cout <<"leptonPlus px: " <<leptonPlus->px() <<std::endl;
//  std::cout <<"leptonPlus py: " <<leptonPlus->py() << std::endl;
//  std::cout <<"leptonPlus pz: " << leptonPlus->pz() <<std::endl;
//  std::cout <<"leptonPlus pt: " << leptonPlus->pt() <<std::endl;
//  std::cout <<"leptonPlus eta: " << leptonPlus->eta() <<std::endl;
//  std::cout <<"leptonPlus phi: " << leptonPlus->phi() <<std::endl;
//  std::cout << std::endl;
//  std::cout << std::endl;

  if (!leptonMinus)
    {
      
    }

  leptonMinusenergy = leptonMinus->energy();
  leptonMinuspx = leptonMinus->px();
  leptonMinuspy = leptonMinus->py();
  leptonMinuspz = leptonMinus->pz();
  leptonPlusenergy = leptonPlus->energy();
  leptonPluspx = leptonPlus->px();
  leptonPluspy = leptonPlus->py();
  leptonPluspz = leptonPlus->pz();
  
  //Get Mandelstam variables                                                    
  TLorentzVector s_hat_4vector(quarkpx + antiquarkpx, quarkpy +antiquarkpy, quarkpz + antiquarkpz, quarkenergy + antiquarkenergy);

  double s_hat = s_hat_4vector.E()*s_hat_4vector.E() - s_hat_4vector.Px()*s_hat_4vector.Px() - s_hat_4vector.Py()*s_hat_4vector.Py() - s_hat_4vector.Pz()*s_hat_4vector.Pz();

  TLorentzVector t_hat_4vector(quarkpx - leptonMinuspx, quarkpy - leptonMinuspy, quarkpz - leptonMinuspz, quarkenergy - leptonMinusenergy);

  double t_hat = t_hat_4vector.E()*t_hat_4vector.E() - t_hat_4vector.Px()*t_hat_4vector.Px() - t_hat_4vector.Py()*t_hat_4vector.Py() - t_hat_4vector.Pz()*t_hat_4vector.Pz();

  TLorentzVector u_hat_4vector(quarkpx - leptonPluspx, quarkpy- leptonPluspy, quarkpz - leptonPluspz, quarkenergy - leptonPlusenergy);

  double u_hat = u_hat_4vector.E()*u_hat_4vector.E() - u_hat_4vector.Px()*u_hat_4vector.Px() - u_hat_4vector.Py()*u_hat_4vector.Py() - u_hat_4vector.Pz()*u_hat_4vector.Pz();

  sH = s_hat;
  tH = t_hat;
  uH = u_hat;
  sH2 = sH*sH;
  tH2 = tH*tH;
  uH2 = uH*uH;

//  std::cout << "##### MANDELSTAM VARIABLES: " <<std::endl;
//  std::cout << "sHat :" << sH << std::endl;
//  std::cout << "tHat:" << tH << std::endl;
//  std::cout << "uHat:" <<uH << std::endl;
//  std::cout << std::endl;
  if (false)
    {
  std::cout << "sHat square :" << sH2 << std::endl;
    }
//  std::cout << "tHat square :" << tH2 << std::endl;
//  std::cout << "uHat square :" << uH2 << std::endl;
//  std::cout << std::endl;
//  std::cout << std::endl;


  //Begin calculating fracLR and fracRL                                     
  std::complex<double> I(0.0, 1.0);
  // Complex amplitudes.                                                    
  std::complex<double> meLL(0., 0.);
  std::complex<double> meRR(0., 0.);
  std::complex<double> meLR(0., 0.);
  std::complex<double> meRL(0., 0.);
  std::complex<double> meLR_SM(0., 0.);
  std::complex<double> meRL_SM(0., 0.);

  int quarkId=quark->pdgId();
  int idAbs=quarkId;
  // int idAbs=5;                                                           
  int leptonMinusId=leptonMinus->pdgId();
  int idNew=leptonMinusId;

  //Process name                                                            
  // Incoming quarks                                                        
  double tmPgvf = 0.25 * pythia.couplingsPtr->vf(idAbs);
  double tmPgaf = 0.25 * pythia.couplingsPtr->af(idAbs);
  //Outgoing fermions                                                       
  double tmPgvl = 0.25 * pythia.couplingsPtr->vf(idNew);
  double tmPgal = 0.25 * pythia.couplingsPtr->af(idNew);
  double tmPgLf = tmPgvf + tmPgaf;
  double tmPgLl = tmPgvl + tmPgal;
  double tmPgRf = tmPgvf - tmPgaf;
  double tmPgRl = tmPgvl - tmPgal;

  // Kinematics                                                             
  //double qCmNew  = pythia.particleData.m0(idNew);                    
  //double qCmNew2 = qCmNew * qCmNew;                                       
  double qCmZ    = pythia.particleData.m0(23);
  double qCmZ2   = qCmZ * qCmZ;
  double qCGZ    = pythia.particleData.mWidth(23);
  double qCGZ2   = qCGZ * qCGZ;

  if (sH == 0)
    {
      std::cout << "Zero sHat value detected\n";
      return {0,0};
    }

 // Necessary variables to ampitude                                        
  // First term                                                             
  // double alpEM =1./137;                                                  
  double tmPe2QfQl = 4. * M_PI * alpEM * pythia.couplingsPtr->ef(idAbs) * pythia.couplingsPtr->ef(idNew);
  double qCPropGm   = 1./sH;
  //Second term.Model depended variables are defined using incoming quark and outgoing fermion information                                                 
  double tmPe2s2c2 = 4. * M_PI * alpEM 
    / (pythia.couplingsPtr->sin2thetaW() * pythia.couplingsPtr->cos2thetaW());
  double denomPropZ = pow((sH - qCmZ2), 2) + qCmZ2 * qCGZ2;
  double qCrePropZ  = (sH - qCmZ2) / denomPropZ;
  double qCimPropZ  = -qCmZ * qCGZ / denomPropZ;

  if (denomPropZ == 0)
    {
      std::cout << "Zero value of denomPropZ!\n";
      return {0, 0};
    }

 //Third term:4. * M_PI * qCetaLR / qCLambda2;                                  
 // Amplitudes, M = gamma + Z + CI.                                             
      meLL = tmPe2QfQl * qCPropGm
	+ tmPe2s2c2 * tmPgLf * tmPgLl * (qCrePropZ + I * qCimPropZ);
      meRR = tmPe2QfQl * qCPropGm
        + tmPe2s2c2 * tmPgRf * tmPgRl * (qCrePropZ + I * qCimPropZ);
      meLR = tmPe2QfQl * qCPropGm
        + tmPe2s2c2 * tmPgLf * tmPgRl * (qCrePropZ + I * qCimPropZ)
        + 4. * M_PI * qCetaLR / qCLambda2;
      meRL = tmPe2QfQl * qCPropGm
	+ tmPe2s2c2 * tmPgRf * tmPgLl * (qCrePropZ + I * qCimPropZ)
        + 4. * M_PI * qCetaLR / qCLambda2;

      // According to Steve's idea, add SM matrix elements for sigmaNew.        
      // Define standard model matrix elements of RL and LR model               

      meLR_SM = tmPe2QfQl * qCPropGm
        + tmPe2s2c2 * tmPgLf * tmPgRl * (qCrePropZ + I * qCimPropZ);

      meRL_SM = tmPe2QfQl * qCPropGm
        + tmPe2s2c2 * tmPgRf * tmPgLl * (qCrePropZ + I * qCimPropZ);

      // Calculate weighting facror                                             
      double sigma0 = 1.0;
      double sigmaOld = sigma0 * uH2 * std::real(meLL*std::conj(meLL));
      sigmaOld += sigma0 * uH2 * std::real(meRR*std::conj(meRR));
      sigmaOld += sigma0 * tH2 * std::real(meLR*std::conj(meLR));
      sigmaOld += sigma0 * tH2 * std::real(meRL*std::conj(meRL));

      double sigmaNewLR = sigma0 * uH2 *std:: real(meLL*std::conj(meLL));
      sigmaNewLR += sigma0 * uH2 * std::real(meRR*std::conj(meRR));
      sigmaNewLR += sigma0 * tH2 * std::real(meLR*std::conj(meLR));
      // sigma += sigma0 * tH2 * std::real(meRL*std::conj(meRL));               
      sigmaNewLR += sigma0 * tH2 * std::real(meRL_SM *std::conj(meRL_SM));
      double fracLR = sigmaNewLR / sigmaOld;

      double sigmaNewRL = sigma0 * uH2 *std:: real(meLL*std::conj(meLL));
      sigmaNewRL += sigma0 * uH2 * std::real(meRR*std::conj(meRR));
      //sigmaNew += sigma0 * tH2 * std::real(meLR*std::conj(meLR));             
      sigmaNewRL += sigma0 * tH2 * std::real(meRL*std::conj(meRL));
      sigmaNewRL += sigma0 * tH2 * std::real(meLR_SM*std::conj(meLR_SM));
      double fracRL = sigmaNewRL / sigmaOld;
      // double fracSum += frac;                                                
      //double weight *= frac;                                                  
      //double sumRL_Plus_LR =fracLR+fracRL;

      //      static bool doneOnce = false;
//      if (lam < 1000000 && fracLR < 1 && !doneOnce)
//	{
//	  std::cout << std::scientific << "Weight of " << fracLR << "\nGamma term: quark term: " << tmPe2QfQl << " propogator: "
//		    << qCPropGm << " Total: " << tmPe2QfQl * qCPropGm
//		    << "\nZ term: " << tmPe2s2c2 * tmPgLf * tmPgRl * qCrePropZ 
//		    << " + " << tmPe2s2c2 * tmPgLf * tmPgRl * qCimPropZ << " i\nCI term: "
//		    <<  4. * M_PI * qCetaLR / qCLambda2 << '\n';
//
//	  doneOnce = true;
//	}
//
//      if (fracLR > 5)
//	{
//	  std::cout << std::scientific << "Weight of " << fracLR << " is greater than 5!\nGamma term: quark term: " << tmPe2QfQl << " propogator: "
//		    << qCPropGm << " Total: " << tmPe2QfQl * qCPropGm
//		    << "\nZ term: " << tmPe2s2c2 * tmPgLf * tmPgRl * qCrePropZ 
//		    << " + " << tmPe2s2c2 * tmPgLf * tmPgRl * qCimPropZ << " i\nCI term: "
//		    <<  4. * M_PI * qCetaLR / qCLambda2 << "\ntHat: " << tH2 << "\nSigmaOld: " << sigmaOld << "\nSigmaNew: "
//		    << sigmaNewLR << '\n';
//	}

//      std::cout << std::endl;
//      std::cout <<"############ FRACTION LR and RL #############" << std::endl;
//      std::cout << "fracLR:  " << fracLR << "  " << "fracRL:  " <<fracRL << std::endl;
//      std::cout << std::endl;
//      std::cout << "sumRL_Plus_LR:  " << fracLR+fracRL << std::endl;
//      std::cout << std::endl;
//      std::cout << std::endl;
//      std::cout <<"############ WEIGHT  #############" << std::endl;

      //Set the weighting values 
      return {fracLR, fracRL};
}

const reco::Candidate* LRWeightModule::getBosonMother(const reco::GenParticle& p) const
{
  const reco::Candidate* firstMother = nullptr;
  const reco::Candidate* muonMother = nullptr;
  const reco::Candidate* tempMother = nullptr;
  const reco::Candidate* firstMotherdaughter = nullptr;
  //  const reco::Candidate* motherdaughter = nullptr;
  int motherID;
  int firstMotherdaughterID;
  int motherdaughterID;

  firstMother = p.mother();
  firstMotherdaughter = firstMother->daughter(0);
   if (!firstMother){
    std::cout << "First mother failed for boson." <<std::endl;
    return nullptr;
  };
   if (!firstMotherdaughter){
    std::cout << "First mother daughter failed for boson." <<std::endl;
    return nullptr;
  };

  double firstMotherID = firstMother->pdgId();
  firstMotherdaughterID = firstMotherdaughter->pdgId();

  // First mother from hard interaction
  if (firstMotherID == 23 && (abs(firstMotherdaughterID) ==  11 || abs(firstMotherdaughterID) == 13)) {
	muonMother= firstMother;
  }//end if 
  
  //More than one mother
  tempMother = p.mother();
  
  motherID =tempMother->pdgId();
  const reco::Candidate* motherdaughter = nullptr;
  // Loops over untill it finds mother from hard interaction
  while(motherID != 23) {
    motherdaughter = tempMother;
    tempMother=tempMother->mother();
    
    if (!tempMother){
      return nullptr;
    }
    if (!motherdaughter){
      return nullptr;
    }

    motherID=tempMother->pdgId();
    motherdaughterID = motherdaughter->pdgId();

    if (motherID == 23 && (abs(motherdaughterID) ==  11 || abs(motherdaughterID) == 13)) {

      //      const reco::GenParticle& lastMuon=p;
      //std::cout<< "lastMuon" << lastMuon.pdgId() << std::endl;
	  muonMother=tempMother;
      }//end if 
  }


  // Checking 
  if(muonMother!=nullptr){

    //    std::cout<<" bosonMother pointer is  filled " << std::endl;

  }
  else{
    //    std::cout<<" bosonMother pointer is not filled " << std::endl;
  }// end of checking

  return muonMother;
}
		       
const reco::Candidate* LRWeightModule::getLeptonMother(const reco::GenParticle& p, bool second) const
{
  const reco::Candidate*  firstMother=nullptr;
  const reco::Candidate*  muonMother=nullptr;
  const reco::Candidate* tempMother=nullptr;
  int motherID;

  
  firstMother = p.mother();
  if (!firstMother)
    return nullptr;

  // First mother from hard interaction
  if (abs(firstMother->pdgId()) <= 6 && firstMother->status()== 21){

      if (second)
	if (firstMother->pdgId() > 0){
	  muonMother=firstMother->daughter(0)->mother(1);
	}
	else{
	  muonMother = firstMother;
	}
      else
	if (firstMother->pdgId() > 0){
	  muonMother= firstMother;
	}
	else{
	  muonMother=firstMother->daughter(0)->mother(1);
	}
    }


    // Function call for muon

    //    getLastMuon(

    //const reco::GenParticle muon =p;
    // muonMother=p.mother();

    //const reco::Candidate* lastMuon =p;
    //    const reco::GenParticle* lastMuon=p;
    // std::cout<< "lastMuon" << lastMuon->pdgId() << std::endl;
  //end if 
  


  //More than one mother
  tempMother = p.mother();
  motherID =tempMother->pdgId();
 
  // Loops over untill it finds mother from hard interaction
  while(abs(motherID) > 6) {

    tempMother=tempMother->mother();
    if (!tempMother)
      return nullptr;
    motherID=tempMother->pdgId();

    if (abs(motherID) <= 6 && tempMother->status()== 21){

      //      const reco::GenParticle& lastMuon=p;
      //std::cout<< "lastMuon" << lastMuon.pdgId() << std::endl;
      if (second)
	if (motherID > 0){
	  muonMother=tempMother->daughter(0)->mother(1);
	}
	else{
	  muonMother = tempMother;
	}
      else
	if (motherID > 0){
	  muonMother= tempMother;
	}
	else{
	  muonMother=tempMother->daughter(0)->mother(1);
	}	



    }

      //      const reco::GenParticle& lastMuon=p;
      //std::cout<< "lastMuon" << lastMuon.pdgId() << std::endl;  
    


  }//end while


  // Checking 
  if(muonMother!=nullptr){

    //    std::cout<<" muonMother pointer is  filled " << std::endl;

  }
  else{
    //    std::cout<<" muonMother pointer is not filled " << std::endl;
  }// end of checking

  return muonMother;
}
