// #include "CMSAnalysis/Analysis/interface/HiggsChannelsAnalysis.hh"
// #include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
// #include "CMSAnalysis/Analysis/interface/Channel.hh"
// #include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
// #include "CMSAnalysis/Analysis/interface/Estimator.hh"
// #include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
// #include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
// #include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
// #include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
// #include "CMSAnalysis/Analysis/interface/Process.hh"
// #include "CMSAnalysis/Analysis/interface/HistVariable.hh"
// #include "CMSAnalysis/Utility/interface/Utility.hh"
// #include <memory>   
// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <string>
// #include <cmath>
// #include "TList.h"
// #include "TH1.h"

// HiggsChannelsAnalysis::HiggsChannelsAnalysis() {
//     std::vector<double> massTargets = {900};
//     std::vector<std::string> particles = {"e", "u", "t"};
//     std::vector<std::string> names = Utility::channelMaker(particles, 4, true);
//     for(std::string name : names) {
//         for(double massTarget : massTargets) {
//             std::vector<HistVariable> histVariables;
//             //Change this file to your folder to use your own cross sections
//             auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
//             //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
//             const std::string filePath = "/uscms/home/gbayliss/CMSAnalysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/";
//             //Add your hists here
//             histVariables.push_back(HistVariable::SameSignMass(name + "Reco Same Sign Invariant Mass"));
//             //double luminosity = 3000;
//             auto higgsSignal = std::make_shared<Process>("Higgs Signal", 2);
//             //higgsSignal->addProcess(makeBasicProcess(histVariables, filePath, "Hist_DoublyChargedHiggs.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
//             std::vector<std::shared_ptr<Process>> backgroundProcesses = { higgsSignal };
//             auto leptonBackgrounds = std::make_shared<Channel>(name + std::to_string((int) massTarget), backgroundProcesses);
//             channels.push_back(leptonBackgrounds);
//         }
//     } 
// }

// TH1* HiggsChannelsAnalysis::getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected, std::string channelName) const {
//     int maxBinNum = 0;
// 	double maxBarWidth = 0.0;
// 	int channelNumber = 0;
//     std::string name = processName;
// 	for (const auto& channel : channels)
// 	{
//         std::string channelName = channel->getName();
//         channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
//         if(channelName == std::to_string((int) massTarget)) {
//             channelNumber++;
//             //std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
//             TH1* channelHist = channel->findProcess(processName)->getHist(histType, scaleToExpected);
//             if (channelHist == 0) {
//                 throw std::runtime_error("Histogram not found in channel: " + channel->getName());
//             }
//             if (channelHist->GetNbinsX() > maxBinNum)
//             {
//                 maxBinNum = channelHist->GetNbinsX();
//             }
//             if ((channelHist->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
//             {
//                 maxBarWidth = (channelHist->GetXaxis()->GetBinWidth(maxBinNum));
//             }
//             delete channelHist;
//         }
// 	}
// 	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
// 	TH1* toAdd;
// 	TList* toMerge = new TList;
//     TH1::AddDirectory(kFALSE);
// 	for (const auto& channel : channels)	
// 	{
//         std::string channelName = channel->getName();
//         channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
// 		if(channelName == std::to_string((int) massTarget)) {
//             toAdd = channel->findProcess(processName)->getHist(histType, scaleToExpected);
//         }
//         toMerge->Add(toAdd);
// 	}
//     TH1::AddDirectory(kTRUE);
// 	hist->Merge(toMerge);
// 	hist->SetLineColor(channels.at(0)->findProcess(processName)->getColor());
// 	hist->SetFillColor(channels.at(0)->findProcess(processName)->getColor());
// 	return hist;
// }
