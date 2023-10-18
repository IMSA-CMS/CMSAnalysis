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
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/Correction.hh"
#include "CMSAnalysis/Analysis/interface/ConstantCorrection.hh"
#include <memory>	
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TH1.h"
#include "TList.h"
 
HiggsCompleteAnalysis::HiggsCompleteAnalysis() {
    std::vector<double> massTargets {1400};
    //std::vector<std::string> particles = {"e", "u"};
    //std::vector<std::string> names = Utility::channelMaker(particles, 4, false);
    const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
    const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};

    TH1::SetDefaultSumw2();
    //for(std::string name : names) {
    for (std::string genSimDecay : genSimDecays){
        for (std::string recoDecay : recoDecays){
            for(double massTarget : massTargets) {
                //std::cout << name + particleString + std::to_string((int) massTarget) << std::endl;
                std::vector<HistVariable> histVariablesBackground;
                std::vector<HistVariable> histVariablesData;
                //Change this file to your folder to use your own cross sections
                auto reader = std::make_shared<CrossSectionReader>("/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/crossSections.txt");
                //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
                const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/"; 
                histVariablesBackground.push_back(HistVariable::InvariantMass(recoDecay + "__hists/" + recoDecay + "Opposite Sign Invariant Mass"));
                std::string decayName = genSimDecay + "_" + recoDecay;
                histVariablesData.push_back(HistVariable::InvariantMass(decayName + "__hists/" + decayName + "Opposite Sign Invariant Mass"));
                
                //Add your hists here
                //std::cout << particleString << name << particleString << " Reco Invariant Mass Background" << std::endl;
                //histVariables.push_back(HistVariable::SameSignMass(name + "_hists/" + name + "Muon" + " Reco Same Sign Invariant Mass"));
                //histVariables.push_back(HistVariable::Pt(name + "_hists/" + name + "Muon" + " Reco Leading lepton pT"));
                //histVariables.push_back(HistVariable::MET(particleString + name + "MET"));
                double luminosity = 20;
                
                //cross sections should be all lowercase
                
                /*
                auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
                ttbarBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "TTbartoLL.root", "ttbar_lep50to500", reader, massTarget, luminosity));
                ttbarBackground->addProcess(makeSignalProcess(histVariables, filePath, "TTWJetstoLNu.root", "ttw", reader, massTarget, luminosity));
                ttbarBackground->addProcess(makeSignalProcess(histVariables, filePath, "TTZto2L2Nu.root", "ttz", reader, massTarget, luminosity));
                
                auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto4L.root", "zz4l", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto2e2mu.root", "zz2e2m", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto2e2tau.root", "zz2e2t", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto2mu2tau.root", "zz2m2t", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto4e.root", "zz4e", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto4mu.root", "zz4m", reader, massTarget, luminosity));
                zzBackground->addProcess(makeSignalProcess(histVariables, filePath, "ZZto4tau.root", "zz4t", reader, massTarget, luminosity));


                auto dyBackground = std::make_shared<Process>("DY Background", 3);
                dyBackground->addProcess(makeSignalProcess(histVariables, filePath, "Drell-Yan_10-50.root", "dy10to50", reader, massTarget, luminosity));
                dyBackground->addProcess(makeSignalProcess(histVariables, filePath, "Drell-Yan_50-inf.root", "dy50toinf", reader, massTarget, luminosity));
                */
                auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
                multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_WWto2L.root", "ww2l2nu", reader, massTarget, luminosity));
                /*
                multiBosonBackground->addProcess(makeSignalProcess(histVariables, filePath, "MultiBoson_WWW.root", "www", reader, massTarget, luminosity));
                multiBosonBackground->addProcess(makeSignalProcess(histVariables, filePath, "MultiBoson_WWZJetsto4L.root", "wwz", reader, massTarget, luminosity));
                multiBosonBackground->addProcess(makeSignalProcess(histVariables, filePath, "MultiBoson_WZto3L.root", "wz3lnu", reader, massTarget, luminosity));
                multiBosonBackground->addProcess(makeSignalProcess(histVariables, filePath, "MultiBoson_WZZ.root", "wzz", reader, massTarget, luminosity));
                multiBosonBackground->addProcess(makeSignalProcess(histVariables, filePath, "MultiBoson_ZZZ.root", "zzz", reader, massTarget, luminosity));

                //auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
                //qcdBackground->addProcess(makeSignalProcess(histVariables, filePath, "QCD500_HiggsBackground.root", "qcd500to700", reader, massTarget, luminosity));
                
                auto higgsSignal = std::make_shared<Process>("Higgs Signal", 5);
                higgsSignal->addProcess(makeSignalProcess(histVariables, filePath, "Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
                */
               
                // std::vector<std::shared_ptr<Correction>> corrections = {};
                // auto correction = std::make_shared<ConstantCorrection>(2);
                //corrections.push_back(correction);

                auto higgsData = std::make_shared<Process>("Higgs Data", 1);
                //higgsData->addProcess(makeSignalProcess(histVariables, filePath, "data_singleMuon.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
                //higgsData->addProcess(makeSignalProcess(histVariables, filePath, "data_singleElectron.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
                //higgsData->addProcess(makeSignalProcess(histVariables, filePath, "../python/pythonPlotFull.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
                //higgsData->addProcess(makeSignalProcess(histVariables, filePath, "test.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
                higgsData->addProcess(makeSignalProcess(histVariablesData, filePath, "test.root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));


                //std::vector<std::shared_ptr<Process>> backgroundProcesses = {ttbarBackground, zzBackground, dyBackground, multiBosonBackground, higgsSignal, higgsData};
                std::vector<std::shared_ptr<Process>> backgroundProcesses = {multiBosonBackground, higgsData};

                
                auto leptonBackgrounds = std::make_shared<Channel>(decayName + std::to_string((int) massTarget), backgroundProcesses);
                //leptonBackgrounds->cleanProcesses();
                channels.push_back(leptonBackgrounds);
            }
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

TH1* HiggsCompleteAnalysis::getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected) const {
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