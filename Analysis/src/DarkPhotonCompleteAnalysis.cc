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
    auto reader = std::make_shared<CrossSectionReader>("/uscms/home/mkubon/analysis/clean/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    const std::string filePath = "/uscms/home/mkubon/analysis/clean/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/"; 
    double luminosity = 20;

    TH1::SetDefaultSumw2();
    double massTarget = 0.3;
    //for(std::string name : names) {
    //for (std::string recoDecay : recoDecays){
      //  for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariablesBackground;
            histVariablesBackground.push_back(HistVariable::InvariantMass("_hists/1st Highest Lepton Jet Pt"));
            
            //cross sections should be all lowercase
            auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
            ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_NA_Decay_LL_Run_2.root", "ttbar_lep", reader, massTarget, luminosity));
            ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_W_Decay_L_Run_2.root", "TTW", reader, massTarget, luminosity));
            ttbarBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_Z_Decay_LL_Run_2.root", "TTZ", reader, massTarget, luminosity));
            
            auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
            //zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4L_Run_2.root", "ZZTo4L", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2mu_Run_2.root", "ZZTo2e2mu", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2tau_Run_2.root", "ZZTo2e2tau", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_2mu2tau_Run_2.root", "ZZTo2mu2tau", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4e_Run_2.root", "ZZTo4e", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4mu_Run_2.root", "ZZTo4mu", reader, massTarget, luminosity));
            zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4tau_Run_2.root", "ZZTo4tau", reader, massTarget, luminosity));

            auto dyBackground = std::make_shared<Process>("DY Background", 3);
            dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, massTarget, luminosity));
            dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toInf", reader, massTarget, luminosity));
            
            auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WW_Decay_2L_Run_2.root", "WWTo2L2Nu", reader, massTarget, luminosity));
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWW_Decay_NA_Run_2.root", "WWW", reader, massTarget, luminosity));
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.root", "WWZ", reader, massTarget, luminosity));
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZ_Decay_3L_Run_2.root", "WZTo3LNu", reader, massTarget, luminosity));
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZZ_Decay_NA_Run_2.root", "WWZ", reader, massTarget, luminosity));
            multiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.root", "ZZZ", reader, massTarget, luminosity));
            
            auto DarkPhotonSignal = std::make_shared<Process>("Dark Photon Signal", 5);
           
            
                std::vector<HistVariable> histVariablesSignal;
                histVariablesSignal.push_back(HistVariable::InvariantMass("_hists/1st Highest Lepton Jet Pt"));                
                DarkPhotonSignal->addProcess(makeBasicProcess(histVariablesSignal, filePath, "darkPhotonBaselineRun2.root", "DarkPhoton", reader, massTarget, luminosity));
             
            // std::vector<std::shared_ptr<Correction>> corrections = {};
            // auto correction = std::make_shared<ConstantCorrection>(2);
            //corrections.push_back(correction);

            auto DarkPhotonData = std::make_shared<Process>("Data", 1);
            //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "SingleMuonRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "SingleElectronRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            DarkPhotonData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018A.root", "LeptonJet" + std::to_string((int) massTarget), reader, massTarget, luminosity));

            std::vector<std::shared_ptr<Process>> backgroundProcesses = {ttbarBackground, zzBackground, dyBackground, multiBosonBackground, DarkPhotonSignal, DarkPhotonData};
            
            auto leptonBackgrounds = std::make_shared<Channel>(std::to_string((int) massTarget), backgroundProcesses);
            //leptonBackgrounds->cleanProcesses();
            channels.push_back(leptonBackgrounds);

}

// bool DarkPhotonCompleteAnalysis::checkChannelName(std::string channelName, double massTarget){
//     return channelName == std::to_string((int) massTarget);
// }
// TH1* DarkPhotonCompleteAnalysis::getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected, std::string channelName) const {
//     int maxBinNum = 0;
// 	double maxBarWidth = 0.0;
// 	int channelNumber = 0; 
//     std::string name = processName;
// 	for (const auto& channel : channels)
// 	{
//         std::string channelName = channel->getName();
//         if(channelName == std::to_string((int) massTarget)) {
//             channelNumber++;
//             //std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
            
//             TH1* channelHist = channel->findProcess(processName)->getHist(histType, scaleToExpected);
//             std::cout << "number for channelHist GetNBinsX = " << channelHist->GetNbinsX() << "\n";
//             if (!channelHist)
//             {
//                 throw std::runtime_error("Histogram not found in channel: " + channel->getName());
//             }

//             if(channelHist->GetEntries() > 0) {
//                 if (channelHist == 0) {
//                     throw std::runtime_error("Histogram not found in channel: " + channel->getName());
//                 }
//                 if (channelHist->GetNbinsX() > maxBinNum)
//                 {
//                     maxBinNum = channelHist->GetNbinsX();
                   
//                 }
                
//                 if ((channelHist->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
//                 {
//                     maxBarWidth = (channelHist->GetXaxis()->GetBinWidth(maxBinNum));
                    
//                 }
//             }
//             delete channelHist;
//         }
// 	}
	
//     TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
// 	TH1* toAdd = 0;
// 	TList* toMerge = new TList();
//     TH1::AddDirectory(kFALSE);
// 	for (const auto& channel : channels)	
// 	{
//         std::string channelName = channel->getName();
// 		if (!toAdd) {
//             toAdd = channel->findProcess(processName)->getHist(histType, scaleToExpected);
//             //toAdd = dynamic_cast<TH1*>(channel->findProcess(processName)->getHist(histType, scaleToExpected)->Clone((channelName + processName).c_str()));
//         }
//         if(toAdd->GetEntries() > 0) {
//             toMerge->Add(toAdd);
//         }
// 	}
//     TH1::AddDirectory(kTRUE);
// 	hist->Merge(toMerge);
// 	hist->SetLineColor(channels.at(0)->findProcess(processName)->getColor());
// 	hist->SetFillColor(channels.at(0)->findProcess(processName)->getColor());
// 	return hist;
// }
