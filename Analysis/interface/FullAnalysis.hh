#ifndef FULLANALYSIS_HH
#define FULLANALYSIS_HH

#include <vector>
#include "Channel.hh"
#include <memory>
#include "CrossSectionReader.hh"
#include "SimpleEstimator.hh"
#include "SingleProcess.hh"
#include "HistVariable.hh"
#include "Correction.hh"
#include "TH1.h"

class FullAnalysis
{
    public:
        FullAnalysis() {TH1::SetDefaultSumw2();}
        virtual ~FullAnalysis() {}
        //To my knowledge, this has to be defined in derived classes to use that class' variables
        std::shared_ptr<Channel> getChannel(std::string name);
        std::vector<std::shared_ptr<Channel>> getChannels() const {return channels;}
        /**
         * Get histograms method
         * @param histType The variable the histogram should show
         * @param processName The specific process that formed the events, like Signal, Drell-Yan, ZZ, TTbar, etc
         * @param channelName Name of the Channel class to get the process from
         * @return Returns a list of histograms ordered by mass from lowest to highest
        */
        std::vector<TH1*> getHistograms(const std::string& histType, const std::string& processName, const std::string& channel, bool scaleToExpected = false);
        TH1* getHist(std::string histType, std::string processName, bool scaleToExpected = false, std::string channelName = "") const;
        // virtual bool checkChannelName(std::string channelName, double massTarget) const = 0;
        //Process makers for easy use when loading channels
        static SingleProcess makeBasicProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections = {});

    private:
        std::vector<std::shared_ptr<Channel>> channels;

};

#endif
