#ifndef CHANNELLOADER_HH
#define CHANNELLOADER_HH

#include "Channel.hh"
#include "HistogramFinder.hh"
#include <memory>
#include "SingleProcess.hh"
#include <string>

class ChannelLoader 
{
    public:
        //Put all channel creating in makeChannel function
        static std::shared_ptr<Channel> makeChannel(std::string channelName, double massTarget);
        //Background process creator (uses fitEstimator)
        static SingleProcess makeSingleProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::shared_ptr<HistogramFinder> fitHistogramFinder, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);
        //Signal process creator (uses windowEstimator)
        static SingleProcess makeSignalProcess(std::shared_ptr<HistogramFinder> histogramFinder, std::string filePathway, std::string fileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);

    protected:
        //Make construction impossible
        ChannelLoader() {}
};

#endif