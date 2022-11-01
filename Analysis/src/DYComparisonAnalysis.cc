#include "CMSAnalysis/Analysis/interface/DYComparisonAnalysis.hh"
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

DYComparisonAnalysis::DYComparisonAnalysis() {
    std::vector<double> massTargets {50};
    std::vector<std::string> names = {"Muon", "Electron"};
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            auto histFinder = std::make_shared<HistogramFinder>();
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            //const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
            const std::string newFilePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
                //Add your hists here
            histFinder->addHist(HistVariable::InvariantMass, name + name + " Reco Invariant Mass Background");
            histFinder->addHist(HistVariable::SameSignMass, "GenSim Same Sign Invariant Mass");
	    histFinder->addHist(HistVariable::pT, "genSim Leading lepton pT");
            histFinder->addHist(HistVariable::MET, "MET;1");
            double luminosity = 3000;
                auto newDY = std::make_shared<Process>("Run 2", 1);
                newDY->addProcess(makeSignalProcess(histFinder, newFilePath, "newDY50.root", "dy50toinf", reader, massTarget, luminosity));
            auto oldDY = std::make_shared<Process>("Delphes", 2);
            oldDY->addProcess(makeSignalProcess(histFinder, newFilePath, "DY50.root", "dy50toinf", reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> dYProcesses = { newDY, oldDY };
            auto dYChannels = std::make_shared<Channel>(name + std::to_string((int) massTarget), dYProcesses);
            channels.push_back(dYChannels);
        }
    }
}

std::shared_ptr<Channel> DYComparisonAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}

