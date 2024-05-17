#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/WindowEstimator.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/SimpleEstimator.hh"
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TH1.h"
#include "TList.h"

std::shared_ptr<Channel> FullAnalysis::getChannel(std::string name)
{
    for (std::shared_ptr<Channel> singleChannel : channels)
    {
        if (singleChannel->getName() == name)
        {
            return singleChannel;
        }
    }
    throw std::runtime_error("Channel of name " + name + " not found.");
}

std::vector<TH1 *> FullAnalysis::getHistograms(const std::string &histType, const std::string &processName, const std::string &channel, bool scaleToExpected)
{
    auto targetChannel = getChannel(channel);
    return targetChannel->getHists(histType, processName, scaleToExpected);
}

// Use this as long as you don't need to use scaleToExpected or don't have fit histograms.
SingleProcess FullAnalysis::makeBasicProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, double luminosity, std::vector<std::shared_ptr<Correction>> corrections)
{
    auto inputFile = std::make_shared<RootFileInput>(filePathway + fileName, histVariables);
    auto histEstimator = std::make_shared<SimpleEstimator>();
    return SingleProcess(crossSectionName, inputFile, crossReader, histEstimator, luminosity, corrections);
}

TH1 *FullAnalysis::getHist(std::string histType, std::string processName, bool scaleToExpected, std::string channelName) const
{
    // int maxBinNum = 0;
    // double maxBarWidth = 0.0;
    // int channelNumber = 0;
    std::string name = processName;
    for (const auto &channel : channels)
    {
        if (channel->getName() == channelName)
        {
            // channelNumber++;
            // std::vector<TH1*> channelHists = channel->getHists(histType, "signal", false);
            TH1 *channelHist = channel->findProcess(processName)->getHist(histType, scaleToExpected);
            if (channelHist == 0)
            {
                throw std::runtime_error("Histogram not found in channel: " + channel->getName());
            }
            // if (channelHist->GetNbinsX() > maxBinNum)
            // {
            //     maxBinNum = channelHist->GetNbinsX();
            // }
            // if ((channelHist->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
            // {
            //     maxBarWidth = (channelHist->GetXaxis()->GetBinWidth(maxBinNum));
            // }
            return channelHist;
        }
    }
    throw std::runtime_error("Channel name " + channelName + " not found!");
    // TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
    // TH1* toAdd;
    // TList* toMerge = new TList;
    // TH1::AddDirectory(kFALSE);
    // for (const auto& channel : channels)
    // {
    //     std::string channelName = channel->getName();
    //     channelName = channelName.substr((channelName.length() - 2) - int(log10((int) massTarget)) + 1, int(log10((int) massTarget)) + 1);
    // 	if(channelName == std::to_string((int) massTarget)) {
    //         toAdd = channel->findProcess(processName)->getHist(histType, scaleToExpected);
    //     }
    //     toMerge->Add(toAdd);
    // }
    // TH1::AddDirectory(kTRUE);
    // hist->Merge(toMerge);
    // hist->SetLineColor(channels.at(0)->findProcess(processName)->getColor());
    // hist->SetFillColor(channels.at(0)->findProcess(processName)->getColor());
    // return hist;
}