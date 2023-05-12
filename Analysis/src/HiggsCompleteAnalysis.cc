#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
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
#include <string>
#include <cmath>
#include "TH1.h"
#include "TList.h"

HiggsCompleteAnalysis::HiggsCompleteAnalysis() {
    std::vector<double> massTargets {900};
    std::vector<std::string> particles = {"e", "u"};
    std::vector<std::string> names = Utility::channelMaker(particles, 4, true);
    TH1::SetDefaultSumw2();
    for(std::string name : names) {
        for(double massTarget : massTargets) {
            //std::cout << name + particleString + std::to_string((int) massTarget) << std::endl;
            std::vector<HistVariable> histVariables;
            //Change this file to your folder to use your own cross sections
            auto reader = std::make_shared<CrossSectionReader>("/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/Analysis/bin/crossSections.txt");
            //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
            const std::string filePath = "/uscms/home/fciancio/practice/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
            //Add your hists here
            //histVariables.push_back(HistVariable::InvariantMass(name + "_hists/" + name + "Muon" + " Reco Invariant Mass Background"));
            //std::cout << particleString << name << particleString << " Reco Invariant Mass Background" << std::endl;
            histVariables.push_back(HistVariable::SameSignMass(name + "_hists/" + name + "Muon" + " Reco Same Sign Invariant Mass"));
            //histVariables.push_back(HistVariable::Pt(name + "_hists/" + name + "Muon" + " Reco Leading lepton pT"));
            //histVariables.push_back(HistVariable::MET(particleString + name + "MET"));
            double luminosity = 3000;
            auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
            ttbarBackground->addProcess(makeSignalProcess(histVariables, filePath, "TTBar_HiggsBackground.root", "ttbar_lep", reader, massTarget, luminosity));
            auto zzBackground = std::make_shared<Process>("ZZ Background", 5);
            zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZ_HiggsBackground.root", "zz4l", reader, massTarget, luminosity));
            auto dyBackground = std::make_shared<Process>("DY Background", 3);
            dyBackground->addProcess(makeSignalProcess(histVariables, filePath, "DY50_HiggsBackground.root", "dy10to50", reader, massTarget, luminosity));
            auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
            qcdBackground->addProcess(makeSignalProcess(histVariables, filePath, "QCD500_HiggsBackground.root", "qcd500to700", reader, massTarget, luminosity));
            auto higgsSignal = std::make_shared<Process>("Higgs Signal", 5);
            higgsSignal->addProcess(makeSignalProcess(histVariables, filePath, "Higgs" + std::to_string((int) massTarget) + "_HiggsBackground.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            auto higgsData = std::make_shared<Process>("Higgs Data", 1);
            higgsData->addProcess(makeSignalProcess(histVariables, filePath + "../python/", "data.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
            std::vector<std::shared_ptr<Process>> backgroundProcesses = { ttbarBackground, zzBackground, dyBackground, qcdBackground, higgsSignal, higgsData };
            auto leptonBackgrounds = std::make_shared<Channel>(name  + std::to_string((int) massTarget), backgroundProcesses);
            channels.push_back(leptonBackgrounds);
        }
    }
}

std::shared_ptr<Channel> HiggsCompleteAnalysis::getChannel(std::string name)
{
    for(std::shared_ptr<Channel> singleChannel: channels) {
        if(singleChannel->getName() == name) {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}

TH1* HiggsCompleteAnalysis::getHiggsHist(std::string histType, double massTarget, bool scaleToExpected) const {
    int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int channelNumber = 0;
    std::string name = "Higgs";
	for (const auto& channel : channels)
	{
        std::string channelName = channel->getName();
        channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
        if(channelName == std::to_string((int) massTarget)) {
            channelNumber++;
            std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
            if (channelHists.size() == 0) {
                throw std::runtime_error("Histogram not found in channel: " + channel->getName());
            }
            if (channelHists.at(0)->GetNbinsX() > maxBinNum)
            {
                maxBinNum = channelHists.at(0)->GetNbinsX();
            }
            if ((channelHists.at(0)->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
            {
                maxBarWidth = (channelHists.at(0)->GetXaxis()->GetBinWidth(maxBinNum));
            }
            for(auto histogram : channelHists) {
                delete histogram;
                channelHists.clear();
            }
        }
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	std::vector<TH1*> toAdd;
	TList* toMerge = new TList;
    TH1::AddDirectory(kFALSE);
	for (const auto& channel : channels)	
	{
        std::string channelName = channel->getName();
        channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
		if(channelName == std::to_string((int) massTarget)) {
            toAdd = channel->getHists(histType, "signal", scaleToExpected);
        }
        for(auto vechist : toAdd) {
            toMerge->Add(vechist);
        }
	}
    TH1::AddDirectory(kTRUE);
	hist->Merge(toMerge);
	hist->SetLineColor(kBlack);
	hist->SetFillColor(kBlack);
	return hist;
}