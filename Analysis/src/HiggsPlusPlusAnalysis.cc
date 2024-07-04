// #include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
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
// #include <memory>	
// #include <iostream>
// #include <vector>
// #include "TH1.h"
// #include "TList.h"

// HiggsPlusPlusAnalysis::HiggsPlusPlusAnalysis() {
//     std::vector<double> massTargets {300, 500, 700, 900, 1100, 1300, 1500, 1700};
//     //std::vector<double> massTargets {900};
//     std::vector<std::string> names = {"Muon", "Electron"};
    
//     //std::vector<std::string> particles = {"e", "u"};
//     //std::vector<std::string> names = Utility::channelMaker(particles, 4, true);

//     TH1::SetDefaultSumw2();
//     for(std::string name : names) {
//         for(double massTarget : massTargets) {
//             std::vector<HistVariable> histVariables;
//             std::vector<HistVariable> fitHistVariables;
//             //Change this file to your folder to use your own cross sections
//             auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
//             //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
//             const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
//             //Add your hists here
//             histVariables.push_back(HistVariable::InvariantMass("Cut4" + name + name + " Reco Invariant Mass Background"));
// 	        histVariables.push_back(HistVariable::SameSignMass("Cut4" + name + name + " Reco Same Sign Invariant Mass"));
// 	        histVariables.push_back(HistVariable::Pt("Cut4Leading lepton pT"));
// 	        histVariables.push_back(HistVariable::MET("Cut4MET"));
// 	        fitHistVariables.push_back(HistVariable::InvariantMass("Cut4Reco Invariant Mass Background"));
// 	        fitHistVariables.push_back(HistVariable::SameSignMass("Cut4Reco Same Sign Invariant Mass"));
// 	        fitHistVariables.push_back(HistVariable::Pt("Cut4Leading lepton pT"));
// 	        fitHistVariables.push_back(HistVariable::MET("Cut4MET"));
//             double luminosity = 3000;
//             auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
//             ttbarBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/TTBarPick500.root", "/30SelectBackgroundRuns/TTBar.root", "ttbar_lep", reader, massTarget, luminosity));
//             auto zzBackground = std::make_shared<Process>("ZZ Background", 5);
//             zzBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/ZZPick4.root", "30SelectBackgroundRuns/ZZ.root", "zz4l", reader, massTarget, luminosity));
//             auto dyBackground = std::make_shared<Process>("DY Background", 3);
//             dyBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/DY10.root", "30SelectBackgroundRuns/DY10.root", "dy10to50", reader, massTarget, luminosity));
//             dyBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/DY50.root", "30SelectBackgroundRuns/DY50.root", "dy50toinf", reader, massTarget, luminosity));
//             auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
//             qcdBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/QCD500.root", "30SelectBackgroundRuns/QCD500.root", "qcd500to700", reader, massTarget, luminosity));
//             qcdBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/QCD700.root", "30SelectBackgroundRuns/QCD700.root", "qcd700to1000", reader, massTarget, luminosity));
//             qcdBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/QCD1000.root", "30SelectBackgroundRuns/QCD1000.root", "qcd1000to1500", reader, massTarget, luminosity));
//             qcdBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/QCD1500.root", "30SelectBackgroundRuns/QCD1500.root", "qcd1500to2000", reader, massTarget, luminosity));
//             qcdBackground->addProcess(makeBasicProcessFit(histVariables, fitHistVariables, filePath, "BackgroundRunCuts/QCD2000.root", "30SelectBackgroundRuns/QCD2000.root", "qcd2000toinf", reader, massTarget, luminosity));
//             auto higgsSignal = std::make_shared<Process>("Higgs Signal", 1);
//             higgsSignal->addProcess(makeBasicProcess(histVariables, filePath, "HiggsRunCuts/Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
//             std::vector<std::shared_ptr<Process>> backgroundProcesses = { ttbarBackground, zzBackground, dyBackground, qcdBackground, higgsSignal };
//             auto leptonBackgrounds = std::make_shared<Channel>(name + std::to_string((int) massTarget), backgroundProcesses);
//             channels.push_back(leptonBackgrounds);
//         }
//     } 
// }

// SingleProcess HiggsPlusPlusAnalysis::makeBasicProcessFit(std::vector<HistVariable> histVariables, std::vector<HistVariable> fitHistVariables, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections) 
// {
//     auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histVariables);
//     auto fitInput = std::make_shared<RootFileInput>(filePathway + fitFileName, fitHistVariables);
//     auto histEstimator = std::make_shared<FitEstimator>(massTarget, fitInput);
//     return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity, corrections);
// }
