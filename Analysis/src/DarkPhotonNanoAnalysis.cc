
#include "CMSAnalysis/Analysis/interface/DarkPhotonNanoAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/Correction.hh"
#include "CMSAnalysis/Analysis/interface/ConstantCorrection.hh"
#include <memory>	
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TH1.h"
#include "TList.h"
 
DarkPhotonNanoAnalysis::DarkPhotonNanoAnalysis(int start, int end) {

    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.root");
    //const std::string filePath = "/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
	auto reader = std::make_shared<CrossSectionReader>("/uscms/home/mkubon/analysis/clean/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    const std::string filePath = "/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton/"; 
    double luminosity = 20;

    TH1::SetDefaultSumw2();
    std::string massTarget = "0.3";

    std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign"};
    std::vector<std::string> connecters = {"_1st Highest Lepton Jet "};
    std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
    std::vector<std::string> LJVars = {"LeptonJetMLOutput"};

    //#std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};

    std::vector<HistVariable> histVariablesBackground;

    for (std::string rowName : rowNames)
    {
        for (std::string connecter : connecters)
        {
            for (std::string columnName : columnNames)
            {
                histVariablesBackground.push_back(
                    //HistVariable(columnName + " " + rowName, rowName + "__hists/" + rowName + connecter + columnName));
                    HistVariable(columnName + " " + rowName));
            }

            for (std::string LJVar : LJVars)
            {
                histVariablesBackground.push_back(
                    //HistVariable(LJVar + " " + rowName, rowName + "__hists/" + rowName + "_" + LJVar));
                    HistVariable(LJVar + " " + rowName));
            }
        }
    }

    //cross sections should be all lowercase

    // auto dyBackground = std::make_shared<Process>("DY Background", 3);
    // dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, luminosity));
    // dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toInf", reader, luminosity));
    
    auto dpAOD1 = std::make_shared<Process>("Higgs2DP_DpMass_0_1_FSR_0_0_m1000", 1);
        dpAOD1->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD2 = std::make_shared<Process>("Higgs2DP_DpMass_0_2_FSR_0_0_m1000", 2);
        dpAOD2->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD3 = std::make_shared<Process>("Higgs2DP_DpMass_0_3_FSR_0_0_m1000", 3);
        dpAOD3->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD4 = std::make_shared<Process>("Higgs2DP_DpMass_0_3_FSR_0_0_m125", 4);
        dpAOD4->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD5 = std::make_shared<Process>("Higgs2DP_DpMass_0_3_FSR_0_0_m300", 5);
        dpAOD5->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD6 = std::make_shared<Process>("Higgs2DP_DpMass_0_3_FSR_0_1_m1000", 6);
        dpAOD6->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD7 = std::make_shared<Process>("Higgs2DP_DpMass_0_3_FSR_0_3_m1000", 7);
        dpAOD7->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD8 = std::make_shared<Process>("Higgs2DP_DpMass_0_4_FSR_0_0_m1000", 8);
        dpAOD8->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD9 = std::make_shared<Process>("Higgs2DP_DpMass_0_6_FSR_0_0_m1000", 9);
        dpAOD9->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD10 = std::make_shared<Process>("Higgs2DP_DpMass_0_9_FSR_0_0_m1000", 11);
        dpAOD10->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD11 = std::make_shared<Process>("Higgs2DP_DpMass_1_2_FSR_0_0_m1000", 12);
        dpAOD11->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD12 = std::make_shared<Process>("Higgs2DP_DpMass_1_5_FSR_0_0_m1000", 13);
        dpAOD12->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD13 = std::make_shared<Process>("Higgs2DP_DpMass_2_5_FSR_0_0_m1000", 14);
        dpAOD13->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD14 = std::make_shared<Process>("Higgs2DP_DpMass_4_0_FSR_0_0_m1000", 15);
        dpAOD14->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD15 = std::make_shared<Process>("Higgs4DP_DpMass_0_3_FSR_0_0_m1000", 16);
        dpAOD15->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD16 = std::make_shared<Process>("HiggsDPZ_DpMass_0_1_FSR_0_0_m1000", 17);
        dpAOD16->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD17 = std::make_shared<Process>("SUSY_DpMass_0_3_FSR_0_0_m1000", 18);
        dpAOD17->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    auto dpAOD18 = std::make_shared<Process>("ZPrime_DpMass_0_3_FSR_0_0_m1000", 19);
        dpAOD18->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", "DarkPhoton", reader, luminosity));
    
    auto DarkPhotonSignal = std::make_shared<Process>("Dark Photon Signal", 5);
        DarkPhotonSignal->addProcess(makeBasicProcess(histVariablesBackground, filePath, "darkPhotonBaselineRun2.root", "DarkPhoton", reader, luminosity));

    auto DarkPhotonData = std::make_shared<Process>("Data", 1);
    
    auto mbp = makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018A.root", "LeptonJet" + massTarget, reader, luminosity);
    DarkPhotonData->addProcess(mbp);

    std::vector<std::shared_ptr<Process>> nanoProcesses = {
        dpAOD1, 
        dpAOD2, 
        dpAOD3, 
        dpAOD4, 
        dpAOD5, 
        dpAOD6, 
        dpAOD7, 
        dpAOD8, 
        dpAOD9, 
        dpAOD10, 
        dpAOD11, 
        dpAOD12, 
        dpAOD13, 
        dpAOD14, 
        dpAOD15, 
        dpAOD16, 
        dpAOD17, 
        dpAOD18
    };

    std::vector<std::shared_ptr<Process>> selected;
    for (int i = 0; i < static_cast<int>(nanoProcesses.size()); ++i) {
        const auto& elem = nanoProcesses[i];
        if (i >= start && i <= end) {
            selected.push_back(elem);
        }
    }

    selected.push_back(DarkPhotonSignal);
    selected.push_back(DarkPhotonData);
    
    auto leptonBackgrounds = std::make_shared<Channel>(massTarget, selected);
    getChannelsProtected().push_back(leptonBackgrounds);

}
