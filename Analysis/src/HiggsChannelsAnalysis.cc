#include "CMSAnalysis/Analysis/interface/HiggsChannelsAnalysis.hh"
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
#include "CMSAnalysis/DataCollection/interface/Utility.hh"
#include <memory>   
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

HiggsChannelsAnalysis::HiggsChannelsAnalysis() {
    std::vector<double> massTargets = {900};
    std::vector<std::string> particles = {"e", "u", "t"};
    std::vector<std::string> names = Utility::channelMaker(particles, 4, true);
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariables;
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/gbayliss/CMSAnalysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/";
            //Add your hists here
            histVariables.push_back(HistVariable::SameSignMass(name + "Reco Same Sign Invariant Mass"));
            double luminosity = 3000;
            auto higgsSignal = std::make_shared<Process>("Higgs Signal", 2);
            higgsSignal->addProcess(makeSignalProcess(histVariables, filePath, "Hist_DoublyChargedHiggs.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> backgroundProcesses = { higgsSignal };
            auto leptonBackgrounds = std::make_shared<Channel>(name + std::to_string((int) massTarget), backgroundProcesses);
            channels.push_back(leptonBackgrounds);
        }
    } 
}

std::shared_ptr<Channel> HiggsChannelsAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}

