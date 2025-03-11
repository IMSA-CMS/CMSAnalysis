
#include "CMSAnalysis/Analysis/interface/DarkPhotonInputAnalysis.hh"
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
 
DarkPhotonInputAnalysis::DarkPhotonInputAnalysis(const std::string filePath) {
    
    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    //const std::string filePath = "/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
	auto reader = std::make_shared<CrossSectionReader>("/uscms/home/mkubon/analysis/clean/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    // const std::string filePath = "/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLEval_MuPtCut_Config1/"; 
    double luminosity = 20;

    TH1::SetDefaultSumw2();
    std::string massTarget = "0.3";
    //for(std::string name : names) {
    //for (std::string recoDecay : recoDecays){
      //  for(double massTarget : massTargets) {

    std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign"};

    // For when control region filter is off
    std::vector<std::string> LJVars = {"Input Delta Pt Values", "Input Leading Pt Values", "Input Delta R Values", "Input Eta Values", "Input MaxIsolation Values", "Input Sum Pt Values", "Input nParticles Values", "1st Highest Lepton Jet Pt", "1st Highest mu- Pt", "1st Highest mu- Eta", "2nd Highest mu- Pt", "2nd Highest mu- Eta", "1st Highest Lepton Jet Lepton Jet Delta R"};

    //#std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};

    std::vector<HistVariable> histVariablesBackground;
    std::map<std::string, std::string> histVariableToFileMapping;

    for (std::string rowName : rowNames)
    {
        for (std::string LJVar : LJVars)
        {
            histVariableToFileMapping[LJVar + " " + rowName] = rowName + "__hists/" + rowName + "_" + LJVar;
            histVariablesBackground.push_back(
                //HistVariable(LJVar + " " + rowName, rowName + "__hists/" + rowName + "_" + LJVar));
                HistVariable(LJVar + " " + rowName));
        }
    }

    //histVariablesBackground.push_back(HistVariable("Pt High Mass and Same Sign","High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Pt"));
    //histVariablesBackground.push_back(HistVariable("Eta High Mass and Same Sign", "High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Eta"));

    //histVariablesBackground.push_back(HistVariable("Pt Low Mass and Same Sign","Low Mass and Same Sign__hists/Low Mass and Same Sign_1st Highest mu- Pt"));
    //histVariablesBackground.push_back(HistVariable("Eta Low Mass and Same Sign","Low Mass and Same Sign__hists/Low Mass and Same Sign_1st Highest mu- Eta"));

    //histVariablesBackground.push_back(HistVariable("Pt High Mass and Different Signs","High Mass and Different Signs__hists/High Mass and Different Signs_1st Highest mu- Pt"));      
    //histVariablesBackground.push_back(HistVariable("Eta High Mass and Different Signs", "High Mass and Different Signs__hists/High Mass and Different Signs_1st Highest mu- Eta")); 

    //cross sections should be all lowercase
    auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar.root", "ttbar_lep", reader, luminosity, histVariableToFileMapping));
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTW.root", "TTW", reader, luminosity, histVariableToFileMapping));
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTZ.root", "TTZ", reader, luminosity, histVariableToFileMapping));
    
    auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
    //zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4L_Run_2.root", "ZZTo4L", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ.root", "ZZTo2e2mu", reader, luminosity, histVariableToFileMapping));

    auto dyBackground = std::make_shared<Process>("DY Background", 3);
    dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DY10-50.root", "dy10to50", reader, luminosity, histVariableToFileMapping));
    dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "DY50-inf.root", "dy50toInf", reader, luminosity, histVariableToFileMapping));
    
    auto qcdBackground = std::make_shared<Process>("QCD Background", 8);
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD100-200.root", "QCD_100-200", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD200-300.root", "QCD_200-300", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD300-500.root", "QCD_300-500", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD500-700.root", "QCD_500-700", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD700-1000.root", "QCD_700-1000", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD1000-1500.root", "QCD_1000-1500", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD1500-2000.root", "QCD_1500-2000", reader, luminosity, histVariableToFileMapping));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD2000-inf.root", "QCD_2000-inf", reader, luminosity, histVariableToFileMapping));
    //#qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_2000-Inf_Run_2_Year_2018.root", "QCD_2000-Inf", reader, luminosity));

    auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "WW.root", "WWTo2L2Nu", reader, luminosity, histVariableToFileMapping));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "WWW.root", "WWW", reader, luminosity, histVariableToFileMapping));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "WWZ.root", "WWZ", reader, luminosity, histVariableToFileMapping));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "WZ.root", "WZTo3LNu", reader, luminosity, histVariableToFileMapping));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "WZZ.root", "WWZ", reader, luminosity, histVariableToFileMapping));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZZ.root", "ZZZ", reader, luminosity, histVariableToFileMapping));
    
    auto darkPhotonSignal = std::make_shared<Process>("Dark Photon Signal", 5);
    darkPhotonSignal->addProcess(makeBasicProcess(histVariablesBackground, filePath, "darkPhotonBaselineRun2.root", "DarkPhoton", reader, luminosity, histVariableToFileMapping));

    auto darkPhotonData = std::make_shared<Process>("Data", 1);
    darkPhotonData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Muon2016.root", "Muon2016", reader, luminosity, histVariableToFileMapping));
    darkPhotonData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Muon2016APV.root", "Muon2016APV", reader, luminosity, histVariableToFileMapping));
    darkPhotonData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Muon2017.root", "Muon2017", reader, luminosity, histVariableToFileMapping));
    darkPhotonData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Muon2018.root", "Muon2018", reader, luminosity, histVariableToFileMapping));

    std::vector<std::shared_ptr<Process>> backgroundProcesses = {ttbarBackground, zzBackground, dyBackground, qcdBackground, darkPhotonSignal, darkPhotonData};
    
    auto leptonBackgrounds = std::make_shared<Channel>(massTarget, backgroundProcesses);
    //leptonBackgrounds->cleanProcesses();
    getChannelsProtected().push_back(leptonBackgrounds);

}