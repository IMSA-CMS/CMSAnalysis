///////
// author: Malte Buschmann
// Description: Generates HepMC files for the process
// 	p p -> Z' -> DM DM  with dark abelian shower DM -> DM A' attached
//
//  see arXiv:1505.07459 for further details

// #include "Pythia8/Pythia.h"
// #include "Pythia8Plugins/HepMC2.h"

// using namespace Pythia8;
typedef unsigned int uint;

#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>

using namespace std;

// Determines the A' decay branching ratios for a given mass
// and passes them on to Pythia.
// If this function is not called dark photons remain stable
// See 1505.07459 Section II.C for details
void SetDarkPhotonBranchingRatios(double mA)
{
  double me = 0.511e-3;
  double mm = 0.1057;
  double mt = 1.77682;
  double mq[6] = {0.0023,0.0048,0.095,1.275,4.18,173.07}; // udscbt
  double cq[6] = {2./3.,-1./3.,-1./3.,2./3.,-1./3.,2./3.};
  double alphaEM = 1./137.;
 
  double eps2Gamma = 0.;
  double BR_ee = 0;
  double BR_mm = 0;
  double BR_tt = 0;
  double BR_Pi_pm = 0;
  double BR_K_pm = 0;
  double BR_K_00 = 0;
  double BR_PiGamma = 0;
  double BR_EtaGamma = 0;
  double BR_2PiGamma = 0;
  double BR_Pi_pm0 = 0;
  double BR_PiK_pm = 0;
  double BR_KsK_pm_Pi_pm =0;
  double BR_Pi_pm00 = 0;
  double BR_Pi_pmpm = 0;
  double BR_K_pm_Pi_pm =0;
  double BR_2GammaPi_pm=0;
  double BR_Pi_pmpm0 = 0;
  double BR_Pi_pm000 = 0;
  double BR_Pi_pmpmpm = 0;
  double BR_Pi_pmpm00 = 0;
  double BR_uu = 0;
  double BR_dd = 0;
  double BR_cc = 0;
  double BR_ss = 0;
  double BR_bb = 0;

  if( mA < 2. )
  {
	 //compute partical decay widths/eps²
	ifstream f("rpp2014-hadronicrpp_page1001.dat");
	string sqrts,strR="0.",bufs;
	char bufc[1024];
	while( !f.eof() && f >> sqrts && atof(sqrts.c_str()) <= mA )
	{
		f >> bufs >> bufs >> strR;
		f.getline(bufc,1024);
	}
	double Rhad = atof(strR.c_str());

	double xme = me*me/mA/mA;
	double xmm = mm*mm/mA/mA;
	double eps2Gamma_ee = 1./3.*alphaEM*mA*sqrt( 1.-4.*xme )*(1.+2.*xme);  
	double eps2Gamma_mm = mA > 2*mm ? 1./3.*alphaEM*mA*sqrt( 1.-4.*xmm )*(1.+2.*xmm) : 0.;   
	double eps2Gamma_had = eps2Gamma_mm*Rhad;

	//calculate epsilon 
	eps2Gamma = eps2Gamma_ee+eps2Gamma_mm+eps2Gamma_had;

	//get Branching Ratios
	ifstream f2("BR-2-2000-19-channels.txt");
	string str_mA,strBR[19];
	while( !f2.eof() && f2 >> str_mA && atof(str_mA.c_str())/1000. <= mA )
	{
		f2 >> strBR[0]>> strBR[1]>> strBR[2]>> strBR[3]>> strBR[4]
		   >> strBR[5]>> strBR[6]>> strBR[7]>> strBR[8]>> strBR[9] 
		   >> strBR[10]>> strBR[11]>> strBR[12]>> strBR[13]>> strBR[14]
		   >> strBR[15]>> strBR[16]>> strBR[17]>> strBR[18];
		f2.getline(bufc,1024);
	}
	BR_ee = atof( strBR[0].c_str() );
	BR_mm = atof( strBR[1].c_str() );
	BR_Pi_pm = atof( strBR[2].c_str() );
	BR_K_pm = atof( strBR[3].c_str() );
	BR_K_00 =atof( strBR[4].c_str() );
	BR_PiGamma = atof( strBR[5].c_str() );
	BR_EtaGamma = atof( strBR[6].c_str() );
	BR_2PiGamma = atof( strBR[7].c_str() );
	BR_Pi_pm0 = atof( strBR[8].c_str() );
	BR_PiK_pm = atof( strBR[9].c_str() );
	BR_KsK_pm_Pi_pm =atof( strBR[10].c_str() );
	BR_Pi_pm00 = atof( strBR[11].c_str() );
	BR_Pi_pmpm = atof( strBR[12].c_str() );
	BR_K_pm_Pi_pm =atof( strBR[13].c_str() );
	BR_2GammaPi_pm=atof( strBR[14].c_str() );
	BR_Pi_pmpm0 = atof( strBR[15].c_str() );
	BR_Pi_pm000 = atof( strBR[16].c_str() );
	BR_Pi_pmpmpm = atof( strBR[17].c_str() );
	BR_Pi_pmpm00 = atof( strBR[18].c_str() );
    }else{
	double xme = me*me/mA/mA;
        double xmm = mm*mm/mA/mA;
        double xmt = mt*mt/mA/mA;

        double Gamma_ee = 1./3.*alphaEM*mA*sqrt( 1.-4.*xme )*(1.+2.*xme);
        double Gamma_mm = mA > 2.*mm ? 1./3.*alphaEM*mA*sqrt( 1.-4.*xmm )*(1.+2.*xmm) : 0.;
        double Gamma_tt = mA > 2.*mt ? 1./3.*alphaEM*mA*sqrt( 1.-4.*xmt )*(1.+2.*xmt) : 0.;
        double Gamma_qq[6];
        eps2Gamma = Gamma_ee + Gamma_mm + Gamma_tt;
        for(int i=0;i<6;i++)
        {
        	Gamma_qq[i] = mA > 2.*mq[i] ? 3.*cq[i]*cq[i]*Gamma_ee : 0.;
        	eps2Gamma += Gamma_qq[i];
        }

        BR_ee = Gamma_ee/eps2Gamma;
        BR_mm = Gamma_mm/eps2Gamma;
        BR_tt = Gamma_tt/eps2Gamma;
        BR_uu = Gamma_qq[0]/eps2Gamma;
        BR_dd = Gamma_qq[1]/eps2Gamma;
        BR_ss = Gamma_qq[2]/eps2Gamma;
        BR_cc = Gamma_qq[3]/eps2Gamma;
        BR_bb = Gamma_qq[4]/eps2Gamma;
  }

  stringstream BRstr[26];
  BRstr[0] << "4900022:oneChannel = 1 " <<  BR_ee   		<< " 91 11 -11";
  BRstr[1] << "4900022:addChannel = 1 " <<  BR_mm   		<< " 91 13 -13";
  BRstr[2] << "4900022:addChannel = 1 " <<  BR_tt   		<< " 91 15 -15";
  BRstr[3] << "4900022:addChannel = 1 " <<  BR_Pi_pm 		<< " 100 211 -211";
  BRstr[4] << "4900022:addChannel = 1 " <<  BR_K_pm 		<< " 100 321 -321";
  BRstr[5] << "4900022:addChannel = 1 " <<  BR_K_00 		<< " 100 311 -311";
  BRstr[6] << "4900022:addChannel = 1 " <<  BR_PiGamma 		<< " 100 111 22";
  BRstr[7] << "4900022:addChannel = 1 " <<  BR_EtaGamma 	<< " 100 221 22";
  BRstr[8] << "4900022:addChannel = 1 " <<  BR_2PiGamma		<< " 100 111 111 22";
  BRstr[9] << "4900022:addChannel = 1 " <<  BR_Pi_pm0 		<< " 100 211 -211 111";
  BRstr[10] << "4900022:addChannel = 1 " <<  BR_PiK_pm 		<< " 100 111 321 -321";
  BRstr[11] << "4900022:addChannel = 1 " <<  BR_KsK_pm_Pi_pm/2. << " 100 310 -321 211";
  BRstr[12] << "4900022:addChannel = 1 " <<  BR_KsK_pm_Pi_pm/2. << " 100 310 321 -211";
  BRstr[13] << "4900022:addChannel = 1 " <<  BR_Pi_pm00 	<< " 100 211 -211 111 111";
  BRstr[14] << "4900022:addChannel = 1 " <<  BR_Pi_pmpm 	<< " 100 211 -211 211 -211";
  BRstr[15] << "4900022:addChannel = 1 " <<  BR_K_pm_Pi_pm 	<< " 100 321 -321 211 -211";
  BRstr[16] << "4900022:addChannel = 1 " <<  BR_2GammaPi_pm 	<< " 100 22 22 211 -211";
  BRstr[17] << "4900022:addChannel = 1 " <<  BR_Pi_pmpm0 	<< " 100 211 -211 211 -211 111";
  BRstr[18] << "4900022:addChannel = 1 " <<  BR_Pi_pm000 	<< " 100 211 -211 111 111 111";
  BRstr[19] << "4900022:addChannel = 1 " <<  BR_Pi_pmpmpm 	<< " 100 211 -211 211 -211 211 -211";
  BRstr[20] << "4900022:addChannel = 1 " <<  BR_Pi_pmpm00 	<< " 100 211 -211 211 -211 111 111";
  BRstr[21] << "4900022:addChannel = 1 " <<  BR_dd   		<< " 91 1 -1";
  BRstr[22] << "4900022:addChannel = 1 " <<  BR_uu   		<< " 91 2 -2";
  BRstr[23] << "4900022:addChannel = 1 " <<  BR_ss   		<< " 91 3 -3";
  BRstr[24] << "4900022:addChannel = 1 " <<  BR_cc   		<< " 91 4 -4";
  BRstr[25] << "4900022:addChannel = 1 " <<  BR_bb   		<< " 91 5 -5";

  //added Nov 5
  for(int i=0;i<26;i++)
  	std::cout <<  BRstr[i].str().c_str() << std::endl;

}




