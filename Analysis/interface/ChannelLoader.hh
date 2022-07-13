#ifndef CHANNELLOADER_HH
#define CHANNELLOADER_HH

#include "Channel.hh"
#include <memory>
#include "SingleProcess.hh"
#include <string>

class ChannelLoader 
{
    public:
        static std::shared_ptr<Channel> makeChannel(std::string channelName, double massTarget);
        static SingleProcess makeSingleProcess(std::string histogramName, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);
        static SingleProcess makeSignalProcess(std::string histogramName, std::string filePathway, std::string fileName, std::string fitFileName, std::string crossSectionName, std::shared_ptr<CrossSectionReader> crossReader, int massTarget, double luminosity);

    protected:
    //Make construction impossible
        ChannelLoader() {}
};

#endif