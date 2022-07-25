#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Input.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>
#include <fstream>
#include <memory>	
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include <string>

std::shared_ptr<Channel> ChannelLoader::makeChannel(std::string channelName, double massTarget)
{
    //For new channels, make new if statements with channelNames
    if(channelName == "default")
    {
        auto reader = std::make_shared<CrossSectionReader>("crossSections.txt");
        //filePath is shared between most files. The rest of the filePath to a given file is still given when making singleProcesses.
        const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
        const std::string histName = "Cut4MuonMuon Reco Invariant Mass Background";
        const std::string fitHistName = "Cut4Reco Invariant Mass Background";
        double luminosity = 3000;
        auto ttbarBackground = std::make_shared<Process>("TTBar Background", 1);
        ttbarBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/TTBarPick500.root", "/30SelectBackgroundRuns/TTBar.root", "ttbar_lep", reader, massTarget, luminosity));
        auto zzBackground = std::make_shared<Process>("ZZ Background", 2);
        zzBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/ZZPick4.root", "30SelectBackgroundRuns/ZZ.root", "zz4l", reader, massTarget, luminosity));
        auto dyBackground = std::make_shared<Process>("DY Background", 3);
        dyBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/DY10.root", "30SelectBackgroundRuns/DY10.root", "dy10to50", reader, massTarget, luminosity));
        dyBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/DY50.root", "30SelectBackgroundRuns/DY50.root", "dy50toinf", reader, massTarget, luminosity));
        auto qcdBackground = std::make_shared<Process>("QCD Background", 4);
        qcdBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/QCD500.root", "30SelectBackgroundRuns/QCD500.root", "qcd500to700", reader, massTarget, luminosity));
        qcdBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/QCD700.root", "30SelectBackgroundRuns/QCD700.root", "qcd700to1000", reader, massTarget, luminosity));
        qcdBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/QCD1000.root", "30SelectBackgroundRuns/QCD1000.root", "qcd1000to1500", reader, massTarget, luminosity));
        qcdBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/QCD1500.root", "30SelectBackgroundRuns/QCD1500.root", "qcd1500to2000", reader, massTarget, luminosity));
        qcdBackground->addProcess(makeSingleProcess(histName, fitHistName, filePath, "BackgroundRunCuts/QCD2000.root", "30SelectBackgroundRuns/QCD2000.root", "qcd2000toinf", reader, massTarget, luminosity));
        auto higgsSignal = std::make_shared<Process>("Higgs Signal", 5);
        higgsSignal->addProcess(makeSignalProcess(histName, filePath, "HiggsRunCuts/Higgs" + std::to_string((int) massTarget) + ".root", "higgs4l" + std::to_string((int) massTarget), reader, massTarget, luminosity));
        std::vector<std::shared_ptr<Process>> backgroundProcesses = { ttbarBackground, zzBackground, dyBackground, qcdBackground, higgsSignal };
        auto leptonBackgrounds = std::make_shared<Channel>("Lepton Jet Backgrounds", backgroundProcesses);
        return leptonBackgrounds;
    }
    else {
        throw std::runtime_error("Channel not found in ChannelLoader");
    }
}

SingleProcess ChannelLoader::makeSingleProcess(std::string histogramName, std::string fitHistogramName, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histogramName);
    auto fitInput = std::make_shared<RootFileInput>(filePathway + fitFileName, fitHistogramName);
    auto histEstimator = std::make_shared<FitEstimator>(massTarget, fitInput);
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity);
}

SingleProcess ChannelLoader::makeSignalProcess(std::string histogramName, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity) 
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histogramName);
    auto histEstimator = std::make_shared<WindowEstimator>(massTarget - (massTarget * .05), massTarget + (massTarget * .05));
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity);
}