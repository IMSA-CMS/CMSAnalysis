#ifndef HIGGSCHANNELSANALYSIS_HH
#define HIGGSCHANNELSANALYSIS_HH

#include "FullAnalysis.hh"
#include "Channel.hh"
#include <memory>
#include <vector>

class HiggsChannelsAnalysis : public FullAnalysis
{
    public:
        HiggsChannelsAnalysis();
        std::shared_ptr<Channel> getChannel(std::string name) override;
	    std::vector<std::shared_ptr<Channel>> getChannels() override {return channels;}
        std::vector<std::string> makeChannels(std::vector<std::string> particles, int length, bool checkPairs);
        TH1* getDecayHist(std::string histType, std::string processName, double massTarget, bool scaleToExpected = false) const override;

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif