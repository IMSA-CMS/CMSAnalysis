#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "TLorentzVector.h"

// Empty constructor
LRWeightModule::LRWeightModule()
{}

// update to remove event
bool LRWeightModule::process()
{
  /*if (getFileParams().getProcess() != Process::CI())
  {
    return true;
  }
  ParticleCollection genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);	
  GenEventInfoProduct genInfo = getInput()->getGenInfo();
  
  std::string lambdaString = getFileParams().getLambda();

 try 
  {
    double lambda = lambdaString == "100k" ? 100000000 : std::stod(lambdaString) * 1000;
    if (getFileParams().getProcess() == Process::CI())
    {
    int interference = getFileParams().getInterference() == Interference::constructive() ? -1 : 1;

    auto weights = calculateWeights(genInfo, genParticles.getParticles(), lambda, interference);
    lrWeight = weights.first;
    rlWeight = weights.second;
    }
  }
  catch (...)
  {
    lrWeight = 1;
    rlWeight = 1;
  }*/

  return true;
}

double LRWeightModule::findAf(int idAbs) const
{
  double af = 0.000;
  /*int evenTest = idAbs % 2;
  if (evenTest == 0)
  {
    af = 1.000;
  }
  else
  {
    af = -1.000;
  }*/
  
  return af;
}

double LRWeightModule::findVf(int idAbs) const
{
  double vf = 0.000;
  /*int evenTest = idAbs % 2;
  if (idAbs <= 8 and evenTest == 0)
  {
    vf = 0.383;
  }
  else if (idAbs <= 8 and evenTest == 1)
  {
    vf = -0.691;
  }
  else if (idAbs >= 11 and evenTest == 0)
  {
    vf = 1.000;
  }
  else if (idAbs >= 11 and evenTest == 1)
  {
    vf = -0.074;
  }*/
  
  return vf;
}

double LRWeightModule::findEf(int idAbs) const
{
  double ef = 0.000;
  /*int evenTest = idAbs % 2;
  if (idAbs <= 8 and evenTest == 0)
  {
    ef = 0.66666666666667;
  }
  else if (idAbs <= 8 and evenTest == 1)
  {
    ef = -0.33333333333333;
  }
  else if (idAbs >= 11 and evenTest == 0)
  {
    ef = 0.000;
  }
  else if (idAbs >= 11 and evenTest == 1)
  {
    ef = -1.000;
  } */
  
  return ef; 
}

// Jarvis' code
// Bug-fixed 8/6/19
std::pair<double, double> LRWeightModule::calculateWeights(const GenEventInfoProduct& genEventInfoProduct, 
							   const std::vector<reco::GenParticle>& gen,
							   double lam, int inter) const
{ 
  /*double sH,tH,uH, sH2, tH2, uH2;

  const reco::Candidate* boson = nullptr;
  const reco::Candidate* leptons;

    double muonpdgId;
    double muonpt;
    double muoneta;
    double muonphi;
    double muoncharge;
  
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
  // I will assume the pruned Particle is the parameter gen you passed in
  for(auto& particle : gen)
    {
     //Continue to next particle if not quark, protons or leptons
    if (abs(particle.pdgId())!= 11 && abs(particle.pdgId()) != 13)
      {
	continue;
      };
    // Check for final state lepton
    if ((particle.pdgId() == 13 || particle.pdgId() == 11) &&  particle.status() == 1){
      
      // Function call to get quark and boson from final state lepton
      boson = getBosonMother(particle);
      leptons =getLeptonMother(particle, false);// leptons here are quarks
      if (!leptons)
	{
	  continue;
	}

      if (leptons != nullptr)
	{
	//Trying to store particle into final state lepton by changing its type from genParticle into Reco::Candidate
	
	muMinus = particle.clone;
	
	if (muMinus == nullptr)
	  {
	  }
	}
      if (boson != nullptr){
      }
      //Assigning quarks to leptons
      quark = leptons;

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
	      
	      leptonPlusenergy = leptonPlus->energy();
	      leptonPluspx = leptonPlus->px();
	      leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
	    }

	  }

	else
	  {
	    leptonPlus = leptons->daughter(0);
	   
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
	  	      
	      leptonPlusenergy = leptonPlus->energy();
	      leptonPluspx = leptonPlus->px();
	      leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
	    }

	  }

	else
	  {
	    leptonPlus = boson->daughter(0);
	  	    
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
          continue;
        }

      if (leptons != nullptr){
        muPlus = particle.clone;

	if (!muPlus){
	}
      }

      quark = leptons;
      const reco::Candidate* otherLepton = getLeptonMother(particle, true);

      if (!otherLepton)
	{
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

              leptonPlusenergy = leptonPlus->energy();
              leptonPluspx = leptonPlus->px();
              leptonPluspy = leptonPlus->py();
	      leptonPluspz = leptonPlus->pz();
            }
          }

	else
          {
            leptonPlus = leptons->daughter(0);

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
	  return {0, 0};
	}
      if ((leptonPlus->status() != 23) || (leptonMinus->status() != 23))
	{
	}
    if ((leptonPlus->status() == 23) && (leptonMinus->status() == 23)) 
      {
      }
    }

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

  if (false)
    {
  std::cout << "sHat square :" << sH2 << std::endl;
    }

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
  int leptonMinusId=leptonMinus->pdgId();
  int idNew=leptonMinusId;

  //Process name
  //Incoming quarks                                                     
  double tmPgvf = 0.25 * findVf(idAbs);
  double tmPgaf = 0.25 * findAf(idAbs);
  //Outgoing fermions                                                       
  double tmPgvl = 0.25 * findVf(idNew);
  double tmPgal = 0.25 * findAf(idNew);
  double tmPgLf = tmPgvf + tmPgaf;
  double tmPgLl = tmPgvl + tmPgal;
  double tmPgRf = tmPgvf - tmPgaf;
  double tmPgRl = tmPgvl - tmPgal;

  // Kinematics                                                                                                 
  const double qCmZ = 91.18760;
  double qCmZ2   = qCmZ * qCmZ;
  const double qCGZ = 2.49520;
  double qCGZ2   = qCGZ * qCGZ;

  if (sH == 0)
    {
      std::cout << "Zero sHat value detected\n";
      return {0,0};
    }

 // Necessary variables to ampitude                                        
  // First term                                                             
  // double alpEM =1./137;                                                  
  double tmPe2QfQl = 4. * M_PI * alpEM * findEf(idAbs) * findEf(idNew);
  double qCPropGm   = 1./sH;
  //Second term.Model depended variables are defined using incoming quark and outgoing fermion information                                                 
  double tmPe2s2c2 = 4. * M_PI * alpEM 
    / (findSin2thetaW() * findCos2thetaW());
  double denomPropZ = pow((sH - qCmZ2), 2) + qCmZ2 * qCGZ2;
  double qCrePropZ  = (sH - qCmZ2) / denomPropZ;
  double qCimPropZ  = -qCmZ * qCGZ / denomPropZ;

  if (denomPropZ == 0)
    {
      std::cout << "Zero value of denomPropZ!\n"; */
      return {0, 0};
    /*}

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

      //Set the weighting values 
      return {fracLR, fracRL};*/
}

const reco::Candidate* LRWeightModule::getBosonMother(const reco::GenParticle& p) const
{
  /*const reco::Candidate* firstMother = nullptr;
  const reco::Candidate* muonMother = nullptr;
  const reco::Candidate* tempMother = nullptr;
  const reco::Candidate* firstMotherdaughter = nullptr;

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

	  muonMother=tempMother;
      }//end if 
  }


  // Checking 
  if(muonMother!=nullptr){

  }
  else{
  }// end of checking

  return muonMother;*/
  return nullptr;
}
		       
const reco::Candidate* LRWeightModule::getLeptonMother(const reco::GenParticle& p, bool second) const
{
  /*const reco::Candidate*  firstMother=nullptr;
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
   }//end while


  // Checking 
  if(muonMother!=nullptr){

  }
  else{
  }// end of checking

  return muonMother;*/
  return nullptr;
}
