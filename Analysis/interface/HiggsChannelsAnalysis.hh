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
	    std::vector<std::shared_ptr<Channel>> getChannels() {return channels;}
        std::vector<std::string> makeChannels(std::vector<std::string> particles, int length, bool checkPairs);

    private:
        std::vector<std::shared_ptr<Channel>> channels;
};

#endif