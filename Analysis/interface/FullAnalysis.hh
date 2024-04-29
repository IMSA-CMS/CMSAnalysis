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
        virtual std::shared_ptr<Channel> getChannel(std::string name) = 0;
        virtual std::vector<std::shared_ptr<Channel>> getChannels() = 0;
        virtual TH1* getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false) const = 0;
        //Process makers for easy use when loading channels
        virtual SingleProcess makeSingleProcess(std::vector<HistVariable> histVariables, std::vector<HistVariable> fitHistVariables, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections = {});
        virtual SingleProcess makeSignalProcess(std::vector<HistVariable> histVariables, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity, std::vector<std::shared_ptr<Correction>> corrections = {});
};

#endif
