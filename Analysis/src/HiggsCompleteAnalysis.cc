#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
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
 
HiggsCompleteAnalysis::HiggsCompleteAnalysis() {
    //This is a mass we put into background because the
    //makeBasicProcess method requires it. It doesn't actually
    //make sense so in the future fix this
    double tempMass = 1400;

    //Actual masses for the Higgs signal
    //std::vector<double> massTargets {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400};
    std::vector<double> massTargets { 500, 600, 700, 800, 900, 1100, 1200, 1300, 1400};
    const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
    
    //const std::vector<std::string> genSimDecays{};
    //const std::vector<std::string> recoDecays{};

    const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
    //const std::vector<std::string> recoDecays{"u u", "uu", "uuu", "uuuu"};
    //const std::vector<std::string> recoDecays{"ee", "eu", "e u", "uu", "u u"};
    //const std::vector<std::string> recoDecays{"u u"};

    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/home/ichen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    //const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/"; 
    //const std::string signalFilePath = "/uscms/home/ichen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/H++MassFiles/";
    auto reader = std::make_shared<CrossSectionReader>("/uscms/homes/m/mchen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt");
    //auto reader = std::make_shared<CrossSectionReader>("/uscms/homes/m/mchen2/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/crossSections.txt");
    const std::string filePath = "/uscms/homes/s/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Higgs/"; 
    const std::string signalFilePath = "/uscms/homes/s/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Higgs/";
    // Vincents: /uscms/homes/v/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output 
    double luminosity = 1.507;

    std::vector<HistVariable> histVariablesBackground;
    std::vector<HistVariable> histVariablesData;

    std::vector<std::string> rowNames = {"ee", "eu", "e u", "uu", "u u"};
    //std::vector<std::string> rowNames = {"u u"};
    std::vector<std::string> connecters = {"_1st Highest mu- ", "_1st Highest e- "}; //_1st Highest e-
    std::vector<std::string> columnNames = {"Dxy", "Dz", "Eta", "Isolation", "Phi", "Pt"};

    for (std::string rowName : rowNames)
    {
        for (std::string connecter : connecters){

            for(std::string columnName : columnNames)
            {
                histVariablesBackground.push_back(
                    HistVariable(columnName + " " + rowName,
                    rowName + "__hists/" + rowName + connecter + columnName));
                //    "High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Pt"));
            }
        }
    }
    for (std::string rowName : rowNames)
    {
        for (std::string connecter : connecters)
        {
            for (std::string columnName : columnNames)
            {
                histVariablesData.push_back(
                    HistVariable(columnName + " " + rowName,
                    rowName + "_Pass__hists/" + rowName + "_Pass" + connecter + columnName));
                //    "High Mass and Same Sign__hists/High Mass and Same Sign_1st Highest mu- Pt"));
            }
        }
    }



    TH1::SetDefaultSumw2();
    for (std::string recoDecay : recoDecays){
        std::vector<std::shared_ptr<Process>> processes;
        //auto higgsSignal = std::make_shared<Process>("Higgs Signal", 5);
        auto higgsGroupSignal = std::make_shared<Process>("Higgs Group "+ recoDecay, 5);
        for (const auto& genSimDecay : genSimDecays)
        {
            
            for(double massTarget : massTargets) 
            {
                //auto higgsSignal = std::make_shared<Process>("Higgs Signal " + std::to_string((int) massTarget), 5);
                
                std::vector<HistVariable> histVariablesSignal;
                std::string decayName = recoDecay + "_" + genSimDecay;
                //histVariablesSignal.push_back(HistVariable::InvariantMass(decayName + "__hists/" + decayName + "_Opposite Sign Invariant Mass"));      
                histVariablesSignal.push_back(HistVariable::sameSignMass(decayName + "__hists/" + decayName + "_Reco Same Sign Invariant Mass")); 
                //histVariablesSignal.push_back(HistVariable::InvariantMass(decayName + "__hists/" + decayName + "_1st Highest mu- Eta"));                
                auto higgsProcess = makeBasicProcess(histVariablesSignal, signalFilePath, "Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity);
                auto higgsSignal = std::make_shared<Process>("Higgs signal " + genSimDecay + " " + std::to_string((int)massTarget), 1);
                higgsSignal->addProcess(higgsProcess);
                processes.push_back(higgsSignal);
                higgsGroupSignal->addProcess(higgsProcess);
            }
            
        }
        processes.push_back(higgsGroupSignal);
        
        //std::vector<HistVariable> histVariablesBackground;
        //histVariablesBackground.push_back(HistVariable::InvariantMass(recoDecay + "__hists/" + recoDecay + "_Opposite Sign Invariant Mass"));
        //histVariablesBackground.push_back(HistVariable::SameSignMass(recoDecay + "__hists/" + recoDecay + "_Same Sign Invariant Mass"));
        //histVariablesBackground.push_back(HistVariable::InvariantMass(recoDecay + "__hists/" + recoDecay + "_1st Highest mu- Eta"));
        //histVariablesBackground.push_back(HistVariable::InvariantMass(recoDecay + "__hists/" + recoDecay + "_1st Highest mu- Pt"));

        auto zzBackground = std::make_shared<Process>("ZZ Background", 3);
        zzBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "ZZ_Decay_4L_Run_2.root", "zzto4l", reader, luminosity));

        //cross sections should be all lowercase
        //auto ttBarandMultiBosonandZZBackground = std::make_shared<Process>("t#bar{t}, WW, WZ, ZZ Background", 4);
        auto ttBarandMultiBosonBackground = std::make_shared<Process>("t#bar{t}, Multiboson Background", 4);
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_NA_Decay_LL_Run_2.root", "ttbar_lep", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_W_Decay_L_Run_2.root", "ttw", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "TTbar_Boson_Z_Decay_LL_Run_2.root", "ttz", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.root", "zzz", reader, luminosity));

        //auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WW_Decay_2L_Run_2.root", "wwto2l2nu", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWW_Decay_NA_Run_2.root", "www", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.root", "wwz", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZ_Decay_3L_Run_2.root", "wzto3lnu", reader, luminosity));
        ttBarandMultiBosonBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZZ_Decay_NA_Run_2.root", "wzz", reader, luminosity));
        //ttBarandMultiBosonandZZBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.root", "zzz", reader, luminosity));
        
        auto dyBackground = std::make_shared<Process>("Drell-Yan Background", 2);
        dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, luminosity));
        dyBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toinf", reader, luminosity));
        
        
        auto qcdBackground = std::make_shared<Process>("QCD Background", 8);
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_100-200_Run_2_Year_2018.root", "QCD_100-200", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_200-300_Run_2_Year_2018.root", "QCD_200-300", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_300-500_Run_2_Year_2018.root", "QCD_300-500", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_500-700_Run_2_Year_2018.root", "QCD_500-700", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_700-1000_Run_2_Year_2018.root", "QCD_700-1000", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_1000-1500_Run_2_Year_2018.root", "QCD_1000-1500", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_1500-2000_Run_2_Year_2018.root", "QCD_1500-2000", reader, luminosity));
        qcdBackground->addProcess(makeBasicProcess(histVariablesBackground, filePath, "QCD_HTCut_2000-inf_Run_2_Year_2018.root", "QCD_2000-inf", reader, luminosity));
       

        auto higgsData = std::make_shared<Process>("Data", 1);
        // 150022816 events in Data_Trigger_SingleMuon_Year_2016B.root before TriggerCut change
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016B.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016D.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));            
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016E.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016F.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016G.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2016H.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017B.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017D.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017E.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017F.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017G.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2017H.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018A.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018B.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018D.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2022A.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2022B.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        //higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2022C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        //higgsData->addProcess(makeBasicProcess(histVariablesData, filePath, "testHiggs.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        //higgsData->addProcess(makeBasicProcess(histVariablesData, filePath, "testHiggsM.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "testlumi.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2016C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2016D.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2016F.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2016H.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2017B.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2017C.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2017D.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2017E.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));
        // higgsData->addProcess(makeBasicProcess(histVariablesBackground, filePath, "Data_Trigger_SingleElectron_Year_2017F.root", "higgs4l" + std::to_string((int) tempMass), reader, luminosity));

        processes.push_back(ttBarandMultiBosonBackground);
        processes.push_back(dyBackground);
        processes.push_back(higgsData);
        processes.push_back(qcdBackground);
        processes.push_back(zzBackground);
        auto leptonProcesses = std::make_shared<Channel>(recoDecay, processes);

        for(std::string processName : leptonProcesses->getNames()) {
				//std::cout << processName << std::endl;
				//Change this line to make the described name your signal process name.
				if(processName.substr(0, 5) == "Group" || processName.substr(0, 5) == "Higgs") {
					leptonProcesses->labelProcess("signal", processName);
                    //std::cout << "Labeled Signal: " << processName << std::endl;

				}
				// "Monte Carlo Data"
				else if(processName == "Data") { //This line is only used for complete plots
					leptonProcesses->labelProcess("data", processName);
                    //std::cout << "Labeled Data: " << processName << std::endl;
				}
				else {
					leptonProcesses->labelProcess("background", processName);
                    //std::cout << "Labeled Background: " << processName << std::endl;
				}
		}
        //leptonBackgrounds->cleanProcesses();
        getChannelsProtected().push_back(leptonProcesses);
    }
}

