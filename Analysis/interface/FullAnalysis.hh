#ifndef FULLANALYSIS_HH
#define FULLANALYSIS_HH

#include <vector>
#include "Channel.hh"
#include <memory>
#include "CrossSectionReader.hh"
#include "SingleProcess.hh"
#include "HistogramFinder.hh"

class FullAnalysis
{
    public:
        virtual ~FullAnalysis() {}
        //To my knowledge, this has to be defined in derived classes to use that class' variables
        virtual std::shared_ptr<Channel> getChannel(std::string name) = 0;
        //Process makers for easy use when loading channels
        static SingleProcess makeSingleProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::shared_ptr<HistogramFinder> fitHistogramFinder, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);
        static SingleProcess makeSignalProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);
};

#endif