#include "CMSAnalysis/Analysis/interface/HiggsCutsAnalysis.hh"
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
#include "TList.h"
#include "TH1.h"

HiggsCutsAnalysis::HiggsCutsAnalysis() {
    std::vector<double> massTargets = {300, 500, 700, 900, 1100, 1300, 1500};
    std::vector<std::string> names = {"Muon", "Electron"};
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariables;
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
            //Add your hists here
            histVariables.push_back(HistVariable::SameSignMass(name + name + " Reco Same Sign Invariant Mass"));
            double luminosity = 3000;
	        auto dyBackground = std::make_shared<Process>("DY Background", 3);
            dyBackground->addProcess(makeSignalProcess(histVariables, filePath, "DY50Run2_HiggsBackground.root", "dy50toinf", reader, massTarget, luminosity));
            auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
            qcdBackground->addProcess(makeSignalProcess(histVariables, filePath, "QCD500_HiggsBackground.root", "qcd500to700", reader, massTarget, luminosity));
            auto higgsSignal = std::make_shared<Process>("Higgs Signal", 2);
            higgsSignal->addProcess(makeSignalProcess(histVariables, filePath, "Higgs" + std::to_string((int) massTarget) + "_HiggsBackground.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> backgroundProcesses = { dyBackground, qcdBackground, higgsSignal };
            auto leptonBackgrounds = std::make_shared<Channel>(name + std::to_string((int) massTarget), backgroundProcesses);
            channels.push_back(leptonBackgrounds);
        }
    } 
}

std::shared_ptr<Channel> HiggsCutsAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}
