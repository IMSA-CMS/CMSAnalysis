

#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"
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
#include "CMSAnalysis/Analysis/interface/DarkPhotonHistNameFinder.hh"
#include <memory>  
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TH1.h"
#include "TList.h"
 
DarkPhotonCompleteAnalysis::DarkPhotonCompleteAnalysis() {

    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    //const std::string filePath = "/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
	const std::string crossSectionPath = "/uscms/home/snrai/shivDirectory/CMSSW_14_0_1/src/CMSAnalysis/DataCollection/bin/crossSections.txt";
    auto reader = std::make_shared<CrossSectionReader>(crossSectionPath);
    //const std::string filePath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output_Full/"; 
    //const std::string filePath = "/eos/uscms/store/user/jpalamad/rootBackups/MLBadRange"; // Backup of some old files


    const std::string filePath = "/uscms/home/bhenning/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Output/DarkPhoton/"; 
    const std::string filePathM = "/uscms/home/mkubon/analysis/clean/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
    
    double luminosity = 137;


    TH1::SetDefaultSumw2();
    std::string massTarget = "0.3";
    //for(std::string name : names) {
    //for (std::string recoDecay : recoDecays){
      //  for(double massTarget : massTargets) {

    std::vector<std::pair<std::string, std::string>> darkPhotonFiles = {
        {"Dark Photon Baseline", "darkPhotonBaselineRun2.root"}, 
        {"Dark Photon Higgs4DP", "darkPhotonDecay_Higgs4DP.root"},
        {"Dark Photon HiggsDPZ", "darkPhotonDecay_HiggsDPZ.root"},
        {"Dark Photon SUSY", "darkPhotonDecay_SUSY.root"},
        {"Dark Photon ZPrime", "darkPhotonDecay_ZPrime.root"},
        {"Dark Photon Higgs125", "darkPhotonHiggs125.root"},
        {"Dark Photon Higgs300", "darkPhotonHiggs300.root"},
        {"Dark Photon 2FSR_0_1", "darkPhotonRun2FSR_0_1.root"},
        {"Dark Photon 2FSR_0_3", "darkPhotonRun2FSR_0_3.root"},
        {"Dark Photon DpMass_0_1", "darkPhoton_DpMass0_1.root"},
        {"Dark Photon DpMass_0_2", "darkPhoton_DpMass0_2.root"},
        {"Dark Photon DpMass_0_4", "darkPhoton_DpMass0_4.root"},
        {"Dark Photon DpMass_0_6", "darkPhoton_DpMass0_6.root"},
        {"Dark Photon DpMass_0_9", "darkPhoton_DpMass0_9.root"},
        {"Dark Photon DpMass_1_2", "darkPhoton_DpMass1_2.root"},
        {"Dark Photon DpMass_1_5", "darkPhoton_DpMass1_5.root"},
        {"Dark Photon DpMass_2_5", "darkPhoton_DpMass2_5.root"},
        {"Dark Photon DpMass_4_0", "darkPhoton_DpMass4_0.root"}
    };

    std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign", "Low Mass and Different Sign"};
    std::vector<std::string> nJets = {"1Jet", "multiJet"};


    for (auto chan : rowNames)
    {
        for (auto jetNumber : nJets)
        {
            std::string channelName = chan + "/" + jetNumber;
            //#std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};


            std::vector<HistVariable> histVariablesBackground;

            auto histVariableToFileMappingData = std::make_shared<DarkPhotonHistNameFinder>(chan, jetNumber, true);
            auto histVariableToFileMapping = std::make_shared<DarkPhotonHistNameFinder>(chan, jetNumber, false);


            std::vector<std::shared_ptr<Process>> processes;


            //cross sections should be all lowercase
            auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
            ttbarBackground->addProcess(makeBasicProcess(filePath, "TTbar.root", "ttbar_lep", reader, luminosity, histVariableToFileMapping));
            ttbarBackground->addProcess(makeBasicProcess(filePath, "TTW.root", "TTW", reader, luminosity, histVariableToFileMapping));
            ttbarBackground->addProcess(makeBasicProcess(filePath, "TTZ.root", "TTZ", reader, luminosity, histVariableToFileMapping));
           
            auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
            //zzBackground->addProcess(makeBasicProcess(filePath, "ZZ_Decay_4L_Run_2.root", "ZZTo4L", reader, luminosity));
            zzBackground->addProcess(makeBasicProcess(filePath, "ZZ.root", "ZZTo2e2mu", reader, luminosity, histVariableToFileMapping));


            auto dyBackground = std::make_shared<Process>("DY Background", 3);
            dyBackground->addProcess(makeBasicProcess(filePath, "DY10-50.root", "dy10to50", reader, luminosity, histVariableToFileMapping));
            dyBackground->addProcess(makeBasicProcess(filePath, "DY50-inf.root", "dy50toInf", reader, luminosity, histVariableToFileMapping));
           
            auto qcdBackground = std::make_shared<Process>("QCD Background", 8);
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD100-200.root", "QCD_100-200", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD200-300.root", "QCD_200-300", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD300-500.root", "QCD_300-500", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD500-700.root", "QCD_500-700", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD700-1000.root", "QCD_700-1000", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD1000-1500.root", "QCD_1000-1500", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD1500-2000.root", "QCD_1500-2000", reader, luminosity, histVariableToFileMapping));
            qcdBackground->addProcess(makeBasicProcess(filePath, "QCD2000-inf.root", "QCD_2000-inf", reader, luminosity, histVariableToFileMapping));
            //#qcdBackground->addProcess(makeBasicProcess(filePath, "QCD_HTCut_2000-Inf_Run_2_Year_2018.root", "QCD_2000-Inf", reader, luminosity));


            auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "WW.root", "WWTo2L2Nu", reader, luminosity, histVariableToFileMapping));
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "WWW.root", "WWW", reader, luminosity, histVariableToFileMapping));
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "WWZ.root", "WWZ", reader, luminosity, histVariableToFileMapping));
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "WZ.root", "WZTo3LNu", reader, luminosity, histVariableToFileMapping));
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "WZZ.root", "WWZ", reader, luminosity, histVariableToFileMapping));
            multiBosonBackground->addProcess(makeBasicProcess(filePath, "ZZZ.root", "ZZZ", reader, luminosity, histVariableToFileMapping));
           

            for (const auto& [crossSectionName, fileName] : darkPhotonFiles) 
            {
                auto darkPhotonProcess = std::make_shared<Process>(crossSectionName, 5);
                darkPhotonProcess->addProcess(makeBasicProcess(filePath, fileName, "DarkPhoton", reader, luminosity, histVariableToFileMapping));
                processes.push_back(darkPhotonProcess);
            }
           


            auto darkPhotonData = std::make_shared<Process>("Data", 1);
            darkPhotonData->addProcess(makeBasicProcess(filePath, "Muon2016.root", "Muon2016", reader, luminosity, histVariableToFileMappingData));
            darkPhotonData->addProcess(makeBasicProcess(filePath, "Muon2016APV.root", "Muon2016APV", reader, luminosity, histVariableToFileMappingData));
            darkPhotonData->addProcess(makeBasicProcess(filePath, "Muon2017.root", "Muon2017", reader, luminosity, histVariableToFileMappingData));
            darkPhotonData->addProcess(makeBasicProcess(filePath, "Muon2018.root", "Muon2018", reader, luminosity, histVariableToFileMappingData));


            processes.push_back(ttbarBackground);
            processes.push_back(zzBackground);
            processes.push_back(dyBackground);
            processes.push_back(qcdBackground);
            processes.push_back(multiBosonBackground);
            //processes.push_back(darkPhotonSignal);
            processes.push_back(darkPhotonData);

            auto leptonBackgrounds = std::make_shared<Channel>(channelName, processes);

            for(std::string processName : leptonBackgrounds->getNames()) {
                std::cout << processName << std::endl;


                if(processName.find("Dark Photon") != std::string::npos) 
                {
                    if (processName == "Dark Photon Baseline")
                    {
                        leptonBackgrounds->labelProcess("signal", processName);
                    }
                }
                else if(processName == "Data")
                { //This line is only used for complete plots
                    leptonBackgrounds->labelProcess("data", processName);
                }
                else
                {
                    leptonBackgrounds->labelProcess("background", processName);
                }
            }

            getChannelsProtected().push_back(leptonBackgrounds);
        }
    }  
}

