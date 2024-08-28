
#include "CMSAnalysis/Analysis/interface/DarkPhotonNoCutAnalysis.hh"
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
 
DarkPhotonNoCutAnalysis::DarkPhotonNoCutAnalysis(const std::string filePath) {
    
    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    //const std::string filePath = "/uscms/home/maxchen/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
	//auto reader = std::make_shared<CrossSectionReader>("/uscms/home/mkubon/analysis/clean/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    auto reader = std::make_shared<CrossSectionReader>("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    // const std::string filePath = "/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLEval_MuPtCut_Config1/"; 
    double luminosity = 20;

    TH1::SetDefaultSumw2();
    std::string massTarget = "0.3";
    //for(std::string name : names) {
    //for (std::string recoDecay : recoDecays){
      //  for(double massTarget : massTargets) {

    // std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign"};
    // std::vector<std::string> LJVars = {
    //     "1st Highest mu- Pt"
    // };

    std::vector<std::string> LJVars = {
        "1st Highest mu- Pt", 
        "1st Highest mu- Eta", 
        "2nd Highest mu- Pt", 
        "2nd Highest mu- Eta", 
        "mu- Opposite Sign Invariant Mass", 
        "1st Highest Lepton Jet Pt", 
        "1st Highest Lepton Jet Eta", 
        "1st Highest Lepton Jet Lepton Jet Delta R"
    };

    //#std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};

    std::vector<HistVariable> histVariablesBackground;

    for (std::string LJVar : LJVars)
    {
        histVariablesBackground.push_back(
            HistVariable(LJVar,
            "_hists/" + LJVar));
    }

    //histVariablesBackground.push_back(HistVariable("Pt High Mass and Same Sign","High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Pt"));
    //histVariablesBackground.push_back(HistVariable("Eta High Mass and Same Sign", "High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Eta"));

    //histVariablesBackground.push_back(HistVariable("Pt Low Mass and Same Sign","Low Mass and Same Sign__hists/Low Mass and Same Sign_1st Highest mu- Pt"));
    //histVariablesBackground.push_back(HistVariable("Eta Low Mass and Same Sign","Low Mass and Same Sign__hists/Low Mass and Same Sign_1st Highest mu- Eta"));

    //histVariablesBackground.push_back(HistVariable("Pt High Mass and Different Signs","High Mass and Different Signs__hists/High Mass and Different Signs_1st Highest mu- Pt"));      
    //histVariablesBackground.push_back(HistVariable("Eta High Mass and Different Signs", "High Mass and Different Signs__hists/High Mass and Different Signs_1st Highest mu- Eta")); 

    //cross sections should be all lowercase
    auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_NA_Decay_LL_Run_2.root", "ttbar_lep", reader, luminosity));
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_W_Decay_L_Run_2.root", "TTW", reader, luminosity));
    ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_Z_Decay_LL_Run_2.root", "TTZ", reader, luminosity));
    
    auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
    //zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4L_Run_2.root", "ZZTo4L", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2mu_Run_2.root", "ZZTo2e2mu", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2tau_Run_2.root", "ZZTo2e2tau", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2mu2tau_Run_2.root", "ZZTo2mu2tau", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4e_Run_2.root", "ZZTo4e", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4mu_Run_2.root", "ZZTo4mu", reader, luminosity));
    zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4tau_Run_2.root", "ZZTo4tau", reader, luminosity));

    auto dyBackground = std::make_shared<Process>("DY Background", 3);
    dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, luminosity));
    dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toInf", reader, luminosity));
    
    auto qcdBackground = std::make_shared<Process>("QCD Background", 8);
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_100-200_Run_2_Year_2018.root", "QCD_100-200", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_200-300_Run_2_Year_2018.root", "QCD_200-300", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_300-500_Run_2_Year_2018.root", "QCD_300-500", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_500-700_Run_2_Year_2018.root", "QCD_500-700", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_700-1000_Run_2_Year_2018.root", "QCD_700-1000", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_1000-1500_Run_2_Year_2018.root", "QCD_1000-1500", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_1500-2000_Run_2_Year_2018.root", "QCD_1500-2000", reader, luminosity));
    qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_2000-inf_Run_2_Year_2018.root", "QCD_2000-inf", reader, luminosity));
    //#qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_2000-Inf_Run_2_Year_2018.root", "QCD_2000-Inf", reader, luminosity));

    auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WW_Decay_2L_Run_2.root", "WWTo2L2Nu", reader, luminosity));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWW_Decay_NA_Run_2.root", "WWW", reader, luminosity));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.root", "WWZ", reader, luminosity));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZ_Decay_3L_Run_2.root", "WZTo3LNu", reader, luminosity));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZZ_Decay_NA_Run_2.root", "WWZ", reader, luminosity));
    multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.root", "ZZZ", reader, luminosity));
    
    auto DarkPhotonSignal = std::make_shared<Process>("Dark Photon Signal", 5);
    DarkPhotonSignal->addProcess(makeBasicProcess(histVariablesBackground, filePath, "darkPhotonBaselineRun2.root", "DarkPhoton", reader, luminosity));

    auto DarkPhotonData = std::make_shared<Process>("Data", 1);
    //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "SingleMuonRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + massTarget, reader, luminosity));
    //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "SingleElectronRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + massTarget, reader, luminosity));
    
    auto mbp = makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018A.root", "LeptonJet" + massTarget, reader, luminosity);
    DarkPhotonData->addProcess(mbp);

    std::vector<std::shared_ptr<Process>> backgroundProcesses = {ttbarBackground, zzBackground, dyBackground, qcdBackground, DarkPhotonSignal, DarkPhotonData};
    
    auto leptonBackgrounds = std::make_shared<Channel>(massTarget, backgroundProcesses);
    //leptonBackgrounds->cleanProcesses();
    getChannelsProtected().push_back(leptonBackgrounds);

}