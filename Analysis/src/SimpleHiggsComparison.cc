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
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/SimpleChannel.hh"
#include "CMSAnalysis/Analysis/interface/SimpleProcess.hh"
#include <memory>
#include <iostream>
#include <vector>

SimpleHiggsComparison::SimpleHiggsComparison() {
    std::vector<double> massTargets {1400};
    const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
    const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
    //std::vector<std::string> names = {"Muon", "Electron"};
    //for(std::string name : names) {
    for (std::string recoDecay : recoDecays){
        for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariables;
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/";
	        //Add your hists here
            histVariables.push_back(HistVariable::invariantMass(recoDecay + "__hists/" + recoDecay + "_Reco Invariant Mass Background"));
            histVariables.push_back(HistVariable::sameSignMass(recoDecay + "__hists/" + recoDecay + "_Reco Same Sign Invariant Mass"));
	        histVariables.push_back(HistVariable::genSimSameSignMass(recoDecay + "__hists/" + recoDecay + "_GenSim Same Sign Invariant Mass"));
            histVariables.push_back(HistVariable::pt(recoDecay + "__hists/" + recoDecay + "_Reco Leading lepton pT"));
	        histVariables.push_back(HistVariable::genSimPt(recoDecay + "__hists/" + recoDecay + "_GenSim Leading lepton pT"));
            histVariables.push_back(HistVariable::mET(recoDecay + "__hists/" + recoDecay + "_MET"));
	    
        auto newHiggs = std::make_shared<SimpleProcess>("Run 2", filePath, 1);
	    newHiggs->addFile("Higgs" + std::to_string((int) massTarget) + ".root", histVariables);
        
        auto higgsSignal = std::make_shared<SimpleProcess>("Delphes", filePath, 2);
	    higgsSignal->addFile("HiggsPick" + std::to_string((int) massTarget) + ".root", histVariables);
	    std::vector<std::shared_ptr<SimpleProcess>> higgsProcesses = { newHiggs, higgsSignal };
            auto higgsChannels = std::make_shared<SimpleChannel>(recoDecay + std::to_string((int) massTarget));
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
