#include "CMSAnalysis/Analysis/interface/HiggsComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/HistogramFinder.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <memory>
#include <iostream>
#include <vector>

HiggsComparisonAnalysis::HiggsComparisonAnalysis() {
    std::vector<double> massTargets {300, 500, 700, 900, 1100, 1300, 1500};
    std::vector<std::string> names = {"Muon", "Electron"};
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            auto histFinder = std::make_shared<HistogramFinder>();
            auto newHiggsHistFinder = std::make_shared<HistogramFinder>();
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
            const std::string newFilePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
	        //Add your hists here
            histFinder->addHist(HistVariable::InvariantMass, "Cut4" + name + name + " Reco Invariant Mass Background");
            histFinder->addHist(HistVariable::SameSignMass, "Cut4" + name + name + " Reco Same Sign Invariant Mass");
            histFinder->addHist(HistVariable::pT, "Cut4Leading lepton pT");
            histFinder->addHist(HistVariable::MET, "Cut4MET");
            newHiggsHistFinder->addHist(HistVariable::InvariantMass, name + name + " Reco Invariant Mass Background");
            newHiggsHistFinder->addHist(HistVariable::SameSignMass, name + name + " Reco Same Sign Invariant Mass");
            newHiggsHistFinder->addHist(HistVariable::pT, "Leading lepton pT");
            newHiggsHistFinder->addHist(HistVariable::MET, "MET;1");
            double luminosity = 3000;
	        auto newHiggs = std::make_shared<Process>("New Higgs", 1);
	        newHiggs->addProcess(makeSignalProcess(newHiggsHistFinder, newFilePath, "Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            auto higgsSignal = std::make_shared<Process>("Old Higgs", 2);
            higgsSignal->addProcess(makeSignalProcess(histFinder, filePath, "HiggsRunCuts/Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> higgsProcesses = { newHiggs, higgsSignal };
            auto higgsChannels = std::make_shared<Channel>(name + std::to_string((int) massTarget), higgsProcesses);
            channels.push_back(higgsChannels);
        }
    } 
}

std::shared_ptr<Channel> HiggsComparisonAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}
