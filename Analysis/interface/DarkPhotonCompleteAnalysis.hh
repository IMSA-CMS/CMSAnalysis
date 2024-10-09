#ifndef DARKPHOTONCOMPLETEANALYSIS_HH
#define DARKPHOTONCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonCompleteAnalysis : public FullAnalysis
{
    public:
        DarkPhotonCompleteAnalysis(const std::string filePath, const std::string crossSectionPath);
        // bool checkChannelName(std::string channelName, double massTarget);

        // TH1* getHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false, std::string channelName = "") const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
