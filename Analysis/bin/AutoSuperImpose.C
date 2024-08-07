#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TSystem.h"
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"

void AutoSuperImpose() {
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");

    //Folder path for your files     
    const std::string filePath = "/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_LepRecoInputs/"; 
    //const std::string filePath = "/eos/uscms/store/user/jpalamad/rootBackups/MLOutputsBackup/";

    ////////////////////////// LeptonJetReco

    //List of files to superimpose
    std::vector<std::string> files = {
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        // "DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        "DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        "DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
        "DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root"
    };   

    // std::vector<std::string> files = {
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root"
    // };  

    // std::vector<std::string> files = {
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.root",
    // };  

    // std::vector<std::string> files = {
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //     "DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    // };  

    std::string fileSet = "RetrainedBDT16-18"; // name to identify the fileset you're working with; only used for outputFile

    //////////////////////////////// BDT

    // //List of files to superimpose
    // std::vector<std::string> files = {
    //     // "TMVA_sgPropWeight_bfPropWeight_0_1_0_1.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_0_1_1_0.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_0_1_1_1.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_0_0_1.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_0_1_0.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_0_1_1.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_1_0_1.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_1_1_0.root",
    //     // "TMVA_sgPropWeight_bfPropWeight_1_1_1_1.root",
    //     // "TMVA_sgPropWeight_bfUniform_0_1_0_1.root",
    //     // "TMVA_sgPropWeight_bfUniform_0_1_1_0.root",
    //     // "TMVA_sgPropWeight_bfUniform_0_1_1_1.root",
    //     // "TMVA_sgPropWeight_bfUniform_1_0_0_1.root",
    //     // "TMVA_sgPropWeight_bfUniform_1_0_1_0.root",
    //     //## "TMVA_sgPropWeight_bfUniform_1_0_1_1.root", // broken
    //     //## "TMVA_sgPropWeight_bfUniform_1_1_0_1.root", // broken
    //     //## "TMVA_sgPropWeight_bfUniform_1_1_1_0.root", // broken
    //     //## "TMVA_sgPropWeight_bfUniform_1_1_1_1.root", // broken
    //     //## "TMVA_sgUniform_bfPropWeight_0_1_0_1.root", // broken
    //     //## "TMVA_sgUniform_bfPropWeight_0_1_1_0.root", // broken
    //     // "TMVA_sgUniform_bfPropWeight_0_1_1_1.root",
    //     // "TMVA_sgUniform_bfPropWeight_1_0_0_1.root",
    //     // "TMVA_sgUniform_bfPropWeight_1_0_1_0.root",
    //     // "TMVA_sgUniform_bfPropWeight_1_0_1_1.root",
    //     // "TMVA_sgUniform_bfPropWeight_1_1_0_1.root",
    //     // "TMVA_sgUniform_bfPropWeight_1_1_1_0.root",
    //     "TMVA_sgUniform_bfPropWeight_1_1_1_1.root",
    //     "TMVA_sgUniform_bfUniform_0_1_0_1.root",
    //     "TMVA_sgUniform_bfUniform_0_1_1_0.root",
    // };   

    // std::string fileSet = "BDT20-23"; // name to identify the fileset you're working with; only used for outputFile
    
    //Labels for the legend    
    //std::vector<TString> names = {"Higgsto2ZD", "Fermion100","Fermion150"};
    std::vector<TString> names;

    //Colors go here
    //std::vector<int> colors = {1, 2, 3, 4, 5};
    std::vector<int> colors;

    //List of histograms to be fetched in ever file

    //std::vector<std::string> histnames = {"1st Highest Dark Photon Pt","1st Highest Dark Photon Daughter Delta R","Delta R"};

    /////////////////////////////// LeptonJetReco

    std::vector<std::string> histnames;
    std::string section = "Low Mass and Different Signs"; // might cause issues because I changed it mid-processing to "Low Mass and Different Sign"
    std::string sectionAbbr = "LMOS";

    std::string varname = "LeptonJetMLOutput";
    
    const std::string outputFile = "SuperPlots/AutoSuperImpose_" + fileSet + "_" + varname + "_" + sectionAbbr + ".root";

    histnames.push_back(section + "__hists/" + section + "_" + varname);

    for (int i = 0; i < static_cast<int>(files.size()); ++i)
    {

        TString legendName = TString(files[i].substr(files[i].find("Decay_"), files[i].find("_Period")).c_str());
        names.push_back(legendName);

        colors.push_back(i + 1);
    }

    ///////////////////////////////// BDT

    // std::vector<std::string> histnames;
    // std::string varname = "MVA_BDT_B";
    
    // const std::string outputFile = "SuperPlots/AutoSuperImpose_" + fileSet + "_" + varname + ".root";

    // histnames.push_back("dataset/Method_BDT/BDT/" + varname);

    // for (int i = 0; i < static_cast<int>(files.size()); ++i)
    // {
    //     TString legendName = TString(files[i].substr(files[i].find("TMVA_"), files[i].find(".root")).c_str());
    //     names.push_back(legendName);

    //     colors.push_back(i + 1);
    // }

    ////////////////////////////////////////////////////

    TString yTitle = "Events (1/Integral)";

    TFile* openedFile;
    TH1* hist;
    std::vector<std::vector<TH1*>> histVectors;
    std::string fileName;
    for (auto histname : histnames)
    {
        std::vector<TH1*> histVector;
        for(std::string file : files) 
        {
	        fileName = filePath + file; 
            openedFile = TFile::Open(fileName.c_str(), "read");
            if(!openedFile) 
            {
                throw std::runtime_error("Cannot open file!");
            }  
            hist = dynamic_cast<TH1*>(openedFile->Get(histname.c_str()));
            if(dynamic_cast<TH2 *>(hist) != 0) 
            {
                TH2* hist2D = dynamic_cast<TH2 *>(hist);
                TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
                histVector.push_back(newhist);
            }
            else 
            {
                histVector.push_back(hist);
            }
            if(!hist)
            {
                throw std::runtime_error("Cannot find hist: \"" + histname +"\"" + " in file " + file);
            }
        }
        histVectors.push_back(histVector);
    }

    TFile *outputRootFile = new TFile(outputFile.c_str(), "RECREATE");
    outputRootFile->cd();
    int count = 0;
    for (auto histVector : histVectors)
    {
        TString xTitle = histVector.at(0)->GetXaxis()->GetTitle();
        TCanvas *canvas = plotFormatter->simpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);
        canvas->Write(histnames.at(count).c_str());
        count++;
    }
    outputRootFile->Close();
    delete outputRootFile;
}