// bool HiggsCompleteAnalysis::checkChannelName(std::string channelName, double massTarget){
//     channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
//     return channelName == std::to_string((int) massTarget);
// }

// TH1* HiggsCompleteAnalysis::getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected, std::string channelName) const {
//     int maxBinNum = 0;
// 	double maxBarWidth = 0.0;
// 	int channelNumber = 0; 
//     std::string name = processName;
// 	for (const auto& channel : getChannels())
// 	{
//         std::string channelNameTemp = channel->getName();
//         channelNameTemp = channelNameTemp.substr((channelNameTemp.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
        
//         if(channelNameTemp == std::to_string((int) massTarget)) {
//             channelNumber++;
//             //std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
            
//             TH1* channelHist = channel->findProcess(processName)->getHist(histType, scaleToExpected);
            
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
// 	for (const auto& channel : getChannels())	
// 	{
//         //std::string channelName = channel->getName();
//         //channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
// 		//if(channelName == std::to_string((int) massTarget)) {
//         if (channelName == channel->getName()){
//             toAdd = channel->findProcess(processName)->getHist(histType, scaleToExpected);
//         //}

//         //add this error check back later! (toAdd->Reset("ICESM"))
//         //if(toAdd->GetEntries() > 0) {
//             toMerge->Add(toAdd);
//         }
// 	}
//     TH1::AddDirectory(kTRUE);
// 	hist->Merge(toMerge);
// 	hist->SetLineColor(getChannels().at(0)->findProcess(processName)->getColor());
// 	hist->SetFillColor(getChannels().at(0)->findProcess(processName)->getColor());
// 	return hist;
// }

