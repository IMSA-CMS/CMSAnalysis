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
    std::vector<double> massTargets = {1400};
    const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
    const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
    //std::vector<std::string> names = {"Muon", "Electron"};
    //for(std::string name : names) {
    for (std::string recoDecay : recoDecays){    
        for(double massTarget : massTargets) {
            std::vector<HistVariable> histVariablesBackground;
            histVariablesBackground.push_back(HistVariable::SameSignMass(recoDecay + "__hists/" + recoDecay + "_Reco Same Sign Invariant Mass"));
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/";
            //Add your hists here
            //histVariables.push_back(HistVariable::SameSignMass(name + name + " Reco Same Sign Invariant Mass"));
            double luminosity = 20;

	        auto dyBackground = std::make_shared<Process>("γ/Z -> e⁺e⁻ Background", 1);
            //doesn't contain all the recoDecays (i.e. "eeee")
            dyBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, massTarget, luminosity));
            dyBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toinf", reader, massTarget, luminosity));

            //auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
            //qcdBackground->addProcess(makeSignalProcess(histVariables, filePath, "QCD500_HiggsBackground.root", "qcd500to700", reader, massTarget, luminosity));
            
            auto higgsSignal = std::make_shared<Process>("Higgs Signal", 2);
            for (std::string genSimDecay : genSimDecays)
            {
                std::vector<HistVariable> histVariablesSignal;
                std::string decayName = genSimDecay + "_" + recoDecay;
                histVariablesSignal.push_back(HistVariable::SameSignMass(decayName + "__hists/" + decayName + "_Reco Same Sign Invariant Mass"));                
                higgsSignal->addProcess(makeSignalProcess(histVariablesSignal, filePath, "Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            }

            std::vector<std::shared_ptr<Process>> processes = {dyBackground, higgsSignal};
            auto leptonProcesses = std::make_shared<Channel>(recoDecay + std::to_string((int) massTarget), processes);
            channels.push_back(leptonProcesses);
        }
    } 
}

std::shared_ptr<Channel> HiggsCutsAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel : channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}

TH1* HiggsCutsAnalysis::getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected) const 
{
    int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int channelNumber = 0; 
    std::string name = processName;
	for (const auto& channel : channels)
	{
        std::string channelName = channel->getName();
        channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
                
        if(channelName == std::to_string((int) massTarget)) {
            channelNumber++;
            //std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
            
            TH1* channelHist = channel->findProcess(processName)->getHist(histType, scaleToExpected);
            
            if (!channelHist)
            {
                throw std::runtime_error("Histogram not found in channel: " + channel->getName());
            }

            if(channelHist->GetEntries() > 0) {
                if (channelHist == 0) {
                    throw std::runtime_error("Histogram not found in channel: " + channel->getName());
                }
                if (channelHist->GetNbinsX() > maxBinNum)
                {
                    maxBinNum = channelHist->GetNbinsX();
                }
                
                if ((channelHist->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
                {
                    maxBarWidth = (channelHist->GetXaxis()->GetBinWidth(maxBinNum));
                }
            }
            delete channelHist;
        }
	}
	
    TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	TH1* toAdd = 0;
	TList* toMerge = new TList();
    TH1::AddDirectory(kFALSE);
	for (const auto& channel : channels)	
	{
        std::string channelName = channel->getName();
        channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
		if(channelName == std::to_string((int) massTarget)) {
            toAdd = channel->findProcess(processName)->getHist(histType, scaleToExpected);
            //toAdd = dynamic_cast<TH1*>(channel->findProcess(processName)->getHist(histType, scaleToExpected)->Clone((channelName + processName).c_str()));
        }
        if(toAdd->GetEntries() > 0) {
            toMerge->Add(toAdd);
        }
	}
    TH1::AddDirectory(kTRUE);
	hist->Merge(toMerge);
	hist->SetLineColor(channels.at(0)->findProcess(processName)->getColor());
	hist->SetFillColor(channels.at(0)->findProcess(processName)->getColor());
	return hist;
}
