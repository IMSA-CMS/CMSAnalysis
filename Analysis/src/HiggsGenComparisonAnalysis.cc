#include "CMSAnalysis/Analysis/interface/HiggsGenComparisonAnalysis.hh"
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
#include <memory>
#include <iostream>
#include <vector>

HiggsGenComparisonAnalysis::HiggsGenComparisonAnalysis() {
    std::vector<double> massTargets {300, 500, 700, 900, 1100, 1300};
    std::vector<std::string> names = {"Muon", "Electron"};
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariables;
            std::vector<HistVariable> newHiggsHistVariables;
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            //const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
            const std::string newFilePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
	        //Add your hists here
            //histVariables.push_back(HistVariable::InvariantMass, "Cut4" + name + name + " Reco Invariant Mass Background");
            //histVariables.push_back(HistVariable::SameSignMass, "Cut4" + name + name + " Reco Same Sign Invariant Mass");
            //histVariables.push_back(HistVariable::pT, "Cut4Leading lepton pT");
            //histVariables.push_back(HistVariable::MET, "Cut4MET");
            //newHiggsHistVariables.push_back(HistVariable::InvariantMass, name + name + " Reco Invariant Mass Background");
            newHiggsHistVariables.push_back(HistVariable::SameSignMass("GenSim Same Sign Invariant Mass"));
            newHiggsHistVariables.push_back(HistVariable::Pt("genSim Leading lepton pT"));
            //newHiggsHistVariables.push_back(HistVariable::MET, "MET;1");
            double luminosity = 3000;
	        auto newHiggs = std::make_shared<Process>("Run 2", 1);
	        newHiggs->addProcess(makeSignalProcess(newHiggsHistVariables, newFilePath, "run2Higgs" + std::to_string((int) massTarget) + "GenSim.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            auto higgsSignal = std::make_shared<Process>("Old Higgs", 2);
            higgsSignal->addProcess(makeSignalProcess(newHiggsHistVariables, newFilePath, "delphesHiggs" + std::to_string((int) massTarget) + "GenSim.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> higgsProcesses = { newHiggs, higgsSignal };
            auto higgsChannels = std::make_shared<Channel>(name + std::to_string((int) massTarget), higgsProcesses);
            channels.push_back(higgsChannels);
        }
    } 
}

std::shared_ptr<Channel> HiggsGenComparisonAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}
