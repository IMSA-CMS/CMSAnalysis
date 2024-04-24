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
    //This is a mass we put into background because the
    //MakeSignalProcess method requires it. It doesn't actually
    //make sense so in the future fix this
    double tempMass = 1400;

    //Actual masses for the Higgs signal
    std::vector<double> massTargets {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400};
    //std::vector<std::string> particles = {"e", "u"};
    //std::vector<std::string> names = Utility::channelMaker(particles, 4, false);
    const std::vector<std::string> genSimDecays{"eeee", "eeeu", "eeet", "eeuu", "eeut", "eett", "eueu", "euet", "euuu", "euut", "eutt", "etet", "etuu", "etut", "ettt", "uuuu", "uuut", "uutt", "utut", "uttt", "tttt"};
    const std::vector<std::string> recoDecays{"eeee", "eeeu", "eeuu", "eueu", "euuu", "uuuu", "eee", "eeu", "eue", "euu", "uue", "uuu", "ee", "e e", "eu", "e u", "uu", "u u", "none"};
    
    //const std::vector<std::string> genSimDecays{"eeee"};
    //const std::vector<std::string> recoDecays{"eeee"};

    //Change this file to your folder to use your own cross sections
    //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
    auto reader = std::make_shared<CrossSectionReader>("/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/crossSections.txt");
    const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/"; 
    const std::string signalFilePath = "/uscms/home/ichen2/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/H++MassFiles/";
    double luminosity = 20;

    TH1::SetDefaultSumw2();
    //for(std::string name : names) {
    for (std::string recoDecay : recoDecays){
        std::vector<std::shared_ptr<Process>> backgroundProcesses;

        for (std::string genSimDecay : genSimDecays)
        {
            for(double massTarget : massTargets) 
            {
                auto higgsSignal = std::make_shared<Process>("Higgs Signal " + std::to_string((int) massTarget), 5);
                std::vector<HistVariable> histVariablesSignal;
                std::string decayName = genSimDecay + "_" + recoDecay;
                histVariablesSignal.push_back(HistVariable::InvariantMass(decayName + "__hists/" + decayName + "_Opposite Sign Invariant Mass"));                
                histVariablesSignal.push_back(HistVariable::SameSignMass(decayName + "__hists/" + decayName + "_Same Sign Invariant Mass"));                
                higgsSignal->addProcess(makeSignalProcess(histVariablesSignal, signalFilePath, "Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l", reader, massTarget, luminosity));
                backgroundProcesses.push_back(higgsSignal);
            }
        }    
        std::vector<HistVariable> histVariablesBackground;
        histVariablesBackground.push_back(HistVariable::InvariantMass(recoDecay + "__hists/" + recoDecay + "_Opposite Sign Invariant Mass"));
        histVariablesBackground.push_back(HistVariable::SameSignMass(recoDecay + "__hists/" + recoDecay + "_Same Sign Invariant Mass"));
        
        //cross sections should be all lowercase
        auto ttbarBackground = std::make_shared<Process>("TTBar Background", 2);
        ttbarBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "TTbar_Boson_NA_Decay_LL_Run_2.root", "ttbar_lep50to500", reader, tempMass, luminosity));
        ttbarBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "TTbar_Boson_W_Decay_L_Run_2.root", "ttw", reader, tempMass, luminosity));
        ttbarBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "TTbar_Boson_Z_Decay_LL_Run_2.root", "ttz", reader, tempMass, luminosity));
        
        auto zzBackground = std::make_shared<Process>("ZZ Background", 4);
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_4L_Run_2.root", "zz4l", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2mu_Run_2.root", "zz2e2m", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_2e2tau_Run_2.root", "zz2e2t", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_2mu2tau_Run_2.root", "zz2m2t", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_4e_Run_2.root", "zz4e", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_4mu_Run_2.root", "zz4m", reader, tempMass, luminosity));
        zzBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "ZZ_Decay_4tau_Run_2.root", "zz4t", reader, tempMass, luminosity));

        auto dyBackground = std::make_shared<Process>("DY Background", 3);
        dyBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_10-50_Run_2.root", "dy10to50", reader, tempMass, luminosity));
        dyBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "Drell-Yan_MassCut_50-inf_Run_2.root", "dy50toinf", reader, tempMass, luminosity));
        
        auto multiBosonBackground = std::make_shared<Process>("MultiBoson Background", 6);
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WW_Decay_2L_Run_2.root", "ww2l2nu", reader, tempMass, luminosity));
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWW_Decay_NA_Run_2.root", "www", reader, tempMass, luminosity));
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.root", "wwz", reader, tempMass, luminosity));
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZ_Decay_3L_Run_2.root", "wz3lnu", reader, tempMass, luminosity));
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_WZZ_Decay_NA_Run_2.root", "wzz", reader, tempMass, luminosity));
        multiBosonBackground->addProcess(makeSignalProcess(histVariablesBackground, filePath, "MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.root", "zzz", reader, tempMass, luminosity));
        
        // std::vector<std::shared_ptr<Correction>> corrections = {};
        // auto correction = std::make_shared<ConstantCorrection>(2);
        //corrections.push_back(correction); 

        auto higgsData = std::make_shared<Process>("Higgs Data", 1);
        //higgsData->addProcess(makeSignalProcess(histVariablesBackground, filePath, "SingleMuonRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + std::to_string((int) tempMass), reader, tempMass, luminosity));
        //higgsData->addProcess(makeSignalProcess(histVariablesBackground, filePath, "SingleElectronRun2017B-UL2017_MiniAODv2-v1.root", "higgs4l" + std::to_string((int) tempMass), reader, tempMass, luminosity));
        higgsData->addProcess(makeSignalProcess(histVariablesBackground, filePath, "Data_Trigger_SingleMuon_Year_2018A.root", "higgs4l" + std::to_string((int) tempMass), reader, tempMass, luminosity));

        backgroundProcesses.push_back(higgsData);
        backgroundProcesses.push_back(ttbarBackground);
        backgroundProcesses.push_back(zzBackground);
        backgroundProcesses.push_back(dyBackground);
        backgroundProcesses.push_back(multiBosonBackground);

        auto leptonBackgrounds = std::make_shared<Channel>(recoDecay, backgroundProcesses);
        //leptonBackgrounds->cleanProcesses();
        channels.push_back(leptonBackgrounds);
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

std::vector<TH1*> HiggsCompleteAnalysis::getHistograms(const std::string& histType, const std::string& processName, const std::string& channel, bool scaleToExpected)
{
    auto targetChannel = getChannel(channel);
    return targetChannel->getHists(histType, processName, scaleToExpected);
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