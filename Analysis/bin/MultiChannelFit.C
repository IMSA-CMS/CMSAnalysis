#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
//#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/HiggsChannelsAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCutsAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsGenComparisonAnalysis.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <map>
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "TString.h"
#include <memory>	
#include "TSystem.h"


// analysis get channel and then get proccess all the channels
//option 1: use the table to find the useful one or just loop through everything
//getHist and then read the data

//integrate suryas code

//make the output so that combine can read it

void MultiChannelFit()
{
	multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	//	fitParameters();
}

void multipleFits()
{

    FILE *Efficiencies
    Efficiencies = fopen("Efficiencies.txt", "f");

    std::vector<std::vector<std::string>> efficiencyTable;

    while (std::getline (f, Efficiencies)) {     
        std::vector<int> v;              
        std::stringstream s (line);   
        while (getline (s, val, ','))       
            v.push_back (val); 
        efficiencyTable.push_back (v);              
    }





    for(i = 1; i <efficiencyTable.size; i++){
        for(j = 1; i <efficiencyTable[i].size; i++){
            if(std::stoi(efficiencyTable[i][j] >= 0.1)){
                std::string recoChannel = efficiencyTable[0][j];
                std::string genSimChannel = efficiencyTable[i][0];
                if(recoChannel != "none"){
                    //code to get each channel
                    //loop over all the fits on this channel
                    //calculate error and find the smallest one
                    //print the function + parameters

                }



            }
        }
        


    }

