#ifndef FULLANALYSIS_HH
#define FULLANALYSIS_HH

#include <vector>
#include "Channel.hh"
#include <memory>
#include "CrossSectionReader.hh"
#include "SimpleEstimator.hh"
#include "SingleProcess.hh"
#include "MultiSystematic.hh"
#include "Correction.hh"
#include "TH1.h"
#include <map>

class HistNameFinder;

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
        std::vector<TH1*> getHistograms(const HistVariable& histType, const std::string& processName, const std::string& channel, bool scaleToExpected = false);
        TH1* getHist(HistVariable histType, std::string processName, bool scaleToExpected = false, std::string channelName = "") const;
        // virtual bool checkChannelName(std::string channelName, double massTarget) const = 0;
        //Process makers for easy use when loading channels
        SingleProcess makeBasicProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, 
        std::shared_ptr<CrossSectionReader> crossReader, double luminosity, std::shared_ptr<HistNameFinder> histVariableToFileMapping,
        bool isData = false, double isBranchingRatioFixer = 1);
        
        
        // SingleProcess makeBasicProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, 
        // std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, double luminosity); //std::vector<std::shared_ptr<Correction>> corrections = {}

    protected:
        std::vector<std::shared_ptr<Channel>>& getChannelsProtected() {return channels;}
        
    private:
        std::vector<std::shared_ptr<Channel>> channels;

};

#endif