//****************************************************************************
// MAIN
//****************************************************************************

int main(int argc,const char* argv[]) 
{
 double mA = 0.3;
 if (argc == 2) {mA = std::stod (argv [1]);}
 // std::cout << "darkphoton mass: " << mA << " GeV \n";
SetDarkPhotonBranchingRatios(mA);
  //init pythia
  // Pythia pythia;

  // stringstream s1; s1 << "Beams:eCM = " << atof(argv[1])*2.;
  // pythia.readString(s1.str().c_str());
  // pythia.readString("Beams:idA = 2212");
  // pythia.readString("Beams:idB = 2212"); 

  // pythia.readString("HiddenValley:Ngauge = 1");
  // pythia.readString("HiddenValley:FSR = on"); 
  // stringstream s2; s2 << "HiddenValley:alphaFSR = " << argv[2];
  // pythia.readString(s2.str().c_str()); 

  // //DM
  // stringstream s3; s3<<"4900101:m0 = " << argv[3];
  // pythia.readString(s3.str().c_str());

  // //dark photon
  // stringstream s4; s4<<"4900022:m0 = " << argv[4];
  // pythia.readString(s4.str().c_str());
  // stringstream s5; s5<<"4900022:tau0 = " << argv[5];
  // pythia.readString(s5.str().c_str());
  // SetDarkPhotonBranchingRatios(atof(argv[4]),pythia); 

  // pythia.readString("Beams:frameType = 4");
  // stringstream s6; s6 << "Beams:LHEF = " << argv[6];
  // pythia.readString(s6.str().c_str());
	

  // ///////
  // //generate events
  // ///////

  // HepMC::Pythia8ToHepMC ToHepMC;
  // HepMC::IO_GenEvent ascii_io("events.hepmc", std::ios::out);
  // pythia.init();
  // //loop over events
  // for(int iEvent = 1; ; ++iEvent){ 
	// if( !pythia.next() ) break;

	// if( iEvent%100 == 0 ) cout << "generated " << iEvent << " events ... " << endl;
	
	// //write to hepmc file
  //   	HepMC::GenEvent* event = new HepMC::GenEvent();
  //   	ToHepMC.fill_next_event( pythia, event );

	// //write to file
	// ascii_io << event;
	
 	// delete event;
  // }//loop over events

  // pythia.stat();
  return 0;
}
