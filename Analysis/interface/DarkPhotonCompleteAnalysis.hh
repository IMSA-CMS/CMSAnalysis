#ifndef DARKPHOTONCOMPLETEANALYSIS_HH
#define DARKPHOTONCOMPLETEANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class DarkPhotonCompleteAnalysis : public FullAnalysis
{
    public:
        DarkPhotonCompleteAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() override {return channels;}
        TH1* getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false, std::string channelName = "") const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif
