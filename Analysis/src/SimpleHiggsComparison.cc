#include "CMSAnalysis/Analysis/interface/SimpleHiggsComparison.hh"
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
#include "CMSAnalysis/Analysis/interface/SimpleChannel.hh"
#include "CMSAnalysis/Analysis/interface/SimpleProcess.hh"
#include <memory>
#include <iostream>
#include <vector>

SimpleHiggsComparison::SimpleHiggsComparison() {
    std::vector<double> massTargets {300, 500, 700, 900, 1100, 1300};
    std::vector<std::string> names = {"Muon", "Electron"};
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            auto histFinder = std::make_shared<HistogramFinder>();
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
	        //Add your hists here
            histFinder->addHist(HistVariable::InvariantMass, name + name + " Reco Invariant Mass Background");
            histFinder->addHist(HistVariable::SameSignMass, name + name + " Reco Same Sign Invariant Mass");
	    histFinder->addHist(HistVariable::GenSimSameSignMass, name + name + " GenSim Same Sign Invariant Mass");
            histFinder->addHist(HistVariable::pT, name + name + " Reco Leading lepton pT");
	    histFinder->addHist(HistVariable::GenSimPt, name + name + " GenSim Leading lepton pT");
            histFinder->addHist(HistVariable::MET, name + "MET");
	    auto newHiggs = std::make_shared<SimpleProcess>("Run 2", filePath, 1);
	    newHiggs->addFile("Higgs" + std::to_string((int) massTarget) + "_HiggsBackground.root", histFinder);
            auto higgsSignal = std::make_shared<SimpleProcess>("Delphes", filePath, 2);
	    higgsSignal->addFile("HiggsPick" + std::to_string((int) massTarget) + "_HiggsBackground.root", histFinder);
	    std::vector<std::shared_ptr<SimpleProcess>> higgsProcesses = { newHiggs, higgsSignal };
            auto higgsChannels = std::make_shared<SimpleChannel>(name + std::to_string((int) massTarget));
	    for(std::shared_ptr<SimpleProcess> process : higgsProcesses) {
		higgsChannels->addProcess(process);
	    }
            channels.push_back(higgsChannels);
        }
    } 
}

std::shared_ptr<SimpleChannel> SimpleHiggsComparison::getChannel(std::string name)
{
    for(std::shared_ptr<SimpleChannel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}
